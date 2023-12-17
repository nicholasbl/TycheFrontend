#pragma once

#include "structmodel.h"

#include <QPointer>
#include <QSortFilterProxyModel>

struct CategoryRecord {
    QString name;
    QString description;
    QString image;
    QString id;

    qint64 investment     = 0;
    qint64 max_investment = 1000000;

    qint64 opt_limit = -1;

    bool selected = false;

    MAKE_META(MetaMember(&CategoryRecord::name, "name"),
              MetaMember(&CategoryRecord::description, "description"),
              MetaMember(&CategoryRecord::image, "image"),
              MetaMember(&CategoryRecord::id, "category_id"),
              MetaMember(&CategoryRecord::investment, "investment", true),
              MetaMember(&CategoryRecord::max_investment, "max_investment"),
              MetaMember(&CategoryRecord::opt_limit, "opt_limit", true),
              MetaMember(&CategoryRecord::selected, "selected", true));

    template <class Archive>
    void archive(Archive& a) {
        a("name", name);
        a("description", description);
        a("image", image);
        a("id", id);
        a("starting_investment", investment);
        a("max_investment", max_investment);
        a("opt_limit", opt_limit);
        a("selected", selected);
    }
};

// =============================================================================

class CategoryModel : public StructTableModel<CategoryRecord> {
    Q_OBJECT

public:
    explicit CategoryModel(QObject* parent = nullptr);

public slots:
    void finalize_choices();

    QSet<QString> selected_indices();
};

// =============================================================================

class SelectedCategoryModel : public QSortFilterProxyModel {
    Q_OBJECT

    QPointer<CategoryModel> m_host;

    qint64 m_maximum_investment;
    qint64 m_opt_funds_used;

    Q_PROPERTY(
        qint64 maximum_investment READ maximum_investment WRITE
            set_maximum_investment NOTIFY maximum_investment_changed FINAL)

    Q_PROPERTY(qint64 opt_funds_used READ opt_funds_used WRITE
                   set_opt_funds_used NOTIFY opt_funds_used_changed FINAL)

public:
    SelectedCategoryModel(CategoryModel*);

    bool filterAcceptsRow(int                source_row,
                          QModelIndex const& source_parent) const override;

    CategoryRecord const* get_at(int) const;

    CategoryModel* host() const { return m_host; }
    qint64         maximum_investment() const;
    void           set_maximum_investment(qint64 newMaximum_investment);

    template <class Function>
    void enumerate(Function&& f) {
        for (auto i = 0; i < rowCount(); i++) {
            CategoryRecord const& r = *get_at(i);
            f(r, i);
        }
    }

    qint64 opt_funds_used() const;
    void   set_opt_funds_used(qint64 newOpt_funds_used);

    void reset_all_opts();

signals:
    void maximum_investment_changed();
    void opt_funds_used_changed();

public slots:
    QVariant get_ui_data(int row, QString name);

private slots:
    void recompute_stats();
};
