#include "networkcontroller.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUuid>


static QString& get_default_host() {
    static QString host;
    return host;
}

static QNetworkAccessManager* get_manager() {
    static auto network_manager = std::make_unique<QNetworkAccessManager>();
    return network_manager.get();
}

QString JSONRpcMethod::default_host() {
    return get_default_host();
}

void JSONRpcMethod::set_default_host(QString s) {
    qDebug() << "Setting RPC host to" << s;
    get_default_host() = s;
}

static QString& get_image_host() {
    static QString host;
    return host;
}

void JSONRpcMethod::set_default_image_host(QString s) {
    qDebug() << "Setting image host to" << s;
    get_image_host() = s;
}

QString JSONRpcMethod::default_image_host() {
    return get_image_host();
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

void JSONRpcMethod::sanitize_result(QNetworkReply* reply, QString id) {
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "RPC network error:" << reply->errorString();
        emit request_system_error("RPC networking error:" +
                                  reply->errorString());
        return;
    }

    auto bytes = reply->readAll();

    if (bytes.isEmpty()) {
        qWarning() << "RPC response is empty";
        emit request_system_error("RPC response is empty");
        return;
    }

    QJsonParseError error;
    auto            document = QJsonDocument::fromJson(bytes, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Unable to parse RPC response" << error.errorString();

        qWarning() << "Context:" << bytes.mid(error.offset - 50, 100);

        emit request_system_error("Failed to parse response");
        return;
    }

    auto object = document.object();

    if (object["jsonrpc"] != "2.0") {
        qWarning() << "Unable to parse RPC response";
        emit request_system_error("Failed to parse response");
        return;
    }

    if (object["id"].toString() != id) {
        qWarning() << "Wrong reply id";
        emit request_system_error("Bad reply ID");
        return;
    }

    // first find a result
    auto iter = object.constFind("result");

    // if we have a result, return it
    if (iter != object.constEnd()) {
        emit request_success(*iter);
        return;
    }

    // otherwise we are looking for the exception info
    iter = object.constFind("error");

    if (iter != object.constEnd()) {
        auto err_obj = iter->toObject();

        auto code    = err_obj["code"].toInt(-1);
        auto message = err_obj["message"].toString();
        auto data    = err_obj["data"].toString();

        qWarning() << "Method failed: " << code << ":" << message << "-"
                   << data;

        if (code < 0) {
            QString err_str =
                QString("Method failed: %1: %2").arg(message, data);

            emit request_system_error(err_str);
            return;
        }

        emit request_exception(message);

        return;
    }

    // if we dont have either, we have a problem.
    emit request_system_error("Malformed reply from server");
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
        sanitize_result(reply, m_id);
    });
}
