#pragma once

#include "simresultdata.h"

class ArchiveModel : public StructTableModel<RunArchive> {
    Q_OBJECT

    int m_run_counter = 0;

    int next_counter();

public:
    ArchiveModel(QObject* parent = nullptr);

    void ask_run_scenario(AskRunScenario,
                          ScenarioRecord,
                          QSet<QString> selected_metrics,
                          QSet<QString> selected_categories);
    void ask_run_optimize(AskRunOptim,
                          ScenarioRecord,
                          QSet<QString> selected_metrics,
                          QSet<QString> selected_categories);


public slots:
    void select_run(int);

    void delete_run(int);

    void delete_all_runs();

    void ask_save_data(int);

signals:
    void data_selected(RunArchive);

    void new_run_ready();

    void error_from_sim(QString);
};
