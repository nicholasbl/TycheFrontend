import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

// A label you can click on and launch an action

Label {
    id: root
    property bool editable: false

    color: editable
           ?
               (mouse_area.containsMouse ? Material.primaryHighlightedTextColor : Material.accentColor)
           : Material.foreground

    signal clicked

    MouseArea {
        id: mouse_area
        enabled: editable
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            root.clicked()
        }
    }
}
