#include <QApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>

#include "categorymodel.h"
#include "metricmodel.h"
#include "scenariomodel.h"
#include "simresultmodel.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

void on_scenario_changed(ScenarioModel& scenario_model,
                         MetricModel&   metric_model,
                         CategoryModel& category_model) {
    qDebug() << Q_FUNC_INFO;
    auto current_index = scenario_model.current_scenario();

    auto* ptr = scenario_model.get_at(current_index);

    if (!ptr) return;

    qDebug() << Q_FUNC_INFO << "Metric model size:" << metric_model.rowCount();
    qDebug() << Q_FUNC_INFO
             << "Category model size:" << category_model.rowCount();

    metric_model.replace(ptr->metrics);
    category_model.replace(ptr->categories);
}

int main(int argc, char* argv[]) {
    doctest::Context context;

    context.applyCommandLine(argc, argv);

    int test_res = context.run();

    if (context.shouldExit()) { return test_res; }

    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    auto fonts = {
        ":/fonts/fa_solid_6.otf",    ":/fonts/Roboto-Regular.ttf",
        ":/fonts/Roboto-Bold.ttf",   ":/fonts/Roboto-BoldItalic.ttf",
        ":/fonts/Roboto-Italic.ttf",
    };

    for (auto font : fonts) {
        auto font_install_result = QFontDatabase::addApplicationFont(font);

        if (font_install_result < 0) {
            qWarning() << "Missing fonts!" << font_install_result;
        } else {
            qInfo() << "Added font" << font << font_install_result
                    << QFontDatabase::applicationFontFamilies(
                           font_install_result);
        }
    }

    //    PortfolioModel model;
    ScenarioModel scenario_model;
    MetricModel   metric_model;
    CategoryModel category_model;

    SelectedMetricModel   selected_metric_model(&metric_model);
    SelectedCategoryModel selected_category_model(&category_model);

    SimResultSumModel sim_result_sum;

    SimResultModel sim_result_model(
        &selected_metric_model, &selected_category_model, &sim_result_sum);

    QObject::connect(&scenario_model,
                     &ScenarioModel::current_scenario_changed,
                     &scenario_model,
                     [&]() {
                         on_scenario_changed(
                             scenario_model, metric_model, category_model);
                     });

    scenario_model.add_placeholder();

    QQmlApplicationEngine engine;

    // This is technically a slower way than using plugins, however, the plugin
    // stuff has always been...odd.
    auto register_as = [&](auto* t, QString name) {
        engine.rootContext()->setContextProperty(name, t);
    };

    register_as(&scenario_model, "scenario_model");
    register_as(&metric_model, "metric_model");
    register_as(&category_model, "category_model");
    register_as(&selected_metric_model, "selected_metric_model");
    register_as(&selected_category_model, "selected_category_model");
    register_as(&sim_result_model, "sim_result_model");
    register_as(&sim_result_sum, "sim_result_sum_model");

    QUrl const url(u"qrc:/qml/main.qml"_qs);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
