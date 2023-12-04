#pragma once

#include "structmodel.h"

#include <QPointer>
#include <QSortFilterProxyModel>

struct MetricRecord {
    QString name;
    QString description;
    QString image;
    QString id;

    bool selected = false;

    float   optim_value = 0;
    QString bound_type;

    MAKE_META(MetaMember(&MetricRecord::name, "name"),
              MetaMember(&MetricRecord::description, "description"),
              MetaMember(&MetricRecord::image, "image"),
              MetaMember(&MetricRecord::id, "metric_id"),
              MetaMember(&MetricRecord::selected, "selected", true),
              MetaMember(&MetricRecord::optim_value, "optim_value", true),
              MetaMember(&MetricRecord::bound_type, "bound_type", true));

    template <class Archive>
    void archive(Archive& a) {
        a("name", name);
        a("description", description);
        a("image", image);
        a("id", id);
        a("selected", selected);
        a("optim_value", optim_value);
        a("bound_type", bound_type);
    }
};

// =============================================================================

class MetricModel : public StructTableModel<MetricRecord> {
    Q_OBJECT

public:
    explicit MetricModel(QObject* parent = nullptr);

    QSet<QString> selected_indices();

public slots:
    void finalize_choices();
};

// =============================================================================

class SelectedMetricModel : public QSortFilterProxyModel {
    QPointer<MetricModel> m_host;

public:
    SelectedMetricModel(MetricModel*);

    MetricRecord const* get_at(int) const;

    bool filterAcceptsRow(int                source_row,
                          QModelIndex const& source_parent) const override;

    MetricModel* host() const { return m_host; }

    template <class Function>
    void enumerate(Function&& f) {
        for (auto i = 0; i < rowCount(); i++) {
            MetricRecord const& r = *get_at(i);
            f(r, i);
        }
    }
};
