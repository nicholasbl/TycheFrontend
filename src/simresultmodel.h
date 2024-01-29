#pragma once

#include "qt_helpers.h"
#include "scenariomodel.h"
#include "structmodel.h"

#include <QAbstractTableModel>
#include <QImage>
#include <QPointer>

class SelectedCategoryModel;
class SelectedMetricModel;
class SimResultSumModel;
class ArchiveModel;
class OptimizationResultModel;

struct RunArchive;
struct AskRunOptimResult;

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

    QPointer<SelectedMetricModel>     m_metrics;
    QPointer<SelectedCategoryModel>   m_categories;
    QPointer<SimResultSumModel>       m_sim_sum_model;
    QPointer<ArchiveModel>            m_archive_model;
    QPointer<OptimizationResultModel> m_opt_archive_model;

    ScenarioRecord m_current_scenario;

    QVector<Cell> m_cells;

    WRITABLE_PROPERTY(QVector<float>, all_cell_stats);
    WRITABLE_PROPERTY(QVector<float>, metric_summary); // lookup by 2*mindex

    WRITABLE_PROPERTY(qint64, total_value);
    WRITABLE_PROPERTY(qint64, opt_portfolio_amount);
    WRITABLE_PROPERTY(qint64, max_opt_portfolio_amount);
    WRITABLE_PROPERTY(QString, current_scenario_name);
    WRITABLE_PROPERTY(QString, optimize_target_metric_id);
    WRITABLE_PROPERTY(QString, optimize_target_sense);

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
                            ArchiveModel*,
                            OptimizationResultModel*,
                            QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index,
                  int                role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    bool edited() const;
    void setEdited(bool newEdited);

    void clear();

    void set_current_scenario(ScenarioRecord);

private slots:
    void source_models_changed();
    void recompute_cell_stats();
    void recompute_total_value();

public slots:
    void ask_run_scenario();
    void ask_run_optimize();

    void ask_save_image(QImage);

    void load_data_from(RunArchive const&);
    void load_from(AskRunOptimResult const&);
};


// =============================================================================

class SimResultSumModel : public StructTableModel<Cell> {
    Q_OBJECT
public:
    explicit SimResultSumModel(QObject* parent = nullptr);
};
