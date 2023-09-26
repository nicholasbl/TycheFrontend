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

void MetricModel::finalize_choices() {
    qDebug() << Q_FUNC_INFO;
    // check if we have at least three selected

    int selected = 0;

    for (auto const& v : *this) {
        selected += v.selected;
    }

    int i = 0;

    while (selected < 3 and i < this->rowCount()) {
        auto* ptr = get_at(i);
        if (!ptr) continue;
        if (!ptr->selected) {
            update_at(i, [&](auto& r) {
                r.selected = true;
                selected++;
            });
        }
        i++;
    }
}

// =============================================================================

SelectedMetricModel::SelectedMetricModel(MetricModel* ptr) : m_host(ptr) {
    setSourceModel(ptr);
}

bool SelectedMetricModel::filterAcceptsRow(
    int                source_row,
    QModelIndex const& source_parent) const {

    if (source_parent.isValid()) return false;

    auto* ptr = m_host->get_at(source_row);

    if (!ptr) return false;

    return ptr->selected;
}
