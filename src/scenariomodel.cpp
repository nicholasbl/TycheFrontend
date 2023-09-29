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
            &JSONRpcMethod::request_failure,
            this,
            &ScenarioModel::scenario_fetch_failure);
}

void ScenarioModel::new_scenario_list(QJsonValue result) {
    qDebug() << Q_FUNC_INFO;

    QVector<ScenarioRecord> record_list;
    from_json(result.toArray(), record_list);
    replace(record_list);
    if (m_current_scenario != 0) {
        set_current_scenario(0);
    } else {
        emit current_scenario_changed();
    }
}
