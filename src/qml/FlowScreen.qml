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

    property var extra: Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

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
            highlighted: true
            flat: true

            text: "Back"
            onClicked: swipeView.currentIndex -= 1
        }

        Label {
            text: root.header_text
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            anchors.left: back_button.right
            anchors.right: continue_button.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            font.bold: true
            font.pointSize: 16
        }

        Button {
            id: continue_button
            visible: root.show_footer_continue
            text: "Continue"
            highlighted: true
            flat: true
            onClicked: proceed_to_next()

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
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

