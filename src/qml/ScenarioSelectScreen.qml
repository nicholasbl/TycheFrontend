import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

FlowScreen {
    id: item1
    header_text: "Select a design scenario to explore"
    show_footer_back: false

    GridView {
        id: main_grid
        anchors.fill: parent
        anchors.margins: 10
        cellWidth: 450
        cellHeight: 150

        model: DebugScenarioModel {}

        delegate: ScenarioDelegate {
            width: 450
            height: 150

            name: model.name
            image_source: image
            description_text: model.description

            onSelected: {
                console.log("Selected scenario", index)
                GridView.view.currentIndex = index
            }

            state: GridView.view.currentIndex === index ? "selected" : ""
        }

        highlight: Item {
            id: item2
            width: parent.cellWidth
            height: parent.cellHeight
            z: 10

            RoundButton {
                text: "\uf054"
                font: loader.font
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 10
                highlighted: true
            }
        }
    }

}
