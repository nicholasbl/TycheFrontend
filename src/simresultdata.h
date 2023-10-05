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

    QHash<QString, qint64> cat_state;
    QHash<QString, qint64> met_state;

    QHash<QString, QHash<QString, QVector<float>>> cells;

    template <class Archive>
    void archive(Archive& a) {
        a("scenario_id", scenario_id);
        a("category_state", cat_state);
        a("metric_state", met_state);
        a("cells", cells);
    }
};

// =============================================================================

struct AskRunOptimMetric {
    float   value;
    QString bound_type;

    template <class Archive>
    void archive(Archive& a) {
        a("value", value);
        a("bound_type", bound_type);
    }
};

struct AskRunOptim {
    QString                    scenario_id;
    qint64                     portfolio;
    QString                    opt_method;
    QVector<AskRunOptimMetric> metric_states;

    template <class Archive>
    void archive(Archive& a) {
        a("scenario_id", scenario_id);
        a("portfolio", portfolio);
        a("opt_method", opt_method);
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
