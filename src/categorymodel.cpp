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
