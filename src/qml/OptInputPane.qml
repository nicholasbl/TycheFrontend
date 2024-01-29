import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import "./utility.js" as Util
import "Global"

TransparentPane {
    id: root
    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                Layout.fillWidth: true
                Layout.columnSpan: 2

                TextField {
                    id: portfolio_limit_text
                    Layout.fillWidth: true
                    placeholderText: "Individual Investment Limit"
                    text: Util.format_money(sim_result_model.opt_portfolio_amount)

                    font.pointSize: 16

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
                            var last = ''
                            if (matches[2]) {
                                last = matches[2].toLowerCase()
                            }


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

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                Layout.margins: 5
                color: Material.dividerColor
            }

            ListView {
                id: investment_sliders

                Layout.fillWidth: true
                Layout.fillHeight: true

                clip: true

                model: selected_category_model

                spacing: 5

                delegate: TransparentRectangle {
                    id: delegate_root
                    width: ListView.view.width
                    height: delegate_layout.implicitHeight + 10

                    radius: 10

                    property bool is_enabled : opt_limit >= 0

                    background_opacity: .5

                    background_color: {
                        var tinted = Constants.get_tinted_color(index, Material.background, .75, 1)
                        if (is_enabled){
                            return tinted
                        } else {
                            return Qt.tint(
                                        tinted,
                                        Util.color_with_alpha(
                                            Material.backgroundDimColor, .75
                                            )
                                        )
                        }
                    }

                    RowLayout {
                        id: delegate_layout
                        anchors.fill: parent
                        anchors.margins: 5

                        CheckBox {
                            checked: is_enabled

                            enabled: sim_result_model.opt_portfolio_amount > 0

                            onClicked: {
                                if (checked) {
                                    //check
                                    opt_limit = 0
                                } else {
                                    // uncheck
                                    opt_limit = -1
                                }
                            }
                        }

                        Label {
                            Layout.preferredWidth: delegate_root.width / 4
                            text: name
                            font.bold: true
                        }

                        Slider {
                            id: invest_limit
                            Layout.fillWidth: true
                            from: 0
                            to: sim_result_model.opt_portfolio_amount

                            value: opt_limit

                            enabled: sim_result_model.opt_portfolio_amount > 0 && is_enabled

                            onMoved: {
                                opt_limit = value
                            }
                        }

                        Label {
                            Layout.preferredWidth: 64
                            text: Util.format_money(opt_limit < 0 ? 0 : opt_limit)
                            font.bold: true
                            enabled: is_enabled
                        }
                    }
                }
            }
        }

        SpiderPlot{
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredWidth: 180
            fill_color: Util.color_with_alpha(Material.accentColor, .5)

            function rebuild() {
                //sim_result_model.opt_portfolio_amount

                model.clear()

                for (let i = 0; i < selected_category_model.rowCount(); i++) {
                    var v = {
                        "label": selected_category_model.get_ui_data(i, "name"),
                        "value": selected_category_model.get_ui_data(i, "opt_limit")
                    }

                    //console.log("HERE:", v.label, v.value)

                    model.append(v)
                }

                return true
            }

            Component.onCompleted: {
                selected_category_model.content_changed.connect(function(){
                    rebuild()
                })
            }

        }
    }
}
