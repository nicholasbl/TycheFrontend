QT += quick widgets

CONFIG += c++20

SOURCES += \
        ../archivemodel.cpp \
        ../categorymodel.cpp \
        main.cpp \
        ../metricmodel.cpp \
        ../networkcontroller.cpp \
        ../scenariomodel.cpp \
        ../simresultdata.cpp \
        ../simresultmodel.cpp \
        ../stat_math.cpp \
        ../structmodel.cpp

CONFIG += sanitizer sanitize_address

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    build_wasm.py \

HEADERS += \
    ../archivemodel.h \
    ../categorymodel.h \
    ../metricmodel.h \
    ../networkcontroller.h \
    ../scenariomodel.h \
    ../simresultdata.h \
    ../simresultmodel.h \
    ../stat_math.h \
    ../structmodel.h \
    ../doctest.h
