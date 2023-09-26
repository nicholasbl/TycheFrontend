#include "simresultmodel.h"

#include "categorymodel.h"
#include "metricmodel.h"

#include "stat_math.h"

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
    return m_cells.value(index_at(metric, category));
}

int SimResultModel::index_at(int metric, int category) const {
    return std::clamp<int>(
        m_metrics->rowCount() * metric + category, 0, m_cells.size());
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

    auto const& cell = cell_at(index.row(), index.column());

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
    qDebug() << Q_FUNC_INFO;

    // clear old

    if (!m_cells.isEmpty()) {
        beginRemoveRows({}, 0, std::max(rowCount() - 1, 0));
        m_cells.clear();
        endRemoveRows();
    }

    if (m_sim_sum_model) { m_sim_sum_model->replace(); }

    // build new
    m_cells.resize(m_metrics->rowCount() * m_categories->rowCount());

    if (m_cells.isEmpty()) {
        // empty, bail
        return;
    }

    beginInsertRows({}, 0, m_metrics->rowCount() - 1);

    // FOR NOW
    for (auto& c : m_cells) {
        c = Cell(generate_random_testing_data());
    }

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

void SimResultModel::recompute_cell_stats() {
    Q_ASSERT(!m_cells.isEmpty());

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

    // qDebug() << Q_FUNC_INFO << global_min << global_max;

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
