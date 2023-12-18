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
            &JSONRpcMethod::request_system_error,
            this,
            &ArchiveModel::sim_system_failure);
    connect(method,
            &JSONRpcMethod::request_exception,
            this,
            &ArchiveModel::sim_exception);

    connect(method,
            &JSONRpcMethod::request_success,
            this,
            [=, this](QJsonValue doc) {
                // qDebug() << "DOC" << doc;
                AskRunResult new_sim_data;
                from_json(doc, new_sim_data);

                auto new_run = RunArchive {
                    .archive_name =
                        QString("%1 (%2)").arg(scenario.name).arg(this_id),
                    .type                = "Simulation",
                    .scenario            = scenario,
                    .time_date           = QDateTime::currentDateTime(),
                    .run_result          = std::move(new_sim_data),
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
    auto record_ptr = const_cast<RunArchive*>(this->get_at(record_id));

    if (!record_ptr) return;

    QByteArray data_to_disk;

    {
        QJsonObject doc_obj = to_json(*record_ptr);
        data_to_disk        = QJsonDocument(doc_obj).toJson();
    }

    auto filename = QString("%1.json").arg(record_ptr->archive_name);

    QFileDialog::saveFileContent(data_to_disk, filename);
}

// ==============================================


OptimizationResultModel::OptimizationResultModel(QObject* p)
    : QObject(p),
      m_cat_result(new CategoryResultModel(this)),
      m_met_result(new MetricResultModel(this)) { }

void OptimizationResultModel::ask_run_optimize(
    AskRunOptim    state,
    ScenarioRecord scenario,
    QSet<QString>  selected_metrics,
    QSet<QString>  selected_categories) {
    // This needs to be moved to an optimize module, but we are hacking it for
    // now

    // auto this_id = next_counter();

    auto* method = JSONRpcMethod::invoke("optimize_scenario",
                                         QJsonArray() << to_json(state));

    connect(method,
            &JSONRpcMethod::request_system_error,
            this,
            &OptimizationResultModel::sim_system_failure);
    connect(method,
            &JSONRpcMethod::request_exception,
            this,
            &OptimizationResultModel::sim_exception);

    connect(method,
            &JSONRpcMethod::request_success,
            this,
            [=, this](QJsonValue doc) {
                this->on_result(
                    doc, scenario, selected_metrics, selected_categories);
            });
}

CategoryResultModel* OptimizationResultModel::cat_result() {
    return m_cat_result;
}
MetricResultModel* OptimizationResultModel::met_result() {
    return m_met_result;
}

void OptimizationResultModel::on_result(QJsonValue     doc,
                                        ScenarioRecord scenario,
                                        QSet<QString>  selected_metrics,
                                        QSet<QString>  selected_categories) {
    qDebug() << Q_FUNC_INFO << doc;

    // qDebug() << doc;
    AskRunOptimResult new_sim_data;
    from_json(doc, new_sim_data);

    m_cat_result->replace();
    m_met_result->replace();

    for (auto iter = new_sim_data.metric_limits.begin();
         iter != new_sim_data.metric_limits.end();
         ++iter) {

        auto item = std::find_if(
            scenario.metrics.begin(),
            scenario.metrics.end(),
            [&](MetricRecord const& p) { return p.id == iter.key(); });

        if (item == scenario.metrics.end()) { continue; }

        m_met_result->append(MetricResult {
            .name  = item->name,
            .value = iter->value,
        });
    }

    // we actually need to sort this to retain the same order as the selected

    for (auto const& category_info : scenario.categories) {

        auto cat_limit_iter =
            new_sim_data.category_limits.find(category_info.id);

        if (cat_limit_iter == new_sim_data.category_limits.end()) { continue; }

        m_cat_result->append(CategoryResult {
            .name  = category_info.name,
            .value = float(cat_limit_iter->value),
        });
    }
}
