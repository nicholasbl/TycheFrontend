QT += quick charts widgets

CONFIG += c++20

SOURCES += \
        categorymodel.cpp \
        main.cpp \
        metricmodel.cpp \
        scenariomodel.cpp \
        simresultmodel.cpp \
        stat_math.cpp \
        structmodel.cpp

RESOURCES += \
    res.qrc

QMAKE_CXXFLAGS_RELEASE += DOCTEST_CONFIG_DISABLE

CONFIG += sanitizer sanitize_address


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    IntroductionScreen.qml \
    RoundedImage.qml \
    qml/qmldir \
    qtquickcontrols2.conf

HEADERS += \
    categorymodel.h \
    metricmodel.h \
    scenariomodel.h \
    simresultmodel.h \
    stat_math.h \
    structmodel.h
