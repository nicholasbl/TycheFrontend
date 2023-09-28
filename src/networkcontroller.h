#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

#include <variant>


template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

class QNetworkAccessManager;

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

Q_DECLARE_METATYPE(MethodResult);


class JSONRpcMethod : public QObject {
    Q_OBJECT
    QString m_id;

    JSONRpcMethod(QString                host,
                  QString                method_name,
                  QJsonDocument          params,
                  QNetworkAccessManager* parent);

public:
    static void set_default_host(QString);

    static JSONRpcMethod* invoke(QString method_name);
    static JSONRpcMethod* invoke(QString method_name, QJsonObject param);
    static JSONRpcMethod* invoke(QString method_name, QJsonArray params);

signals:
    void request_completed(MethodResult);
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

inline QJsonValue to_json(QString s) {
    return s;
}

inline QJsonValue to_json(int s) {
    return s;
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


inline void from_json(QJsonValue v, QString& s) {
    s = v.toString();
}

inline void from_json(QJsonValue v, int& s) {
    s = v.toInt();
}

inline void from_json(QJsonValue v, float& s) {
    s = v.toDouble();
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