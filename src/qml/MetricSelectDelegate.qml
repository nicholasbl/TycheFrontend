import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

TransparentRectangle {
    id: root

    property string metric_name : "Debug Name"
    property string metric_description : "A debug description"
    property string metric_image

    background_opacity: 0

    RoundedImage {
        id: metric_image_place
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 3
        asynchronous: true
        source: metric_image
        autoTransform: true
        fillMode: Image.PreserveAspectCrop
        width: height
        radius: 15

        BusyIndicator {
            anchors.fill: parent
            running: parent.status === Image.Loading
        }
    }

    Label {
        id: metric_name_label
        text: metric_name
        anchors.left: metric_image_place.right
        anchors.top: parent.top
        anchors.right: selection_switch.left
        anchors.margins: 4
        anchors.topMargin: 5
        font.bold: true
    }



    Label {
        id: metric_text
        text: metric_description
        anchors.left: metric_image_place.right
        anchors.top: metric_name_label.bottom
        anchors.right: selection_switch.left
        anchors.bottom: parent.bottom
        wrapMode: Text.WordWrap
        anchors.margins: 4
    }

    CheckBox {
        id: selection_switch
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }
    states: [
        State {
            name: "checked"
            when: selection_switch.checked

            PropertyChanges {
                target: root
                background_color: Material.accent
                background_opacity: .25
            }
        }
    ]

    Rectangle {
        width: metric_name_label.width + selection_switch.width
        anchors.right: parent.right
        height: 1
        color: Material.dividerColor
        anchors.top: parent.bottom
        anchors.topMargin: 1
    }
}
