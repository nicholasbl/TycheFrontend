#include "metricmodel.h"

#include <QJsonObject>

MetricModel::MetricModel(QObject* parent) : StructTableModel(parent) { }

QJsonObject MetricRecord::to_request_object() const {
    return {
        { "name", name },
        { "optim_value", optim_value },
        { "bound_type", bound_type },
    };
}
