#pragma once

#include "categorymodel.h"
#include "metricmodel.h"
#include "networkcontroller.h"
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


    ScenarioRecord() = default;

    template <class Archive>
    void archive(Archive& a) {
        a("name", name);
        a("id", uuid);
        a("description", description);
        a("image", image);
        a("metric_defs", metrics);
        a("category_defs", categories);
    }
};

class ScenarioModel : public StructTableModel<ScenarioRecord> {
    Q_OBJECT

    int m_current_scenario = 0;

    Q_PROPERTY(int current_scenario READ current_scenario WRITE
                   set_current_scenario NOTIFY current_scenario_changed FINAL)

public:
    explicit ScenarioModel(QObject* parent = nullptr);

    int  current_scenario() const;
    void set_current_scenario(int index);


public slots:
    void refresh_scenario_list();

    void new_scenario_list(MethodResult);

signals:
    void current_scenario_changed();
};
