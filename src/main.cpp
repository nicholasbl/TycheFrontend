#include <QApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>

// #include "portfoliomodel.h"

int main(int argc, char* argv[]) {
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
    //    ScenarioModel  scenario_model;

    //    QObject::connect(&NetworkObject::global(),
    //                     &NetworkObject::new_scenario_list,
    //                     &scenario_model,
    //                     &ScenarioModel::recv_new_scenario_list);

    //    QObject::connect(
    //        &scenario_model, &ScenarioModel::use, &model,
    //        &PortfolioModel::use);

    //    NetworkObject::global().ask_scenario_list();

    QQmlApplicationEngine engine;

    //    engine.rootContext()->setContextProperty("portfolio_model", &model);
    //    engine.rootContext()->setContextProperty("scenario_model",
    //    &scenario_model);

    const QUrl url(u"qrc:/qml/main.qml"_qs);

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
