#include "categorymodel.h"

#include <QJsonObject>

CategoryModel::CategoryModel(QObject* parent) : StructTableModel(parent) { }

QJsonObject CategoryRecord::to_request_object() const {
    return {
        { "name", name },
        { "investment", investment },
    };
}
