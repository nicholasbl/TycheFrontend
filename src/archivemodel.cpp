#include "archivemodel.h"
#include "networkcontroller.h"

#include <QBuffer>
#include <QFileDialog>

QString ArchiveModel::current_run_name() const {
    return m_current_run_name;
}

void ArchiveModel::setCurrent_run_name(const QString& newCurrent_run_name) {
    if (m_current_run_name == newCurrent_run_name) return;
    m_current_run_name = newCurrent_run_name;
    emit current_run_nameChanged();
}

QString ArchiveModel::current_run_type() const {
    return m_current_run_type;
}

QDateTime ArchiveModel::current_run_time() const {
    return m_current_run_time;
}

void ArchiveModel::setCurrent_run_time(const QDateTime& newCurrent_run_time) {
    if (m_current_run_time == newCurrent_run_time) return;
    m_current_run_time = newCurrent_run_time;
    emit current_run_timeChanged();
}

void ArchiveModel::setCurrent_run_type(const QString& newCurrent_run_type) {
    if (m_current_run_type == newCurrent_run_type) return;
    m_current_run_type = newCurrent_run_type;
    emit current_run_typeChanged();
}

int ArchiveModel::next_counter() {
    return m_run_counter++;
}

ArchiveModel::ArchiveModel(QObject* parent) : StructTableModel(parent) { }


void ArchiveModel::ask_run_scenario(AskRunScenario state,
                                    ScenarioRecord scenario,
                                    QSet<QString>  selected_metrics,
                                    QSet<QString>  selected_categories) {
    qDebug() << Q_FUNC_INFO;

    auto this_id = next_counter();

    auto* method =
        JSONRpcMethod::invoke("run_scenario", QJsonArray() << to_json(state));

    connect(method,
            &JSONRpcMethod::request_failure,
            this,
            &ArchiveModel::error_from_sim);

    connect(method, &JSONRpcMethod::request_success, this, [=](QJsonValue doc) {
        AskRunResult new_sim_data;
        from_json(doc, new_sim_data);

        auto new_run = RunArchive {
            .archive_name = QString("%1 (%2)").arg(scenario.name).arg(this_id),
            .type         = "Simulation",
            .scenario     = scenario,
            .time_date    = QDateTime::currentDateTime(),
            .run_result   = std::move(new_sim_data),
            .selected_metrics    = selected_metrics,
            .selected_categories = selected_categories,
        };

        this->append(new_run);

        emit this->new_run_ready();
    });
}

void ArchiveModel::ask_run_optimize(AskRunOptim    state,
                                    ScenarioRecord scenario,
                                    QSet<QString>  selected_metrics,
                                    QSet<QString>  selected_categories) {
    // qDebug() << Q_FUNC_INFO;

    auto this_id = next_counter();

    auto* method = JSONRpcMethod::invoke("optimize_scenario",
                                         QJsonArray() << to_json(state));

    connect(method,
            &JSONRpcMethod::request_failure,
            this,
            &ArchiveModel::error_from_sim);

    connect(method, &JSONRpcMethod::request_success, this, [=](QJsonValue doc) {
        // qDebug() << doc;
        AskRunResult new_sim_data;
        from_json(doc, new_sim_data);
        auto new_run = RunArchive {
            .archive_name = QString("%1 (%2)").arg(scenario.name).arg(this_id),
            .type         = "Optimization",
            .scenario     = scenario,
            .time_date    = QDateTime::currentDateTime(),
            .run_result   = std::move(new_sim_data),
            .selected_metrics    = selected_metrics,
            .selected_categories = selected_categories,
        };

        this->append(new_run);

        emit this->new_run_ready();
    });
}

void ArchiveModel::select_run(int index) {
    if (index < 0) {
        // do nothing for now
        return;
    }

    auto* ptr = get_at(index);

    if (!ptr) return;

    setCurrent_run_name(ptr->archive_name);
    setCurrent_run_type(ptr->type);
    setCurrent_run_time(ptr->time_date);

    emit this->data_selected(*ptr);
}

void ArchiveModel::delete_run(int index) {
    auto* ptr = get_at(index);
    if (!ptr) return;

    this->remove_at(index);
}

void ArchiveModel::delete_all_runs() {
    this->replace();
}


void ArchiveModel::ask_save_data(int record_id) {
    auto record_ptr = this->get_at(record_id);

    if (!record_ptr) return;

    QByteArray data_to_disk;

    {
        QJsonObject doc_obj = to_json(*record_ptr);
        data_to_disk        = QJsonDocument(doc_obj).toJson();
    }

    auto filename = QString("%1.json").arg(record_ptr->archive_name);

    QFileDialog::saveFileContent(data_to_disk, filename);
}
