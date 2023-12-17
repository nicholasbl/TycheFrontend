import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import "./utility.js" as Util

Dialog {
    id: root
    title: "Optimize"

    parent: Overlay.overlay
    anchors.centerIn: Overlay.overlay

    width: parent.width * .9
    height: parent.height * .9

    margins: 1

    ColumnLayout {
        anchors.fill: parent
        // Label {
        //     Layout.fillWidth: true
        //     text: "Select a metric to optimize, optimization goals, as well as portfolio optimization bounds. Then tap optimize to see allocations"
        //     wrapMode: Label.WrapAtWordBoundaryOrAnywhere
        //     elide: Label.ElideRight
        // }

        RowLayout {
            id: content_row

            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 10
            ColumnLayout {
                id: left_column

                Layout.preferredWidth: root.width / 2
                Layout.maximumWidth: root.width / 2
                Layout.fillHeight: true

                Label {
                    Layout.fillWidth: true
                    text: "Set portfolio optimization parameters below"
                    wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    elide: Label.ElideRight
                    font.bold: true
                }

                RowLayout {
                    spacing: 5

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

                            // delegate: ExploreColumnHeader {
                            //     width: 80
                            //     height: 48

                            //     round_all: true

                            //     enable_editing: false

                            //     MouseArea {
                            //         anchors.fill: parent
                            //         anchors.margins: 10

                            //         onClicked: {
                            //             target_metric_combo.popup.close()
                            //             target_metric_combo.
                            //             target_metric_combo.currentIndex = index
                            //         }
                            //     }
                            // }

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

                        Label {
                            Layout.alignment: Qt.AlignRight
                            text: "Portfolio"
                        }

                        RowLayout {
                            Layout.fillWidth: true

                            TextField {
                                id: portfolio_limit_text
                                Layout.fillWidth: true
                                //placeholderText: "Portfolio Investment Limit"
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
                        Layout.fillHeight: true
                        Layout.preferredWidth: 1
                        color: Material.dividerColor
                    }

                    SpiderPlot{
                        Layout.fillWidth: false
                        Layout.fillHeight: true
                        Layout.preferredWidth: 180
                        fill_color: Util.color_with_alpha(Material.accentColor, .5)

                        values: {
                            selected_category_model.opt_funds_used
                            sim_result_model.opt_portfolio_amount

                            let ret = []

                            for (let i = 0; i < selected_metric_model.rowCount(); i++) {
                                ret.push(selected_category_model.get_ui_data(i, "opt_limit"))
                            }

                            return ret
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    color: Material.dividerColor
                }

                Label {
                    Layout.fillWidth: true
                    text: "Choose individual category investments"
                    wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    elide: Label.ElideRight
                    font.bold: true
                }

                ListView {
                    id: investment_sliders

                    Layout.preferredHeight: 48*4
                    Layout.fillWidth: true

                    clip: true

                    property real funds_left : sim_result_model.opt_portfolio_amount - selected_category_model.opt_funds_used

                    model: selected_category_model

                    delegate: Item {
                        id: delegate_root
                        width: ListView.view.width
                        height: invest_limit.implicitHeight

                        RowLayout {
                            anchors.fill: parent
                            Label {
                                Layout.preferredWidth: delegate_root.width / 4
                                text: name
                            }

                            Slider {
                                id: invest_limit
                                Layout.fillWidth: true
                                from: 0
                                to: opt_limit + investment_sliders.funds_left

                                value: opt_limit

                                onMoved: {
                                    opt_limit = value
                                }
                            }

                            Label {
                                Layout.preferredWidth: 64
                                text: Util.format_money(opt_limit)
                                font.bold: true
                            }
                        }
                    }
                }


                Rectangle {
                    Layout.preferredHeight: 1
                    Layout.fillWidth: true
                    color: Material.dividerColor
                }
                Button {
                    text: "Optimize"
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignRight
                    onClicked: sim_result_model.ask_run_optimize()
                }
            }

            Frame {
                Layout.fillHeight: true
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent

                    ListView {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        model: opt_archive_model.cat_result()
                        delegate: ItemDelegate {
                            text: name + ": " + Util.format_money(value)
                        }
                    }

                    ListView {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        model: opt_archive_model.met_result()
                        delegate: ItemDelegate {
                            text: name + ": " + value
                        }
                    }
                }


            }
        }
    }

    onOpened: {
        if (sim_result_model.opt_portfolio_amount < 1) {
            let new_val = sim_result_model.max_opt_portfolio_amount / 2;
            if (new_val > 1){
                sim_result_model.opt_portfolio_amount = new_val
                //portfolio_amount_slider.value = new_val
            }
        }

        if (target_metric_combo.currentIndex < 0) {
            console.log("No target metric selected, fixup")
            target_metric_combo.currentIndex = 0
        }
    }
}
