QT += quick widgets

CONFIG += c++20

SOURCES += \
        archivemodel.cpp \
        categorymodel.cpp \
        main.cpp \
        metricmodel.cpp \
        networkcontroller.cpp \
        scenariomodel.cpp \
        simresultdata.cpp \
        simresultmodel.cpp \
        stat_math.cpp \
        structmodel.cpp

RESOURCES += \
    res.qrc

wasm {
    # optim for size
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -Os
} else {
    # only use sanitizers on desktop platforms
    CONFIG += sanitizer sanitize_address
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    build_wasm.py \

HEADERS += \
    archivemodel.h \
    categorymodel.h \
    metricmodel.h \
    networkcontroller.h \
    qt_helpers.h \
    scenariomodel.h \
    simresultdata.h \
    simresultmodel.h \
    stat_math.h \
    structmodel.h
