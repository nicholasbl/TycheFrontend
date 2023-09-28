#pragma once

#include "scenariomodel.h"
#include "structmodel.h"

#include <QAbstractTableModel>
#include <QPointer>

class SelectedCategoryModel;
class SelectedMetricModel;
class SimResultSumModel;

struct AskRunResult;

struct Cell {
    QVector<float> raw_data;
    QVector<float> violin_x_data;
    QVector<float> violin_y_data;
    QVector<float> box_structure = { 0, 1, 2, 3, 4 };
    QVector<float> box_outliers;

    Cell() = default;
    Cell(QVector<float> data);

    MAKE_META(MetaMember(&Cell::raw_data, "raw_data"),
              MetaMember(&Cell::violin_x_data, "violin_x"),
              MetaMember(&Cell::violin_y_data, "violin_y"),
              MetaMember(&Cell::box_structure, "box_structure"),
              MetaMember(&Cell::box_outliers, "box_outliers"));
};

// =============================================================================

class SimResultModel : public QAbstractTableModel {
    Q_OBJECT

    QPointer<SelectedMetricModel>   m_metrics;
    QPointer<SelectedCategoryModel> m_categories;
    QPointer<SimResultSumModel>     m_sim_sum_model;

    ScenarioRecord m_current_scenario;

    QVector<Cell> m_cells;

    QVector<float> m_all_cell_stats;

    bool   m_edited;
    qint64 m_total_value;


    Q_PROPERTY(QVector<float> all_cell_stats READ get_all_cell_stats WRITE
                   set_all_cell_stats NOTIFY all_cell_stats_changed FINAL)

    Q_PROPERTY(
        bool edited READ edited WRITE setEdited NOTIFY editedChanged FINAL)

    Q_PROPERTY(qint64 total_value READ total_value WRITE set_total_value NOTIFY
                   total_value_changed FINAL)

private:
    Cell cell_at(int metric, int category) const;
    int  index_at(int metric, int category) const;

    void replace_cells(QVector<Cell> new_cells);

public:
    enum PortfolioRoles {
        RawData = Qt::UserRole + 1,
        ViolinXData,
        ViolinYData,
        BoxStructure,
        BoxOutliers,
    };

    explicit SimResultModel(SelectedMetricModel*,
                            SelectedCategoryModel*,
                            SimResultSumModel*,
                            QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index,
                  int                role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVector<float> get_all_cell_stats() const;
    void           set_all_cell_stats(QVector<float> newAll_cell_stats);

    bool edited() const;
    void setEdited(bool newEdited);

    qint64 total_value() const;
    void   set_total_value(qint64 newTotal_value);

    void set_current_scenario(ScenarioRecord);

    void load_data_from(AskRunResult const&);

    void clear();

private slots:
    void source_models_changed();
    void recompute_cell_stats();
    void recompute_total_value();

public slots:
    void ask_run_scenario();

signals:
    void all_cell_stats_changed();
    void editedChanged();
    void total_value_changed();
};


// =============================================================================

class SimResultSumModel : public StructTableModel<Cell> {
    Q_OBJECT
public:
    explicit SimResultSumModel(QObject* parent = nullptr);
};
