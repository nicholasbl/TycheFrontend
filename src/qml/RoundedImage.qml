import QtQuick
import Qt5Compat.GraphicalEffects

Image {
    id: img
    property int radius: 0

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
