import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import "Global"
import "./utility.js" as Util

TransparentPane {
    id: side_bar
    background_opacity: .75

    ColumnLayout {
        anchors.fill: parent

        Label {
            Layout.fillWidth: true
            text: "Select optimization goals, then tap optimize to see allocations"
            wrapMode: Label.WrapAtWordBoundaryOrAnywhere
            elide: Label.ElideRight
        }

        Item {
            Layout.fillHeight: true
        }

        GridLayout {
            columns: 2

            Label {
                Layout.alignment: Qt.AlignRight
                text: "Metric"
            }

            ComboBox {
                id: target_metric_combo
                Layout.fillWidth: true
                model: selected_metric_model
                textRole: "name"
                valueRole: "metric_id"

                Component.onCompleted: {
                    if (count === 0) {
                        currentIndex = -1
                        return
                    }

                    var idx = indexOfValue(sim_result_model.optimize_target_metric_id)

                    if (idx < 0) id = 0

                    currentIndex = idx
                }

                onActivated: {
                    sim_result_model.optimize_target_metric_id = currentValue;
                }
            }

            Label {
                Layout.alignment: Qt.AlignRight
                text: "Sense"
            }
            ComboBox {
                id: min_max_combo
                Layout.fillWidth: true
                textRole: "text"
                valueRole: "value"
                model: [
                    { value: "min", text: "Minimum"},
                    { value: "max", text: "Maximum"}
                ]

                onActivated: sim_result_model.optimize_target_sense = currentValue

                Component.onCompleted: {
                    if (count === 0) {
                        currentIndex = -1
                        return
                    }

                    var idx = indexOfValue(sim_result_model.optimize_target_sense)

                    if (idx < 0) id = 0

                    currentIndex = idx
                }

            }
        }

        Rectangle {
            Layout.preferredHeight: 2
            Layout.fillWidth: true
            color: Material.dividerColor
        }
        Button {
            text: "Optimize"
            Layout.fillWidth: true
            onClicked: sim_result_model.ask_run_optimize()
        }
    }
}
