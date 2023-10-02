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

    template <class Archive>
    void archive(Archive& a) {
        a("name", name);
        a("description", description);
        a("image", image);
        a("starting_investment", investment);
        a("max_investment", max_investment);
    }
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
    Q_OBJECT

    QPointer<CategoryModel> m_host;

    qint64 m_maximum_investment;

    Q_PROPERTY(
        qint64 maximum_investment READ maximum_investment WRITE
            set_maximum_investment NOTIFY maximum_investment_changed FINAL)

public:
    SelectedCategoryModel(CategoryModel*);

    bool filterAcceptsRow(int                source_row,
                          QModelIndex const& source_parent) const override;

    CategoryRecord const* get_at(int) const;

    CategoryModel* host() const { return m_host; }
    qint64         maximum_investment() const;
    void           set_maximum_investment(qint64 newMaximum_investment);
signals:
    void maximum_investment_changed();

private slots:
    void recompute_stats();
};
