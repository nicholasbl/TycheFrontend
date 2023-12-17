#include "structmodel.h"


QVariant StructTableModelBase::get_ui_data(int row, QString name) {
    auto name_as_byte_array = name.toUtf8();

    int role = roleNames().key(name_as_byte_array, Qt::DisplayRole);

    return data(this->index(row, 0), role);
}
