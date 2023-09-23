#pragma once

#include "structmodel.h"

struct MetricRecord {
    QString name;
    QString units;


    float optim_min   = 0;
    float optim_max   = 100;
    float optim_value = 60;

    int bound_type = 0;

    MAKE_META(MetaMember(&MetricRecord::name, "name"),
              MetaMember(&MetricRecord::units, "units"),
              MetaMember(&MetricRecord::optim_min, "optim_min"),
              MetaMember(&MetricRecord::optim_max, "optim_max"),
              MetaMember(&MetricRecord::optim_value, "optim_value", true),
              MetaMember(&MetricRecord::bound_type, "bound_type", true));

    QJsonObject to_request_object() const;
};

class MetricModel : public StructTableModel<MetricRecord> {
    Q_OBJECT

public:
    explicit MetricModel(QObject* parent = nullptr);
};
