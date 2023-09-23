#pragma once

#include <QAbstractTableModel>
#include <QPointer>

class CategoryModel;
class MetricModel;

struct Cell {
    QVector<float> raw_data;
    QVector<float> violin_data;
    QVector<float> box_structure;
    QVector<float> box_outliers;
};

class SimResultModel : public QAbstractTableModel {
    Q_OBJECT

    QPointer<MetricModel>   m_metrics;
    QPointer<CategoryModel> m_categories;

    QVector<Cell> m_cells;

    Cell cell_at(int metric, int category) const;
    int  index_at(int metric, int category) const;

public:
    enum PortfolioRoles {
        RawData = Qt::UserRole + 1,
        ViolinData,
        BoxStructure,
        BoxOutliers,
    };

    explicit SimResultModel(MetricModel*,
                            CategoryModel*,
                            QObject* parent = nullptr);

    // Header:

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index,
                  int                role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

public:
};
