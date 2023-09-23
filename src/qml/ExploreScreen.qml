import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

FlowScreen {
    id: root

    header_text: "Explore Scenario"

    show_footer_continue: false

    custom_bar: ToolButton {
        text: "Share"
        Layout.fillHeight: true
    }

    TableView {
        id: grid_view
        anchors.fill: parent

        //columnWidthProvider: function (column) { return 200 }
        //rowHeightProvider: function (row) { return height / 6 }

        model: DebugCellModel {}

        delegate: CellDelegate {
            implicitWidth: Math.min(Math.max(240, grid_view.width / 3), 300)
            implicitHeight: Math.min(Math.max(92, grid_view.height / 3), grid_view.width * 0.38)
            //width: Math.min(Math.max(240, grid_view.width / 3), 300)
            //height: Math.min(Math.max(92, grid_view.height / 3), grid_view.width * 0.38)
            //width: 240
            //height: 92
        }

    }

}
