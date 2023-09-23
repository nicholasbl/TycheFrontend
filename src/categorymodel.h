#pragma once

#include "structmodel.h"

struct CategoryRecord {
    QString name;
    QString description;
    int     investment;
    int     max_investment;

    MAKE_META(MetaMember(&CategoryRecord::name, "name"),
              MetaMember(&CategoryRecord::description, "description"),
              MetaMember(&CategoryRecord::investment, "investment", true),
              MetaMember(&CategoryRecord::max_investment, "max_investment"));

    QJsonObject to_request_object() const;
};

class CategoryModel : public StructTableModel<CategoryRecord> {
    Q_OBJECT

public:
    explicit CategoryModel(QObject* parent = nullptr);
};
