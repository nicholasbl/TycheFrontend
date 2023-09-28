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

    property alias custom_bar: custom_buttons.data

    property var extra: Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    header: TransparentRectangle {
        id: header_bar
        implicitHeight: 48
        radius: 0
        Material.elevation: 0
        RowLayout {
            id: header_inset
            anchors.fill: parent
            anchors.margins: 10

            Label {
                text: root.header_text
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.bold: true
                font.pointSize: 16
            }

            RowLayout {
                id: custom_buttons
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

        }

        layer.enabled: header_bar.Material.elevation > 0
        layer.effect: ElevationEffect {
            elevation: header_bar.Material.elevation
            fullWidth: true
        }
    }

    background: Item {}

    footer: TransparentRectangle {
        id: footer
        implicitHeight: 48
        radius: 0

        RowLayout {
            anchors.fill: parent

            Button {
                id: back_button
                visible: root.show_footer_back
                Layout.preferredWidth: continue_button.width
                text: "Back"
                onClicked: swipeView.currentIndex -= 1
                Layout.rightMargin: 5
                Layout.leftMargin: 5
                highlighted: true
                flat: true
            }

            Label {
                id: footer_text
                wrapMode: Text.WordWrap
                Layout.fillHeight: true
                Layout.fillWidth: true
                elide: Label.ElideRight
            }

            Button {
                id: continue_button
                visible: root.show_footer_continue
                text: "Continue"
                highlighted: true
                flat: true
                onClicked: proceed_to_next()
                Layout.rightMargin: 5
                Layout.leftMargin: 5
            }
        }

    }

//    Item {
//        id: content_item
////        anchors.left: parent.left
////        anchors.right: parent.right
////        anchors.top: header.bottom
////        anchors.bottom: footer.top
//    }

    function proceed_to_next() {
        root.before_page_forward()
        swipeView.currentIndex += 1
    }
}

