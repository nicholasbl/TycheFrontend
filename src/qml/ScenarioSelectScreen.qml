import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

FlowScreen {
    id: root
    header_text: "Select a design scenario to explore"
    show_footer_back: false

    GridView {
        id: main_grid
        anchors.fill: parent
        anchors.margins: 10
        cellWidth: 450
        cellHeight: 150

        model: scenario_model

        delegate: ScenarioDelegate {
            width: 445
            height: 145

            name: model.name
            image_source: model.image
            description_text: model.description

            onSelected: {
                console.log("Selected scenario", index)
                GridView.view.currentIndex = index
                scenario_model.current_scenario = index
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
                onClicked:  {
                    root.proceed_to_next()
                }
            }
        }

        Component.onCompleted: {
            scenario_model.current_scenario_changed.connect(function (){
                main_grid.currentIndex = scenario_model.current_scenario
            })
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: main_grid.count === 0
        }
    }

    Popup {
        id: request_fail_pop

        anchors.centerIn: Overlay.overlay

        ColumnLayout {
            Label {
                id: failure_reason
                text: "Unknown error"
                wrapMode: Label.WrapAtWordBoundaryOrAnywhere
            }
            Button {
                text: "Retry..."
                highlighted: true
                flat: true
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    scenario_model.refresh_scenario_list();
                    request_fail_pop.close()
                }
            }
        }


    }

    Component.onCompleted: {
        scenario_model.scenario_fetch_failure.connect(function(err_str){
            failure_reason.text = "An error occurred while attempting to fetch the scenario list. The reason given was: " + err_str
            request_fail_pop.open()
        })
    }
}
