import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Item {
    id: root
    property alias source: img.source

    property alias autoTransform: img.autoTransform
    property alias asynchronous: img.asynchronous
    property alias cache: img.cache
    property alias fillMode: img.fillMode
    property alias mipmap: img.mipmap
    property alias progress: img.progress
    property alias status: img.status
    property int radius: 0

    Image {
        id: img
        anchors.fill: parent
        layer.enabled: true
        visible: false
    }

    Rectangle {
        id: mask
        anchors.fill: parent
        radius: root.radius
        layer.enabled: true
    }

    MultiEffect {
        source: img
        anchors.fill: parent
        maskEnabled: true
        maskSource: mask
    }

}


