#include "simresultmodel.h"

#include "categorymodel.h"
#include "metricmodel.h"
#include "networkcontroller.h"
#include "stat_math.h"

struct CatState {
    int investment;

    template <class Archive>
    void archive(Archive& a) {
        a("investment", investment);
    }
};

struct AskRunScenario {


    QString           scenario_id;
    QVector<CatState> category_states;

    template <class Archive>
    void archive(Archive& a) {
        a("scenario_id", scenario_id);
        a("category_states", category_states);
    }
};

struct AskReplyCell {
    QVector<float> values;

    template <class Archive>
    void archive(Archive& a) {
        a("x", values);
    }
};

struct AskRunResult {
    QString scenario_id;

    QVector<int> cat_state;
    QVector<int> met_state;

    QVector<QVector<AskReplyCell>> cells;

    template <class Archive>
    void archive(Archive& a) {
        a("scenario_id", scenario_id);
        a("category_state", cat_state);
        a("metric_state", met_state);
        a("cells", cells);
    }
};

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
                               QObject*               parent)
    : QAbstractTableModel(parent),
      m_metrics(m),
      m_categories(c),
      m_sim_sum_model(sum) {

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


    // TODO: Remove
    for (auto m_i = 0; m_i < m_metrics->rowCount(); m_i++) {
        for (auto c_i = 0; c_i < m_categories->rowCount(); c_i++) {
            auto idx       = index_at(m_i, c_i);
            new_cells[idx] = Cell(generate_random_testing_data(m_i * 10, 10));
        }
    }

    replace_cells(new_cells);
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
    m_current_scenario = record;
}

void SimResultModel::load_data_from(AskRunResult const& result) {
    qDebug() << Q_FUNC_INFO;

    // resize
    QVector<Cell> new_cells(m_metrics->rowCount() * m_categories->rowCount());

    if (new_cells.isEmpty() or result.cells.isEmpty()) {
        // empty, bail
        return;
    }

    auto& main_cat = *m_categories->host();
    auto& main_met = *m_metrics->host();

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

    auto* method =
        JSONRpcMethod::invoke("run_scenario", QJsonArray() << to_json(state));

    connect(method,
            &JSONRpcMethod::request_completed,
            this,
            [this](MethodResult result) {
                result.visit(
                    [this](QJsonValue doc) {
                        AskRunResult new_sim_data;
                        from_json(doc, new_sim_data);
                        load_data_from(new_sim_data);
                    },
                    [](QString err) {
                        qCritical() << "Unable to get result:" << err;
                    });
            });
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
