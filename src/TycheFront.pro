QT += quick charts widgets

CONFIG += c++20

SOURCES += \
        categorymodel.cpp \
        main.cpp \
        metricmodel.cpp \
        simresultmodel.cpp \
        structmodel.cpp

RESOURCES += \
    res.qrc

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
    qtquickcontrols2.conf

HEADERS += \
    categorymodel.h \
    metricmodel.h \
    simresultmodel.h \
    structmodel.h
