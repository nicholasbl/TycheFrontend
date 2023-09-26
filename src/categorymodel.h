#pragma once

#include "structmodel.h"

#include <QPointer>
#include <QSortFilterProxyModel>

struct CategoryRecord {
    QString name;
    QString description;
    QString image;
    int     investment;
    int     max_investment = 1000000;

    bool selected = false;

    MAKE_META(MetaMember(&CategoryRecord::name, "name"),
              MetaMember(&CategoryRecord::description, "description"),
              MetaMember(&CategoryRecord::image, "image"),
              MetaMember(&CategoryRecord::investment, "investment", true),
              MetaMember(&CategoryRecord::max_investment, "max_investment"),
              MetaMember(&CategoryRecord::selected, "selected", true));

    QJsonObject to_request_object() const;
};

// =============================================================================

class CategoryModel : public StructTableModel<CategoryRecord> {
    Q_OBJECT

public:
    explicit CategoryModel(QObject* parent = nullptr);

public slots:
    void finalize_choices();
};

// =============================================================================

class SelectedCategoryModel : public QSortFilterProxyModel {
    QPointer<CategoryModel> m_host;

public:
    SelectedCategoryModel(CategoryModel*);

    bool filterAcceptsRow(int                source_row,
                          QModelIndex const& source_parent) const override;

    CategoryRecord const* get_at(int) const;
};
