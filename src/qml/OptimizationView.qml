import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import "./utility.js" as Util

RowLayout {
    id: root
    ColumnLayout {
        Layout.margins: 10
        Layout.fillWidth: true
        Layout.fillHeight: true

        OptInputPane {
            Layout.fillWidth: true
            Layout.preferredHeight: root.height / 2 - down_arrow.implicitHeight / 2
            //Layout.verticalStretchFactor: 2
        }

        Label {
            id: down_arrow
            text: "\uf0d7"
            font.family: loader.font.family
            font.pointSize: 32
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 0
        }

        OptOutputPane {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 2
        }

    }

    OptimizeSideBar {
        id: side_bar
        Layout.fillHeight: true
        Layout.margins: 10
        Layout.preferredWidth: 250
    }
}

