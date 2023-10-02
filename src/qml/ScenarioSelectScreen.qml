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

        modal: true

        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                Label {
                    text: "\uf071"
                    font: loader.font

                    color: Material.color(Material.Red)

                    Component.onCompleted: {
                        font.pointSize = 48
                    }
                }

                Label {
                    Layout.maximumWidth: 250
                    id: failure_reason
                    text: "Unknown error"
                    wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                }
            }

            RowLayout{
                Layout.alignment: Qt.AlignRight
                Button {
                    text: "Close"
                    highlighted: true
                    flat: true

                    onClicked: {
                        request_fail_pop.close()
                    }
                }

                Button {
                    text: "Retry..."
                    highlighted: true
                    flat: true
                    onClicked: {
                        scenario_model.refresh_scenario_list();
                        request_fail_pop.close()
                    }
                }

            }

        }


    }

    Component.onCompleted: {
        scenario_model.scenario_fetch_failure.connect(function(err_str){
            failure_reason.text = "An error occurred while attempting to fetch the scenario list. The reason given was: <b>" + err_str + "</b>. You can retry the fetch; if the issue persists, please contact the relevant administrators."
            request_fail_pop.open()
        })
    }
}
