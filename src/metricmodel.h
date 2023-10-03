#pragma once

#include "structmodel.h"

#include <QPointer>
#include <QSortFilterProxyModel>

struct MetricRecord {
    QString name;
    QString description;
    QString image;

    float optim_min = 0;
    float optim_max = 100;

    bool selected = false;

    float optim_value = 60;
    int   bound_type  = 0;

    MAKE_META(MetaMember(&MetricRecord::name, "name"),
              MetaMember(&MetricRecord::description, "description"),
              MetaMember(&MetricRecord::image, "image"),
              MetaMember(&MetricRecord::optim_min, "optim_min"),
              MetaMember(&MetricRecord::optim_max, "optim_max"),
              MetaMember(&MetricRecord::selected, "selected", true),
              MetaMember(&MetricRecord::optim_value, "optim_value", true),
              MetaMember(&MetricRecord::bound_type, "bound_type", true));

    template <class Archive>
    void archive(Archive& a) {
        a("name", name);
        a("description", description);
        a("image", image);
        a("starting_opt", optim_value);
        a("opt_slider_min", optim_min);
        a("opt_slider_max", optim_max);
    }
};

// =============================================================================

class MetricModel : public StructTableModel<MetricRecord> {
    Q_OBJECT

public:
    explicit MetricModel(QObject* parent = nullptr);

    QSet<int> selected_indices();

public slots:
    void finalize_choices();
};

// =============================================================================

class SelectedMetricModel : public QSortFilterProxyModel {
    QPointer<MetricModel> m_host;

public:
    SelectedMetricModel(MetricModel*);

    bool filterAcceptsRow(int                source_row,
                          QModelIndex const& source_parent) const override;

    MetricModel* host() const { return m_host; }
};
