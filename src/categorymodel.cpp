#include "categorymodel.h"

#include <QJsonObject>

CategoryModel::CategoryModel(QObject* parent) : StructTableModel(parent) { }

// QJsonObject CategoryRecord::to_request_object() const {
//     return {
//         { "name", name },
//         { "investment", investment },
//     };
// }

void CategoryModel::finalize_choices() {
    // qDebug() << Q_FUNC_INFO;
    //  check if we have at least three selected

    int selected = 0;

    for (auto const& v : *this) {
        selected += v.selected;
    }

    // qDebug() << Q_FUNC_INFO << selected;

    int i = 0;

    while (selected < 3 and i < this->rowCount()) {
        // qDebug() << Q_FUNC_INFO << selected << i;
        auto* ptr = get_at(i);
        if (!ptr) continue;
        if (!ptr->selected) {
            update_at(i, [&](auto& r) {
                // qDebug() << Q_FUNC_INFO << "UPDATE";
                r.selected = true;
                selected++;
            });
        }
        i++;
    }
}


// =============================================================================

SelectedCategoryModel::SelectedCategoryModel(CategoryModel* ptr) : m_host(ptr) {
    setSourceModel(ptr);

    connect(ptr,
            &CategoryModel::dataChanged,
            this,
            &SelectedCategoryModel::recompute_stats);

    connect(ptr,
            &CategoryModel::rowsInserted,
            this,
            &SelectedCategoryModel::recompute_stats);

    connect(ptr,
            &CategoryModel::rowsRemoved,
            this,
            &SelectedCategoryModel::recompute_stats);
}

bool SelectedCategoryModel::filterAcceptsRow(
    int                source_row,
    QModelIndex const& source_parent) const {

    if (source_parent.isValid()) return false;

    auto* ptr = m_host->get_at(source_row);

    if (!ptr) return false;

    return ptr->selected;
}

CategoryRecord const* SelectedCategoryModel::get_at(int row) const {
    return m_host->get_at(mapToSource(index(row, 0)).row());
}

qint64 SelectedCategoryModel::maximum_investment() const {
    return m_maximum_investment;
}

void SelectedCategoryModel::set_maximum_investment(
    qint64 newMaximum_investment) {
    if (m_maximum_investment == newMaximum_investment) return;
    m_maximum_investment = newMaximum_investment;
    emit maximum_investment_changed();
}

void SelectedCategoryModel::recompute_stats() {
    qDebug() << Q_FUNC_INFO;
    int role = role_for_member(&CategoryRecord::investment);

    qDebug() << Q_FUNC_INFO << "Role is" << role << m_host->roleNames();

    quint64 max = 0;

    for (int i = 0; i < rowCount(); i++) {
        auto d = data(index(i, 0), role).toULongLong();
        max    = std::max(d, max);
        qDebug() << Q_FUNC_INFO << "Item is" << d;
    }

    qDebug() << Q_FUNC_INFO << "Max is" << max;

    set_maximum_investment(max);
}
