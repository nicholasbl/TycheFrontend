#pragma once

#define QOBJECT_READONLY_PROPERTY(typeName, propertyName)                      \
public:                                                                        \
    Q_PROPERTY(typeName* propertyName READ propertyName CONSTANT)              \
protected:                                                                     \
    QPointer<typeName> m_##propertyName;                                       \
                                                                               \
public:                                                                        \
    typeName* propertyName() const {                                           \
        return m_##propertyName;                                               \
    }                                                                          \
                                                                               \
public:


#define WRITABLE_PROPERTY(typeName, propertyName)                              \
public:                                                                        \
    Q_PROPERTY(typeName propertyName READ propertyName WRITE                   \
                   set_##propertyName NOTIFY propertyName##_changed)           \
protected:                                                                     \
    typeName m_##propertyName;                                                 \
                                                                               \
public:                                                                        \
    typeName propertyName() const {                                            \
        return m_##propertyName;                                               \
    }                                                                          \
public Q_SLOTS:                                                                \
    void set_##propertyName(const typeName& newVal) {                          \
        if (m_##propertyName == newVal) return;                                \
        m_##propertyName = newVal;                                             \
        emit propertyName##_changed(m_##propertyName);                         \
    }                                                                          \
Q_SIGNALS:                                                                     \
    void propertyName##_changed(typeName);                                     \
                                                                               \
public:
