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

    for (auto m_i = 0; m_i < m_metrics->rowCount(); m_i++) {
        QVector<float> total;
        for (auto c_i = 0; c_i < m_categories->rowCount(); c_i++) {
            total << cell_at(m_i, c_i).raw_data;
        }
        summary_cells << Cell(total);
    }

    m_sim_sum_model->replace(summary_cells);

    // recompute stats
    recompute_cell_stats();
    recompute_total_value();
}

SimResultModel::SimResultModel(SelectedMetricModel*   m,
                               SelectedCategoryModel* c,
                               SimResultSumModel*     sum,
                               ArchiveModel*          ar,
                               QObject*               parent)
    : QAbstractTableModel(parent),
      m_metrics(m),
      m_categories(c),
      m_sim_sum_model(sum),
      m_archive_model(ar) {

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

    // This is now part of the UI
    //    connect(ar, &ArchiveModel::new_run_ready, this, [this]() {
    //        auto rc = m_archive_model->rowCount() - 1;
    //        auto* ptr = m_archive_model->get_at(rc);
    //        load_data_from(*ptr);
    //    });
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
}

void SimResultModel::load_data_from(RunArchive const& archive) {
    qDebug() << Q_FUNC_INFO;

    set_current_scenario(archive.scenario);

    // set up proper selections

    {
        m_metrics->host()->update_all([&](auto& v, int i) {
            v.selected = archive.selected_metrics.contains(i);
        });

        m_categories->host()->update_all([&](auto& v, int i) {
            v.selected = archive.selected_categories.contains(i);
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
    auto& main_met = *m_metrics->host();

    main_cat.update_all([&](CategoryRecord& r, int i) {
        r.investment = result.cat_state.value(i);
    });


    // TODO: replace with two models; one for data, second for a filtered table.

    for (auto c_i = 0; c_i < result.cells.size(); c_i++) {

        auto dest_cat_index =
            m_categories->mapFromSource(main_cat.index(c_i, 0));

        if (!dest_cat_index.isValid()) {
            // qDebug() << Q_FUNC_INFO << "Category" << c_i << "invalid";
            continue;
        }

        auto const& this_cat = result.cells[c_i];

        for (auto m_i = 0; m_i < this_cat.length(); m_i++) {

            auto dest_mat_index =
                m_metrics->mapFromSource(main_met.index(m_i, 0));

            if (!dest_mat_index.isValid()) {
                // qDebug() << Q_FUNC_INFO << "Metric" << m_i << "invalid";
                continue;
            }

            auto const& this_cell = this_cat[m_i];

            // qDebug() << m_i << c_i << this_cell.values;

            new_cells[index_at(dest_mat_index.row(), dest_cat_index.row())] =
                Cell(this_cell.values);
        }
    }

    replace_cells(new_cells);
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
        state.category_states << CatState {
            .investment = c.selected ? c.investment : -1,
        };
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
    state.opt_method  = "none";

    auto* metric_list = m_metrics->host();

    for (auto const& m : *metric_list) {
        state.metric_states << AskRunOptimMetric {
            .value = m.optim_value,
        };
    }

    m_archive_model->ask_run_optimize(state,
                                      m_current_scenario,
                                      m_metrics->host()->selected_indices(),
                                      m_categories->host()->selected_indices());
}

void SimResultModel::ask_save_image(QImage image) {
#ifdef Q_PROCESSOR_WASM
    QByteArray image_data;
    QBuffer    buffer(&image_data);
    image.save(&buffer, "PNG");
    QFileDialog::saveFileContent("simulation_results.png", image_data);
#else
    auto name = QFileDialog::getSaveFileName(nullptr, "Save Image");

    if (!name.isEmpty()) { image.save(name); }
#endif
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
