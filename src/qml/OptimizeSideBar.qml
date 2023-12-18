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

            RowLayout {
                Layout.fillWidth: true
                Layout.columnSpan: 2

                TextField {
                    id: portfolio_limit_text
                    Layout.fillWidth: true
                    placeholderText: "Portfolio Investment Limit"
                    text: Util.format_money(sim_result_model.opt_portfolio_amount)

                    onAccepted:  {
                        if (text.length === 0) {
                            return
                        }

                        var regexp = /\$?(\d+\.?\d*)\s*([kmb])?/i

                        var matches = regexp.exec(text)

                        console.log(text, matches)

                        // this finds the first int it can get
                        var val = parseFloat(matches[1])

                        // pull out any mulipliers...
                        if (matches.length > 2){
                            var last = matches[2].toLowerCase()

                            if (last === 'k') {
                                val *= 1E3
                            } else if (last === 'm') {
                                val *= 1E6
                            } else if (last === 'b') {
                                val *= 1E9
                            }
                        }

                        sim_result_model.opt_portfolio_amount = val
                    }
                }

                RoundButton {
                    text: "\uf522"
                    font: loader.font
                    flat: true

                    onClicked: {
                        // pick a nice random value between the max opt amount, and something larger than zero
                        sim_result_model.opt_portfolio_amount = Math.random() * sim_result_model.max_opt_portfolio_amount + 10000
                    }
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
