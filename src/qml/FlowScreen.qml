import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Item {
    id: root

    property string header_text: "Header Text"
    property bool show_footer_back: true
    property bool show_footer_continue: true

    default property alias children: content_item.data
    property alias custom_bar: custom_buttons.data

    property var extra: Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    TransparentRectangle {
        id: header
        height: Math.max(header_inset.implicitHeight, 48)
        radius: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

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
    }

    TransparentRectangle {
        id: footer
        radius: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0

        height: back_button.implicitHeight

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
                onClicked: swipeView.currentIndex += 1
                Layout.rightMargin: 5
                Layout.leftMargin: 5
            }
        }

    }

    Item {
        id: content_item
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: footer.top
    }
}

