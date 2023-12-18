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
                Label {
                    text: "Remaining funds:"
                    font.bold: true
                }
                Label {
                    text: Util.format_money(investment_sliders.funds_left)
                    font.bold: true
                    font.pointSize: 18
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

                property real funds_left : sim_result_model.opt_portfolio_amount - selected_category_model.opt_funds_used

                model: selected_category_model

                spacing: 5

                delegate: TransparentRectangle {
                    id: delegate_root
                    width: ListView.view.width
                    height: delegate_layout.implicitHeight + 10

                    radius: 10
                    background_color:  Constants.get_tinted_color(index, Material.background, .75, .50)

                    RowLayout {
                        id: delegate_layout
                        anchors.fill: parent
                        anchors.margins: 5
                        Label {
                            Layout.preferredWidth: delegate_root.width / 4
                            text: name
                            font.bold: true
                        }

                        Slider {
                            id: invest_limit
                            Layout.fillWidth: true
                            from: 0
                            to: opt_limit + investment_sliders.funds_left

                            value: opt_limit

                            enabled: sim_result_model.opt_portfolio_amount > 0

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
}
