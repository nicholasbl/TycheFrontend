#pragma once

#include "categorymodel.h"
#include "metricmodel.h"
#include "structmodel.h"

struct MetricRecord;
struct CategoryRecord;

struct ScenarioRecord {
    QString name;
    QString uuid;
    QString description;
    QString image;

    QVector<MetricRecord>   metrics;
    QVector<CategoryRecord> categories;

    MAKE_META(MetaMember(&ScenarioRecord::name, "name"),
              MetaMember(&ScenarioRecord::uuid, "uuid"),
              MetaMember(&ScenarioRecord::description, "description"),
              MetaMember(&ScenarioRecord::image, "image"));
};

class ScenarioModel : public StructTableModel<ScenarioRecord> {
    Q_OBJECT

    int m_current_scenario = 0;

    Q_PROPERTY(int current_scenario READ current_scenario WRITE
                   set_current_scenario NOTIFY current_scenario_changed FINAL)

public:
    explicit ScenarioModel(QObject* parent = nullptr);

    void add_placeholder();

    int  current_scenario() const;
    void set_current_scenario(int index);
signals:
    void current_scenario_changed();
};
