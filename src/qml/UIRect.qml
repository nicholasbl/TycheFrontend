import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property real shadow_opacity: .5

    property alias radius: trect.radius

    default property alias children: trect.data

    property alias background_color: trect.background_color
    property alias background_opacity: trect.background_opacity
    property alias use_border: trect.use_border

    BorderImage {
        id: shadow_glow
        source: "qrc:/images/shadow_r10.webp"
        width: 100; height: 100
        border.left: 20; border.top: 20
        border.right: 20; border.bottom: 20
        opacity: parent.shadow_opacity
        anchors.fill: parent
    }

    TransparentRectangle {
        id: trect
        anchors.fill: parent
        anchors.margins: 10
        radius: 10
    }
}



