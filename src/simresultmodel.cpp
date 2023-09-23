#include "simresultmodel.h"

#include "categorymodel.h"
#include "metricmodel.h"

Cell SimResultModel::cell_at(int metric, int category) const {
    return m_cells.value(index_at(metric, category));
}

int SimResultModel::index_at(int metric, int category) const {
    return std::clamp<int>(
        m_metrics->rowCount() * metric + category, 0, m_cells.size());
}

SimResultModel::SimResultModel(MetricModel*   m,
                               CategoryModel* c,
                               QObject*       parent)
    : QAbstractTableModel(parent) {
    m_metrics    = m;
    m_categories = c;
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
    case ViolinData: return QVariant::fromValue(cell.violin_data);
    case BoxStructure: return QVariant::fromValue(cell.box_structure);
    case BoxOutliers: return QVariant::fromValue(cell.box_outliers);
    }

    return QVariant();
}

QHash<int, QByteArray> SimResultModel::roleNames() const {
    static QHash<int, QByteArray> const roles = {
        { RawData, "raw_data" },
        { ViolinData, "violin_data" },
        { BoxStructure, "box_structure" },
        { BoxOutliers, "box_outliers" },
    };

    return roles;
}
