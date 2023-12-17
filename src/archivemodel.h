#pragma once

#include "simresultdata.h"

class ArchiveModel : public StructTableModel<RunArchive> {
    Q_OBJECT

    QString   m_current_run_name;
    QString   m_current_run_type;
    QDateTime m_current_run_time;

    int m_run_counter = 0;

    int next_counter();

public:
    ArchiveModel(QObject* parent = nullptr);

    void ask_run_scenario(AskRunScenario,
                          ScenarioRecord,
                          QSet<QString> selected_metrics,
                          QSet<QString> selected_categories);


    QString current_run_name() const;
    void    setCurrent_run_name(const QString& newCurrent_run_name);

    QString current_run_type() const;
    void    setCurrent_run_type(const QString& newCurrent_run_type);

    QDateTime current_run_time() const;
    void      setCurrent_run_time(const QDateTime& newCurrent_run_time);


public slots:
    void select_run(int);

    void delete_run(int);

    void delete_all_runs();

    void ask_save_data(int);

signals:
    void data_selected(RunArchive);

    void new_run_ready();

    void sim_exception(QString);
    void sim_system_failure(QString);

    void current_run_nameChanged();
    void current_run_timeChanged();
    void current_run_typeChanged();

private:
    Q_PROPERTY(QString current_run_name READ current_run_name WRITE
                   setCurrent_run_name NOTIFY current_run_nameChanged FINAL)
    Q_PROPERTY(QDateTime current_run_time READ current_run_time WRITE
                   setCurrent_run_time NOTIFY current_run_timeChanged FINAL)
    Q_PROPERTY(QString current_run_type READ current_run_type WRITE
                   setCurrent_run_type NOTIFY current_run_typeChanged FINAL)
};

// =============================================================================

struct MetricResult {
    QString name;
    float   value;

    MAKE_META(MetaMember(&MetricResult::name, "name"),
              MetaMember(&MetricResult::value, "value"));
};

struct CategoryResult {
    QString name;
    float   value;

    MAKE_META(MetaMember(&CategoryResult::name, "name"),
              MetaMember(&CategoryResult::value, "value"));
};

class MetricResultModel : public StructTableModel<MetricResult> {
    using StructTableModel::StructTableModel;
};

class CategoryResultModel : public StructTableModel<CategoryResult> {
    using StructTableModel::StructTableModel;
};

class OptimizationResultModel : public QObject {
    Q_OBJECT

    CategoryResultModel* m_cat_result;
    MetricResultModel*   m_met_result;

public:
    OptimizationResultModel(QObject* = nullptr);

    void ask_run_optimize(AskRunOptim,
                          ScenarioRecord,
                          QSet<QString> selected_metrics,
                          QSet<QString> selected_categories);

public slots:
    CategoryResultModel* cat_result();
    MetricResultModel*   met_result();

private slots:
    void on_result(QJsonValue doc,
                   ScenarioRecord,
                   QSet<QString> selected_metrics,
                   QSet<QString> selected_categories);

signals:
    void sim_exception(QString);
    void sim_system_failure(QString);
};
