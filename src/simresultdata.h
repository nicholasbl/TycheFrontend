#pragma once

#include <QDateTime>
#include <QHash>
#include <QString>
#include <QVector>

#include "scenariomodel.h"

struct AskRunScenario {
    QString                scenario_id;
    QHash<QString, qint64> category_states;

    template <class Archive>
    void archive(Archive& a) {
        a("scenario_id", scenario_id);
        a("category_states", category_states);
    }
};

// =============================================================================

struct AskReplyCell {
    QVector<float> values;

    template <class Archive>
    void archive(Archive& a) {
        a("x", values);
    }
};

struct AskRunResult {
    QString scenario_id;

    QString opt_metric_id;
    QString opt_sense;

    QHash<QString, qint64> cat_state;
    QHash<QString, qint64> met_state;

    QHash<QString, QHash<QString, QVector<float>>> cells;

    template <class Archive>
    void archive(Archive& a) {
        a("scenario_id", scenario_id);
        a("opt_metric_id", opt_metric_id);
        a("opt_sense", opt_sense);
        a("category_state", cat_state);
        a("metric_state", met_state);
        a("cells", cells);
    }
};

// =============================================================================

struct AskRunOptimMetric {
    QString metric_id;
    float   value;
    QString bound_type;

    template <class Archive>
    void archive(Archive& a) {
        a("metric_id", metric_id);
        a("value", value);
        a("bound_type", bound_type);
    }
};

struct AskRunOptim {
    QString                    scenario_id;
    qint64                     portfolio;
    QString                    metric_target;
    QString                    optim_sense;
    QVector<AskRunOptimMetric> metric_states;

    template <class Archive>
    void archive(Archive& a) {
        a("scenario_id", scenario_id);
        a("portfolio", portfolio);
        a("metric_target", metric_target);
        a("optimize_sense", optim_sense);
        a("metric_states", metric_states);
    }
};


// =============================================================================

struct RunArchive {
    QString        archive_name;
    QString        type;
    ScenarioRecord scenario;
    QDateTime      time_date;
    AskRunResult   run_result;
    QSet<QString>  selected_metrics;
    QSet<QString>  selected_categories;

    MAKE_META(MetaMember(&RunArchive::archive_name, "name", true),
              MetaMember(&RunArchive::type, "type"),
              MetaMember(&RunArchive::time_date, "time_date"));

    template <class Archive>
    void archive(Archive& a) {
        a("archive_name", archive_name);
        a("scenario", scenario);
        a("time_date", time_date);
        a("run_result", run_result);
        a("selected_metrics", selected_metrics);
        a("selected_categories", selected_categories);
    }
};
