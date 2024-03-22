#include "scenariomodel.h"

#include "networkcontroller.h"

#include <QUuid>

// =============================================================================

ScenarioModel::ScenarioModel(QObject* parent) : StructTableModel(parent) { }

int ScenarioModel::current_scenario() const {
    return m_current_scenario;
}

void ScenarioModel::set_current_scenario(int index) {
    if (m_current_scenario == index) return;
    m_current_scenario = index;
    emit current_scenario_changed();
}

void ScenarioModel::refresh_scenario_list() {
    qDebug() << Q_FUNC_INFO;
    auto* reply = JSONRpcMethod::invoke("get_scenarios");

    connect(reply,
            &JSONRpcMethod::request_success,
            this,
            &ScenarioModel::new_scenario_list);
    connect(reply,
            &JSONRpcMethod::request_system_error,
            this,
            &ScenarioModel::scenario_fetch_failure);
    connect(reply,
            &JSONRpcMethod::request_exception,
            this,
            &ScenarioModel::scenario_fetch_failure);
}

template <class T>
bool check_component_ids(QVector<T> const& t) {
    if (t.isEmpty()) return false;
    QSet<QString> known_ids;
    for (auto const& v : t) {
        if (known_ids.contains(v.id)) return false;
        known_ids.insert(v.id);
    }
    return true;
}

static bool is_valid(ScenarioRecord const& record) {
    bool metrics_ok = check_component_ids(record.metrics);
    bool cats_ok    = check_component_ids(record.categories);

    return metrics_ok and cats_ok;
}

inline void fixup_record(ScenarioRecord& record) {
    auto new_host = QUrl(JSONRpcMethod::default_host()).host();

    record.image = record.image.replace(QStringLiteral("localhost"), new_host);
}

void ScenarioModel::new_scenario_list(QJsonValue result) {
    qDebug() << Q_FUNC_INFO;

    QVector<ScenarioRecord> record_list;
    from_json(result.toArray(), record_list);

    // remove any possibly bad records
    // and we have no erase_if
    QVector<ScenarioRecord> ok_record_list;

    // a lovely hack here to change host info for images
    bool need_fixup = !JSONRpcMethod::default_host().contains("localhost");

    for (auto const& v : record_list) {
        if (is_valid(v)) {
            ok_record_list << v;
            if (need_fixup) fixup_record(ok_record_list.back());
        }
    }

    replace(ok_record_list);


    if (m_current_scenario != 0) {
        set_current_scenario(0);
    } else {
        emit current_scenario_changed();
    }
}
