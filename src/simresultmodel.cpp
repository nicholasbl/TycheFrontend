#include "simresultmodel.h"

#include "archivemodel.h"
#include "categorymodel.h"
#include "metricmodel.h"
#include "simresultdata.h"
#include "stat_math.h"

#include <QBuffer>
#include <QByteArray>
#include <QFileDialog>

// =============================================================================

Cell::Cell(QVector<float> data) {
    raw_data = data;

    auto violin_result = compute_violin(data);
    violin_x_data      = violin_result.x;
    violin_y_data      = violin_result.y;

    auto box_result = compute_boxplot(data);
    box_structure   = box_result.structure;
    box_outliers    = box_result.outliers;
}

// =============================================================================

Cell SimResultModel::cell_at(int metric, int category) const {
    auto idx = index_at(metric, category);
    Q_ASSERT(idx >= 0 and idx < m_cells.size());
    return m_cells.value(idx);
}

int SimResultModel::index_at(int metric, int category) const {
    return m_metrics->rowCount() * category + metric;
    // qDebug() << Q_FUNC_INFO << metric << category;
    // return ret;
}

void SimResultModel::replace_cells(QVector<Cell> new_cells) {
    qDebug() << Q_FUNC_INFO;

    // clear old
    clear();

    beginInsertRows({}, 0, m_metrics->rowCount() - 1);

    // build new
    m_cells = new_cells;

    endInsertRows();

    // now add in summary stats
    // compute summary per each metric

    QVector<Cell> summary_cells;

    QVector<float> metric_min_stats(m_metrics->rowCount(),
                                    std::numeric_limits<float>::max());
    QVector<float> metric_max_stats(m_metrics->rowCount(),
                                    std::numeric_limits<float>::lowest());
    bool           stats_changed = false;

    for (auto m_i = 0; m_i < m_metrics->rowCount(); m_i++) {
        QVector<float> total;
        for (auto c_i = 0; c_i < m_categories->rowCount(); c_i++) {
            total << cell_at(m_i, c_i).raw_data;
        }
        auto new_cell = Cell(total);

        if (new_cell.raw_data.size()) {
            stats_changed                 = true;
            auto [metric_min, metric_max] = std::minmax_element(
                new_cell.raw_data.begin(), new_cell.raw_data.end());

            metric_min_stats[m_i] =
                std::min(metric_min_stats[m_i], *metric_min);
            metric_max_stats[m_i] =
                std::max(metric_max_stats[m_i], *metric_max);
        }

        summary_cells << new_cell;
    }

    QVector<float> complete_metric_stats;
    complete_metric_stats.reserve(metric_max_stats.size() +
                                  metric_min_stats.size());

    if (stats_changed) {
        for (auto i = 0; i < metric_min_stats.size(); i++) {
            complete_metric_stats << metric_min_stats[i];
            complete_metric_stats << metric_max_stats[i];
        }
    } else {
        for (auto i = 0; i < metric_min_stats.size(); i++) {
            complete_metric_stats << 0;
            complete_metric_stats << 1;
        }
    }

    qDebug() << "METRIC STATS" << complete_metric_stats;

    set_metric_summary(complete_metric_stats);


    m_sim_sum_model->replace(summary_cells);

    // recompute stats
    recompute_cell_stats();
    recompute_total_value();
}

SimResultModel::SimResultModel(SelectedMetricModel*     m,
                               SelectedCategoryModel*   c,
                               SimResultSumModel*       sum,
                               ArchiveModel*            ar,
                               OptimizationResultModel* oar,
                               QObject*                 parent)
    : QAbstractTableModel(parent),
      m_metrics(m),
      m_categories(c),
      m_sim_sum_model(sum),
      m_archive_model(ar),
      m_opt_archive_model(oar) {

    m_all_cell_stats = { 0, 1 };

    connect(m,
            &SelectedMetricModel::rowsInserted,
            this,
            &SimResultModel::source_models_changed);
    connect(m,
            &SelectedMetricModel::rowsRemoved,
            this,
            &SimResultModel::source_models_changed);

    connect(c,
            &SelectedCategoryModel::rowsInserted,
            this,
            &SimResultModel::source_models_changed);
    connect(c,
            &SelectedCategoryModel::rowsRemoved,
            this,
            &SimResultModel::source_models_changed);

    connect(c,
            &SelectedCategoryModel::dataChanged,
            this,
            &SimResultModel::recompute_total_value);

    connect(ar,
            &ArchiveModel::data_selected,
            this,
            &SimResultModel::load_data_from);
}


int SimResultModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;

    return m_categories->rowCount();
}

int SimResultModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;

    return m_metrics->rowCount();
}

QVariant SimResultModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant();

    auto const& cell = cell_at(index.column(), index.row());

    switch (role) {
    case RawData: return QVariant::fromValue(cell.raw_data);
    case ViolinXData: return QVariant::fromValue(cell.violin_x_data);
    case ViolinYData: return QVariant::fromValue(cell.violin_y_data);
    case BoxStructure: return QVariant::fromValue(cell.box_structure);
    case BoxOutliers: return QVariant::fromValue(cell.box_outliers);
    }

    return QVariant();
}

QHash<int, QByteArray> SimResultModel::roleNames() const {
    static QHash<int, QByteArray> const roles = {
        { RawData, "raw_data" },         { ViolinXData, "violin_x" },
        { ViolinYData, "violin_y" },     { BoxStructure, "box_structure" },
        { BoxOutliers, "box_outliers" },
    };

    return roles;
}

void SimResultModel::source_models_changed() {
    qDebug() << Q_FUNC_INFO << m_metrics->rowCount()
             << m_categories->rowCount();


    // build new
    QVector<Cell> new_cells(m_metrics->rowCount() * m_categories->rowCount());

    replace_cells(new_cells);

    auto& cat_host = *m_categories->host();

    quint64 max = 0;

    for (auto const& v : cat_host) {
        if (v.selected) max += v.max_investment;
    }

    qDebug() << Q_FUNC_INFO << max;

    set_max_opt_portfolio_amount(max);
}

void SimResultModel::recompute_cell_stats() {
    qDebug() << Q_FUNC_INFO;
    if (m_cells.isEmpty()) return;

    float global_min = m_cells[0].raw_data.value(0);
    float global_max = global_min;

    for (auto const& c : m_cells) {
        auto [smallest, largest] =
            std::minmax_element(c.raw_data.begin(), c.raw_data.end());

        if (smallest == c.raw_data.end() or largest == c.raw_data.end()) {
            continue;
        }

        global_min = std::min(*smallest, global_min);
        global_max = std::max(*largest, global_max);

        // qDebug() << Q_FUNC_INFO << c.box_structure << *smallest << *largest;
    }

    qDebug() << Q_FUNC_INFO << global_min << global_max;

    set_all_cell_stats({ global_min, global_max });
}

void SimResultModel::recompute_total_value() {
    qint64 sum = 0;

    for (auto i = 0; i < m_categories->rowCount(); i++) {
        auto* ptr = m_categories->get_at(i);

        if (!ptr) continue;

        sum += ptr->investment;
    }

    set_total_value(sum);
}

QVector<float> SimResultModel::get_all_cell_stats() const {
    return m_all_cell_stats;
}

void SimResultModel::set_all_cell_stats(QVector<float> newAll_cell_stats) {
    if (m_all_cell_stats == newAll_cell_stats) return;
    m_all_cell_stats = newAll_cell_stats;
    emit all_cell_stats_changed();
}

void SimResultModel::set_current_scenario(ScenarioRecord record) {
    if (record.uuid == m_current_scenario.uuid) return;

    // set up basics

    m_current_scenario = record;
    set_current_scenario_name(record.name);

    // now set up ancillary parts
    m_metrics->host()->replace(record.metrics);
    m_categories->host()->replace(record.categories);

    // set up some other UI
    if (auto ptr = m_metrics->get_at(0); ptr) {
        set_optimize_target_metric_id(ptr->id);
    }
}

void SimResultModel::load_data_from(RunArchive const& archive) {
    qDebug() << Q_FUNC_INFO;

    set_current_scenario(archive.scenario);

    // set up proper selections

    {
        m_metrics->host()->update_all([&](auto& v, int) {
            v.selected = archive.selected_metrics.contains(v.id);
        });

        m_categories->host()->update_all([&](auto& v, int) {
            v.selected = archive.selected_categories.contains(v.id);
        });
    }

    auto const& result = archive.run_result;


    // resize
    QVector<Cell> new_cells(m_metrics->rowCount() * m_categories->rowCount());

    if (new_cells.isEmpty() or result.cells.isEmpty()) {
        // empty, bail
        return;
    }

    auto& main_cat = *m_categories->host();
    // auto& main_met = *m_metrics->host();

    main_cat.update_all([&](CategoryRecord& r, int) {
        auto value   = result.category_states.value(r.id);
        r.investment = value;
        qDebug() << "Loading up cat opt" << r.investment;
    });

    // TODO: replace with two models; one for data, second for a filtered table.

    // we put BOTH cat and metric ids in here. after all, they are using
    // UUIDs...
    QHash<QString, int> id_to_index;

    m_categories->enumerate(
        [&](auto const& v, int idx) { id_to_index[v.id] = idx; });
    m_metrics->enumerate(
        [&](auto const& v, int idx) { id_to_index[v.id] = idx; });

    for (auto cat_iter = result.cells.begin(); cat_iter != result.cells.end();
         ++cat_iter) {

        auto const& cat_id = cat_iter.key();

        if (!id_to_index.contains(cat_id)) continue;

        auto const& cat_index = id_to_index[cat_id];

        auto const& cat_dict = cat_iter.value();

        for (auto met_iter = cat_dict.begin(); met_iter != cat_dict.end();
             ++met_iter) {
            auto const& met_id = met_iter.key();

            if (!id_to_index.contains(met_id)) continue;

            auto const& met_index = id_to_index[met_id];

            auto const& cell_info = met_iter.value();

            new_cells[index_at(met_index, cat_index)] = Cell(cell_info);
        }
    }

    replace_cells(new_cells);
}

void SimResultModel::load_from(AskRunOptimResult const& result) {
    if (!result.opt_metric_id.isEmpty()) {
        set_optimize_target_metric_id(result.opt_metric_id);
    } else if (auto p = m_metrics->get_at(0); p) {
        set_optimize_target_metric_id(p->id);
    }

    auto& main_cat = *m_categories->host();
    auto& main_met = *m_metrics->host();

    main_cat.update_all([&](CategoryRecord& r, int) {
        auto value   = result.category_limits.value(r.id);
        r.investment = value.value;
        r.opt_limit  = value.limit;
        qDebug() << "Loading up cat opt" << r.investment << r.opt_limit;
    });

    main_met.update_all([&](MetricRecord& r, int) {
        r.bound_type  = "";
        r.optim_value = 0;

        auto iter = result.metric_limits.find(r.id);

        if (iter != result.metric_limits.end()) {
            qDebug() << "CAN UPDATE METRIC" << iter.value().sense
                     << iter.value().limit;
            r.bound_type  = iter.value().sense;
            r.optim_value = iter.value().limit;
        }
    });

    setOptimize_target_sense(result.opt_sense);
}

void SimResultModel::clear() {
    qDebug() << Q_FUNC_INFO;
    if (!m_cells.isEmpty()) {
        beginRemoveRows({}, 0, std::max(rowCount() - 1, 0));
        m_cells.clear();
        endRemoveRows();
    }

    if (m_sim_sum_model) { m_sim_sum_model->replace(); }
}

void SimResultModel::ask_run_scenario() {

    AskRunScenario state;

    state.scenario_id = m_current_scenario.uuid;

    auto* clist = m_categories->host();
    for (auto const& c : *clist) {
        state.category_states[c.id] = c.selected ? c.investment : -1;
    }

    m_archive_model->ask_run_scenario(state,
                                      m_current_scenario,
                                      m_metrics->host()->selected_indices(),
                                      m_categories->host()->selected_indices());
}

void SimResultModel::ask_run_optimize() {

    AskRunOptim state;

    state.scenario_id = m_current_scenario.uuid;
    state.portfolio   = opt_portfolio_amount();
    state.metric_target = m_optimize_target_metric_id;
    state.optim_sense   = m_optimize_target_sense;

    auto* metric_list = m_metrics->host();
    auto* category_list = m_categories->host();

    for (auto const& m : *metric_list) {

        // if (m.id == m_optimize_target_metric_id) { continue; }
        if (m.bound_type.isEmpty()) { continue; }

        state.metric_states << AskRunOptimMetric {
            .metric_id  = m.id,
            .value      = m.optim_value,
            .bound_type = m.bound_type,
        };
    }

    for (auto const& c : *category_list) {
        auto limit = c.opt_limit > 0 ? c.opt_limit : opt_portfolio_amount();

        state.category_states << AskRunOptimCategory {
            .category_id = c.id,
            .value       = limit,
        };
    }

    m_opt_archive_model->ask_run_optimize(
        state,
        m_current_scenario,
        m_metrics->host()->selected_indices(),
        m_categories->host()->selected_indices());
}

void SimResultModel::ask_save_image(QImage image) {
    QByteArray image_data;

    {
        QBuffer buffer(&image_data);
        image.save(&buffer, "PNG");
    }

    auto filename = QString("%1.png").arg(m_current_scenario_name);

    QFileDialog::saveFileContent(image_data, filename);
}

// =============================================================================

SimResultSumModel::SimResultSumModel(QObject* parent)
    : StructTableModel(parent) { }

bool SimResultModel::edited() const {
    return m_edited;
}

void SimResultModel::setEdited(bool newEdited) {
    if (m_edited == newEdited) return;
    m_edited = newEdited;
    emit editedChanged();
}

qint64 SimResultModel::total_value() const {
    return m_total_value;
}

void SimResultModel::set_total_value(qint64 newTotal_value) {
    if (m_total_value == newTotal_value) return;
    m_total_value = newTotal_value;
    emit total_value_changed();
}

QString SimResultModel::current_scenario_name() const {
    return m_current_scenario_name;
}

void SimResultModel::set_current_scenario_name(
    const QString& newCurrent_scenario_name) {
    if (m_current_scenario_name == newCurrent_scenario_name) return;
    m_current_scenario_name = newCurrent_scenario_name;
    emit current_scenario_name_changed();
}

qint64 SimResultModel::opt_portfolio_amount() const {
    return m_opt_portfolio_amount;
}

void SimResultModel::set_opt_portfolio_amount(qint64 newOpt_portfolio_amount) {
    if (m_opt_portfolio_amount == newOpt_portfolio_amount) return;
    m_categories->reset_all_opts();
    m_opt_portfolio_amount = newOpt_portfolio_amount;
    emit opt_portfolio_amount_changed();
}

qint64 SimResultModel::max_opt_portfolio_amount() const {
    return m_max_opt_portfolio_amount;
}

void SimResultModel::set_max_opt_portfolio_amount(
    qint64 newMax_opt_portfolio_amount) {
    if (m_max_opt_portfolio_amount == newMax_opt_portfolio_amount) return;
    m_max_opt_portfolio_amount = newMax_opt_portfolio_amount;
    emit opt_max_portfolio_amount_changed();
}

QString SimResultModel::optimize_target_metric_id() const {
    return m_optimize_target_metric_id;
}

void SimResultModel::set_optimize_target_metric_id(
    const QString& newOptimize_target_metric_id) {
    qDebug() << Q_FUNC_INFO << newOptimize_target_metric_id;
    if (m_optimize_target_metric_id == newOptimize_target_metric_id) return;
    m_optimize_target_metric_id = newOptimize_target_metric_id;
    emit optimize_target_metric_id_changed();
}

QVector<float> SimResultModel::metric_summary() const {
    return m_metric_summary;
}

void SimResultModel::set_metric_summary(
    const QVector<float>& newMetric_summary) {
    if (m_metric_summary == newMetric_summary) return;
    m_metric_summary = newMetric_summary;
    emit metric_summary_changed();
}
QString SimResultModel::optimize_target_sense() const {
    return m_optimize_target_sense;
}

void SimResultModel::setOptimize_target_sense(
    const QString& newOptimize_target_sense) {
    if (m_optimize_target_sense == newOptimize_target_sense) return;
    m_optimize_target_sense = newOptimize_target_sense;
    emit optimize_target_senseChanged();
}
