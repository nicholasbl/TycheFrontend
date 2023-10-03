import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import "./utility.js" as Util

TransparentRectangle {
    id: root
    width: 72
    height: 72

    signal clicked

    signal askDelete

    property bool large_view: width > 80

    property string name_from_model: name

    property bool show_delete: false

    function user_updated_name(string) {
        name = string
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Label {
            Layout.fillWidth: true
            text: name
            wrapMode: Label.WrapAtWordBoundaryOrAnywhere
            font.bold: true
        }

        Rectangle {
            color: Material.dividerColor
            Layout.preferredHeight: 1
            Layout.fillWidth: true
            visible: large_view
        }

        Label {
            Layout.fillWidth: true
            text: type
            elide: Label.ElideMiddle
            visible: large_view
        }

        Label {
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: time_date
            color: Material.iconDisabledColor
            elide: Label.ElideMiddle
            font.pointSize: 8
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.clicked()
        }
    }

    EditLabel {
        anchors.fill: parent
        text: "\uf2ed"
        font: loader.font

        minimumPointSize: 24

        horizontalAlignment: EditLabel.AlignHCenter
        verticalAlignment: EditLabel.AlignVCenter

        fontSizeMode: EditLabel.Fit

        color: Material.color(Material.Red)

        onClicked: {
            root.askDelete()
        }

        Component.onCompleted: {
            font.pointSize = 48
        }

        visible: show_delete
    }

    states: [
        State {
            name: "selected"
            PropertyChanges {
                target:  root
                background_color: Qt.tint(Material.backgroundColor,
                                          Util.color_with_alpha(Material.textSelectionColor, .40))
            }
        }
    ]
    transitions: Transition {
        ColorAnimation {
            properties: "background_color"
            duration: 250
        }
    }
}
