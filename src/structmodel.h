#pragma once

#include <QAbstractTableModel>
#include <QDebug>

template <class H, class T>
struct MetaMember {
    using ClassName  = H;
    using ReturnType = T;
    T H::*      access;
    char const* name;
    bool        editable;

    constexpr MetaMember(T H::*      _access,
                         char const* _name,
                         bool        _editable = false)
        : access(_access), name(_name), editable(_editable) { }
};


#define MAKE_META(...)                                                         \
    static constexpr inline auto meta = std::tuple(__VA_ARGS__)

template <class Tuple, class Function>
constexpr auto tuple_for_each(Tuple&& t, Function&& f) {
    std::apply([&](auto&... x) { (..., f(x)); }, t);
}

template <class Tuple, int I, class Function>
struct _get_by_idx {
    static_assert(I >= 1);
    static void check(Tuple&& t, int i, Function&& f) {
        if (I == i) { return f(std::get<I>(t)); }

        _get_by_idx<Tuple, I - 1, Function>::check(t, i, std::move(f));
    }
};

template <class Tuple, class Function>
struct _get_by_idx<Tuple, 0, Function> {
    static void check(Tuple&& t, int i, Function&& f) {
        if (0 == i) { return f(std::get<0>(t)); }

        throw std::runtime_error("Bad index");
    }
};


template <class Tuple, class Function>
void tuple_get(Tuple&& t, int index, Function&& f) {
    using LT = std::remove_cvref_t<Tuple>;

    constexpr static auto S = std::tuple_size_v<LT>;

    _get_by_idx<Tuple, S - 1, Function>::check(t, index, std::move(f));
}

///
/// \brief Get the fields of a record
///
template <class Record>
QStringList get_header() {
    QStringList ret;
    tuple_for_each(Record::meta, [&ret]<class T>(T const& m) {
        // make sure we didnt make a mistake in the record
        static_assert(std::is_same_v<Record, typename T::ClassName>,
                      "Record has a bad accessor. Check classnames.");
        ret << m.name;
    });
    return ret;
}

template <class Record>
QHash<int, QByteArray> const& get_name_map() {
    static QHash<int, QByteArray> ret = []() {
        QHash<int, QByteArray> build;

        int i = 0;

        tuple_for_each(Record::meta, [&build, &i](auto const& thing) {
            build[Qt::UserRole + i] = thing.name;
            i++;
        });
        return build;
    }();
    return ret;
}

template <class Record, class ReturnType>
constexpr int role_for_member(ReturnType Record::*ptr) {
    // Need to find a nice way to have a compile error on missing meta
    int ret       = -1;
    using PtrType = decltype(ptr);
    int i         = 0;
    tuple_for_each(Record::meta, [&i, &ret, ptr](auto const& thing) {
        using LT = decltype(thing.access);

        if constexpr (std::is_same_v<LT, PtrType>) {
            if (thing.access == ptr) ret = Qt::UserRole + i;
        }
        i++;
    });

    Q_ASSERT(ret >= 0);

    return ret;
}

template <class Record>
QVariant record_runtime_get(Record const& r, int i) {
    QVariant ret;
    tuple_get(Record::meta, i, [&r, &ret](auto const& a) {
        ret = QVariant::fromValue(r.*a.access);
    });
    return ret;
}

template <class Record>
bool record_runtime_set(Record& r, int i, QVariant const& v) {
    bool ret = false;

    tuple_get(Record::meta, i, [&v, &ret, &r](auto& _value) {
        if (_value.editable) {
            ret = true;

            using LT = std::remove_cvref_t<decltype(r.*_value.access)>;
            (r.*_value.access) = v.value<LT>();
        }
    });

    return ret;
}

class StructTableModelBase : public QAbstractTableModel {
    Q_OBJECT
public:
    using QAbstractTableModel::QAbstractTableModel;

public slots:
    QVariant get_ui_data(int row, QString name);

signals:
    void content_changed();
};

template <class Record>
class StructTableModel : public StructTableModelBase {
    QVector<Record> m_records;

    QStringList const m_header;

public:
    explicit StructTableModel(QObject* parent = nullptr)
        : StructTableModelBase(parent), m_header(get_header<Record>()) { }

    // Header:
    QVariant headerData(int             section,
                        Qt::Orientation orientation,
                        int             role = Qt::DisplayRole) const override {
        if (orientation != Qt::Orientation::Horizontal) return {};
        if (role != Qt::DisplayRole) return {};

        return m_header.value(section);
    }

    int rowCount(QModelIndex const& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_records.size();
    }

    int columnCount(QModelIndex const& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_header.size();
    }

    QVariant data(QModelIndex const& index,
                  int                role = Qt::DisplayRole) const override {

        if (!index.isValid()) return {};
        if (index.row() >= m_records.size()) return {};

        auto const& item = m_records[index.row()];

        if (role == Qt::DisplayRole or role == Qt::EditRole) {
            return record_runtime_get(item, index.column());
        }

        if (role >= Qt::UserRole) {
            auto local_role = role - Qt::UserRole;

            assert(local_role >= 0);

            if (local_role >= m_header.size()) return {};

            return record_runtime_get(item, local_role);
        }

        return {};
    }


    bool setData(QModelIndex const& index,
                 QVariant const&    value,
                 int                role = Qt::EditRole) override {

        if (data(index, role) == value) return false;

        auto& item = m_records[index.row()];

        int location = -1;

        if (role >= Qt::UserRole) {
            location = role - Qt::UserRole;
        } else {
            location = index.column();
        }

        if (location >= m_header.size()) return false;

        bool ok = record_runtime_set(item, location, value);

        qDebug() << Q_FUNC_INFO << index << value << role << ok;

        if (!ok) return false;

        emit dataChanged(index, index, QList<int>() << role);
        emit content_changed();
        return true;
    }

    Qt::ItemFlags flags(QModelIndex const& index) const override {
        if (!index.isValid()) return Qt::NoItemFlags;

        bool can_edit = false;

        tuple_get(Record::meta, index.column(), [&can_edit](auto const& v) {
            can_edit = v.editable;
        });

        if (!can_edit) return Qt::ItemIsEnabled;

        return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

    QHash<int, QByteArray> roleNames() const override {
        static const auto roles = get_name_map<Record>();

        return roles;
    }

    // this emits a remove signal, instead of a reset
    void remove_all() {
        if (m_records.isEmpty()) return;
        beginRemoveRows(QModelIndex(), 0, std::max(rowCount() - 1, 0));
        m_records.clear();
        endRemoveRows();
        emit content_changed();
    }

    void reset(QList<Record> new_records = {}) {
        beginResetModel();
        m_records = new_records;
        endResetModel();
        emit content_changed();
    }

    Record const* get_at(int i) const {
        if (i < 0) return nullptr;
        if (i >= m_records.size()) return nullptr;
        return &m_records[i];
    }

    auto append(Record const& r) {
        int rc = rowCount();
        beginInsertRows({}, rc, rc);
        m_records << r;
        endInsertRows();
        emit content_changed();
    }

    auto append(QVector<Record> r) {
        if (r.isEmpty()) return;
        int rc = rowCount();
        beginInsertRows({}, rc, rc + r.size() - 1);
        m_records << r;
        endInsertRows();
        emit content_changed();
    }

    auto replace(QVector<Record> r = {}) {
        remove_all();
        append(r);
    }

    void remove_at(int i) {
        if (i < 0) return;
        if (i >= m_records.size()) return;

        beginRemoveRows(QModelIndex(), i, i);
        m_records.remove(i);
        endRemoveRows();
        emit content_changed();
    }

    bool set_at(int i, Record const& r) {
        if (i < 0) return false;
        if (i >= m_records.size()) return false;

        m_records[i] = r;

        auto left  = index(i, 0);
        auto right = index(i, columnCount() - 1);

        emit dataChanged(left, right);
        emit content_changed();
        return true;
    }

    template <class Function>
    void update_at(int i, Function&& f) {
        auto* ptr = get_at(i);
        if (ptr) {
            Record r = *ptr;
            f(r);
            set_at(i, r);
        }
        emit content_changed();
    }

    template <class Function>
    void update_all(Function&& f) {
        if (m_records.isEmpty()) return;

        for (auto i = 0; i < rowCount(); i++) {
            Record& r = m_records[i];
            f(r, i);
        }
        auto left  = index(0, 0);
        auto right = index(rowCount() - 1, columnCount() - 1);

        emit dataChanged(left, right);
        emit content_changed();
    }

    template <class Function>
    void enumerate(Function&& f) {
        for (auto i = 0; i < rowCount(); i++) {
            Record const& r = m_records[i];
            f(r, i);
        }
    }

    auto const& vector() const { return m_records; }

    auto begin() const { return m_records.begin(); }
    auto end() const { return m_records.end(); }
};
