import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Material.impl

Page {
    id: root

    property string header_text: "Header Text"
    property bool show_footer_back: true
    property bool show_footer_continue: true

    property var before_page_forward: function() {
        console.log("Default page advance")
    }

    property bool enable_tool_button: false
    signal tool_button_clicked()

    property string help_text: ""

    header: TransparentRectangle {
        id: header_bar
        implicitHeight: 48
        radius: 0
        Material.elevation: 0

        Button {
            id: back_button
            visible: root.show_footer_back
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 2

            flat: true

            text: loader.icon_string("\uf053") + " Back"
            onClicked: swipeView.currentIndex -= 1

            contentItem: Label {
                text: parent.text
                textFormat: Label.RichText
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Label {
            id: header_text_label
            text: root.header_text
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            font.bold: true
            font.pointSize: 16
        }
        RoundButton {
            anchors.left: header_text_label.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            flat: true
            text: "\uf059"
            font: loader.font

            //visible: help_text.length

            onClicked: {
                help_tip.open()
            }

            Popup {
                id: help_tip
                margins: 1

                width: 300

                Label {
                    anchors.fill: parent
                    text: help_text
                    wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                }

                onOpened: {
                    close_timer.start()
                }

                Timer {
                    id: close_timer
                    interval: 10000

                    onTriggered: help_tip.close()
                }
            }
        }

        Button {
            id: continue_button
            visible: root.show_footer_continue
            text: "Continue " + loader.icon_string("\uf054")
            onClicked: proceed_to_next()

            flat: true

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 2

            contentItem: Label {
                text: parent.text
                textFormat: Label.RichText
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        ToolButton {
            visible: enable_tool_button
            text: "\uf013"
            font: loader.font
            onClicked: root.tool_button_clicked()
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 2
        }

        layer.enabled: header_bar.Material.elevation > 0
        layer.effect: ElevationEffect {
            elevation: header_bar.Material.elevation
            fullWidth: true
        }
    }

    background: Item {}

    function proceed_to_next() {
        root.before_page_forward()
        swipeView.currentIndex += 1
    }
}

