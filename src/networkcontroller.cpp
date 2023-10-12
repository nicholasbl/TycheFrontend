#include "networkcontroller.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUuid>


template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

struct MethodResult {
    std::variant<QJsonValue, QString> storage;

    MethodResult() = default;
    MethodResult(QJsonValue);
    MethodResult(QString);

    template <class SuccessF, class FailF>
    void visit(SuccessF&& sf, FailF&& ff) {
        std::visit(overloaded { [&](QJsonValue doc) { sf(doc); },
                                [&](QString err) { ff(err); } },
                   storage);
    }
};

MethodResult::MethodResult(QJsonValue doc) : storage(doc) { }
MethodResult::MethodResult(QString string) : storage(string) { }


// =============================================================================

static QString& get_default_host() {
    static QString host;
    return host;
}

static QNetworkAccessManager* get_manager() {
    static auto network_manager = std::make_unique<QNetworkAccessManager>();
    return network_manager.get();
}

void JSONRpcMethod::set_default_host(QString s) {
    get_default_host() = s;
}

JSONRpcMethod* JSONRpcMethod::invoke(QString method_name) {
    return new JSONRpcMethod(
        get_default_host(), method_name, QJsonDocument(), get_manager());
}

JSONRpcMethod* JSONRpcMethod::invoke(QString method_name, QJsonObject param) {
    return new JSONRpcMethod(
        get_default_host(), method_name, QJsonDocument(param), get_manager());
}

JSONRpcMethod* JSONRpcMethod::invoke(QString method_name, QJsonArray params) {
    return new JSONRpcMethod(
        get_default_host(), method_name, QJsonDocument(params), get_manager());
}

static MethodResult sanitize_result(QNetworkReply* reply, QString id) {
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "RPC network error:" << reply->errorString();
        return QString("RPC networking error:" + reply->errorString());
    }

    auto bytes = reply->readAll();

    if (bytes.isEmpty()) {
        qWarning() << "RPC response is empty";
        return QString("RPC response is empty");
    }

    QJsonParseError error;
    auto            document = QJsonDocument::fromJson(bytes, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Unable to parse RPC response" << error.errorString();

        qWarning() << "Context:" << bytes.mid(error.offset - 50, 100);

        return QString("Failed to parse response");
    }

    auto object = document.object();

    if (object["jsonrpc"] != "2.0") {
        qWarning() << "Unable to parse RPC response";
        return QString("Failed to parse response");
    }

    if (object["id"].toString() != id) {
        qWarning() << "Wrong reply id";
        return QString("Bad reply ID");
    }

    auto iter = object.constFind("result");

    if (iter != object.constEnd()) { return MethodResult(*iter); }

    iter = object.constFind("error");

    if (iter != object.constEnd()) {
        auto err_obj = iter->toObject();

        auto code    = err_obj["code"].toInt();
        auto message = err_obj["message"].toString();
        auto data    = err_obj["data"];

        QString err_str =
            QString("Method failed: %1: %2").arg(code).arg(message);

        qWarning() << "Method failed: " << code << ":" << message << "-"
                   << data;

        return err_str;
    }

    return QString("Malformed reply from server");
}

JSONRpcMethod::JSONRpcMethod(QString                host,
                             QString                method_name,
                             QJsonDocument          params,
                             QNetworkAccessManager* parent)
    : QObject(parent) {

    m_id = QUuid::createUuid().toString();

    QJsonObject request_object = {
        { "jsonrpc", "2.0" },
        { "method", method_name },
        { "id", m_id },
    };

    if (params.isArray()) {
        request_object["params"] = params.array();
    } else if (params.isObject()) {
        request_object["params"] = params.object();
    }

    auto qnetwork_request = QNetworkRequest(host);
    qnetwork_request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                               "application/json");

    // qnetwork_request.setRawHeader("Access-Control-Allow-Origin", "*");


    auto* reply =
        parent->post(qnetwork_request, QJsonDocument(request_object).toJson());

    connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);

    //    connect(reply, &QNetworkReply::errorOccurred, this, []() {
    //        emit this->request_completed(QString("Network error occurred"));
    //    });
    //    connect(reply, &QNetworkReply::sslErrors, this,
    //    &MyClass::slotSslErrors);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        this->deleteLater();
        auto result = sanitize_result(reply, m_id);

        result.visit(
            [this](QJsonValue result) { emit this->request_success(result); },
            [this](QString err) { emit this->request_failure(err); });
    });
}
