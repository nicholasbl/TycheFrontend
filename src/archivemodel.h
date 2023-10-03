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
                          QSet<int> selected_metrics,
                          QSet<int> selected_categories);
    void ask_run_optimize(AskRunOptim,
                          ScenarioRecord,
                          QSet<int> selected_metrics,
                          QSet<int> selected_categories);


public slots:
    void select_run(int);

    void delete_run(int);

    void delete_all_runs();

signals:
    void data_selected(RunArchive);

    void new_run_ready();

    void error_from_sim(QString);
};
