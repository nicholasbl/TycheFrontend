import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Image {
    id: img
    property int radius: 0

    Text {
        anchors.fill: parent
        text: "\uf059"
        font.family: loader.font.family
        font.pointSize: 92
        fontSizeMode: Text.Fit
        visible: img.status == Image.Error || img.status == Image.Null
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
    }

    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Item {
            width: img.width
            height: img.height
            Rectangle {
                anchors.centerIn: parent
                width: img.width
                height: img.height
                radius: img.radius
            }
        }
    }
}
