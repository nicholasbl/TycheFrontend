import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material

import "Global"
import "./utility.js" as Util

Item {
    SelectableRectangle {
        anchors.fill: parent
        anchors.margins: 1
        anchors.bottomMargin: 0
        upper_left: 10
        upper_right: 10

        border_color: Material.dividerColor
        border_width: 1

        fill_color: Constants.get_tinted_color(index, Material.background, .75, .50)

        RoundedImage {
            anchors.fill: parent
            anchors.margins: 5
            asynchronous: true
            source: image
            autoTransform: true
            fillMode: Image.PreserveAspectCrop
            radius: 10

            opacity: .25

            BusyIndicator {
                anchors.fill: parent
                running: parent.status === Image.Loading
            }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 3
            spacing: 3

            Label {
                id: column_name

                text: name

                font.bold: true

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }
        }


    }
}
