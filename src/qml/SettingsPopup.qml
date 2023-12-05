import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Controls.Material
import QtQuick.Layouts

import "./utility.js" as Util

Dialog {
    title: "Settings"

    property alias view_type: view_selection.currentIndex

    GridLayout {
        columns: 2

        Label {
            text: "Display"
            elide: Label.ElideRight
        }
        ComboBox {
            Layout.fillWidth: true
            id: view_selection

            model: ["Boxes", "Violin"]
        }
        Rectangle {
            Layout.preferredHeight: 2
            Layout.columnSpan: 2
            Layout.fillWidth: true
            color: Material.dividerColor
        }
        Button {
            Layout.columnSpan: 2
            text: "Export"
            Layout.fillWidth: true

            onClicked: export_pop.open()

            ExportPopup {
                id: export_pop
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }

    standardButtons: Dialog.Close
}
