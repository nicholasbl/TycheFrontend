import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Material

import "./utility.js" as Util

TransparentPane {
    id: root

    property alias name: label.text
    property string image_source: "https://placehold.co/400/blue/white.png"
    property string description_text: "This is a long line of placeholder text that we can use to measure stuff"

    signal selected

    Material.elevation: (click_area.containsMouse) ? 5 : 2

    padding: 0

    Behavior on Material.elevation {
        NumberAnimation {
            duration: 250
        }
    }

    RoundedImage {
        id: back_image
        radius: 10
        asynchronous: true
        fillMode: Image.PreserveAspectCrop
        source: image_source
        anchors.fill: parent
        opacity: status === Image.Ready ? .20 : 0
        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        }
    }

    RowLayout {
        id: core_layout
        anchors.fill: parent
        Item {
            id: image_container
            Layout.leftMargin: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 120
            Layout.preferredWidth: 120


            RoundedImage {
                id: image
                radius: 10
                source: image_source
                asynchronous: true
                mipmap: true
                fillMode: Image.PreserveAspectCrop
                anchors.fill: parent

                BusyIndicator {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    running: image.status === Image.Loading

                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5

            Label {
                id: label

                text: "Placeholder"
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pointSize: 18
                elide: Text.ElideRight
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 2
                Layout.rightMargin: 5
                color: Material.dividerColor
            }

            Label {
                id: description_zone

                Material.containerStyle: Material.Filled

                Layout.fillHeight: true
                Layout.fillWidth: true

                wrapMode: Text.WordWrap
                font.pointSize: 14
                text: description_text
            }
        }

    }

    MouseArea {
        id: click_area
        anchors.fill: parent
        hoverEnabled: true
        onClicked: selected()
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
