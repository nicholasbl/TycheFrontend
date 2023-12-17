#pragma once

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

    QVector<float> m_all_cell_stats;
    QVector<float> m_metric_summary; // lookup by 2*mindex

    bool    m_edited                   = false;
    qint64  m_total_value              = 0;
    qint64  m_opt_portfolio_amount     = 0;
    qint64  m_max_opt_portfolio_amount = 0;
    QString m_current_scenario_name;
    QString m_optimize_target_metric_id;
    QString m_optimize_target_sense = "max";


    Q_PROPERTY(QVector<float> all_cell_stats READ get_all_cell_stats WRITE
                   set_all_cell_stats NOTIFY all_cell_stats_changed FINAL)

    Q_PROPERTY(
        bool edited READ edited WRITE setEdited NOTIFY editedChanged FINAL)

    Q_PROPERTY(qint64 total_value READ total_value WRITE set_total_value NOTIFY
                   total_value_changed FINAL)

    Q_PROPERTY(QString current_scenario_name READ current_scenario_name WRITE
                   set_current_scenario_name NOTIFY
                       current_scenario_name_changed FINAL)

    Q_PROPERTY(
        qint64 opt_portfolio_amount READ opt_portfolio_amount WRITE
            set_opt_portfolio_amount NOTIFY opt_portfolio_amount_changed FINAL)

    Q_PROPERTY(qint64 max_opt_portfolio_amount READ max_opt_portfolio_amount
                   WRITE set_max_opt_portfolio_amount NOTIFY
                       opt_max_portfolio_amount_changed FINAL)

    Q_PROPERTY(QString optimize_target_metric_id READ optimize_target_metric_id
                   WRITE set_optimize_target_metric_id NOTIFY
                       optimize_target_metric_id_changed FINAL)

    Q_PROPERTY(QVector<float> metric_summary READ metric_summary WRITE
                   set_metric_summary NOTIFY metric_summary_changed FINAL)

    Q_PROPERTY(
        QString optimize_target_sense READ optimize_target_sense WRITE
            setOptimize_target_sense NOTIFY optimize_target_senseChanged FINAL)

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

    QVector<float> get_all_cell_stats() const;
    void           set_all_cell_stats(QVector<float> newAll_cell_stats);

    bool edited() const;
    void setEdited(bool newEdited);

    void clear();

    qint64 total_value() const;
    void   set_total_value(qint64 newTotal_value);

    void    set_current_scenario(ScenarioRecord);
    QString current_scenario_name() const;
    void    set_current_scenario_name(const QString& newCurrent_scenario_name);


    qint64 opt_portfolio_amount() const;
    void   set_opt_portfolio_amount(qint64 newOpt_portfolio_amount);

    qint64 max_opt_portfolio_amount() const;
    void   set_max_opt_portfolio_amount(qint64 newMax_opt_portfolio_amount);

    QString optimize_target_metric_id() const;
    void
    set_optimize_target_metric_id(const QString& newOptimize_target_metric_id);

    QVector<float> metric_summary() const;
    void           set_metric_summary(const QVector<float>& newMetric_summary);

    QString optimize_target_sense() const;
    void    setOptimize_target_sense(const QString& newOptimize_target_sense);

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

signals:
    void all_cell_stats_changed();
    void editedChanged();
    void total_value_changed();
    void current_scenario_name_changed();
    void opt_portfolio_amount_changed();
    void opt_max_portfolio_amount_changed();
    void optimize_target_metric_id_changed();
    void metric_summary_changed();
    void optimize_target_senseChanged();
};


// =============================================================================

class SimResultSumModel : public StructTableModel<Cell> {
    Q_OBJECT
public:
    explicit SimResultSumModel(QObject* parent = nullptr);
};
