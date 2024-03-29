#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class JSONRpcMethod : public QObject {
    Q_OBJECT
    QString m_id;

    void sanitize_result(QNetworkReply* reply, QString id);

    JSONRpcMethod(QString                host,
                  QString                method_name,
                  QJsonDocument          params,
                  QNetworkAccessManager* parent);

public:
    static void    set_default_host(QString);
    static QString default_host();

    static void    set_default_image_host(QString);
    static QString default_image_host();

    static JSONRpcMethod* invoke(QString method_name);
    static JSONRpcMethod* invoke(QString method_name, QJsonObject param);
    static JSONRpcMethod* invoke(QString method_name, QJsonArray params);

signals:
    void request_success(QJsonValue);
    void request_system_error(QString);
    void request_exception(QString);
};


// =============================================================================


struct ToJsonObject {
    QJsonObject value;

    template <class T>
    void operator()(const char* v, T& t);
};


template <class T>
QJsonObject to_json(T& t) {
    ToJsonObject obj;
    t.archive(obj);
    return obj.value;
}

inline QJsonValue to_json(bool b) {
    return b;
}

inline QJsonValue to_json(QString s) {
    return s;
}

inline QJsonValue to_json(qint64 s) {
    return s;
}

inline QJsonValue to_json(float s) {
    return s;
}

inline QJsonValue to_json(QDateTime& d) {
    return QJsonValue::fromVariant(d);
}

template <class T>
QJsonValue to_json(QVector<T> list) {
    QJsonArray array;
    for (auto& t : list) {
        array << to_json(t);
    }
    return array;
}

template <class T>
QJsonValue to_json(QSet<T> list) {
    QJsonArray array;
    for (auto& t : list) {
        array << to_json(t);
    }
    return array;
}

template <class T>
QJsonValue to_json(QHash<QString, T> obj) {
    QJsonObject ret;
    for (auto iter = obj.begin(); iter != obj.end(); ++iter) {
        ret[iter.key()] = to_json(iter.value());
    }
    return ret;
}

template <class T>
void ToJsonObject::operator()(const char* v, T& t) {
    value[v] = to_json(t);
}

// =============================================================================

struct FromJsonObject {
    QJsonObject value;

    template <class T>
    void operator()(const char* v, T& t) {
        auto iter = value.constFind(v);

        if (iter != value.constEnd()) { from_json(*iter, t); }
    }
};

template <class T>
void from_json(QJsonValue v, T& t) {
    FromJsonObject obj { v.toObject() };

    // qDebug() << obj.value;

    t.archive(obj);
}

inline void from_json(QJsonValue v, bool& b) {
    b = v.toBool();
}

inline void from_json(QJsonValue v, QString& s) {
    s = v.toString();
}

inline void from_json(QJsonValue v, qint64& s) {
    if (v.isDouble()) {
        s = v.toDouble();
    } else {
        s = v.toInt();
    }
}

inline void from_json(QJsonValue v, float& s) {
    if (v.isDouble()) {
        s = v.toDouble();
    } else {
        s = v.toInteger();
    }
}

template <class T>
void from_json(QJsonValue v, QVector<T>& list) {
    list.clear();
    QJsonArray array = v.toArray();
    for (auto const& t : array) {
        T new_value;
        from_json(t, new_value);
        list << new_value;
    }
}

template <class T>
void from_json(QJsonValue v, QHash<QString, T>& dest_obj) {
    dest_obj.clear();
    QJsonObject source_obj = v.toObject();
    for (auto iter = source_obj.begin(); iter != source_obj.end(); ++iter) {
        T new_value;
        from_json(iter.value(), new_value);
        dest_obj[iter.key()] = new_value;
    }
}
