import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import "Global"
import "./utility.js" as Util

TransparentPane {
    id: side_bar
    background_opacity: .75

    property alias view_type: view_selection.currentIndex
    property alias edit_mode: edit_stack.currentIndex

    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: mode_bar
            Layout.fillWidth: true

            TabButton {
                text: "\uf002"
                font: loader.font
            }
            TabButton {
                text: "\ue522"
                font: loader.font
            }
            TabButton {
                text: "\uf013"
                font: loader.font
            }
        }

        FadingStack {
            id: edit_stack

            Layout.fillWidth: true
            currentIndex: mode_bar.currentIndex


            Item {
                ColumnLayout {
                    anchors.fill: parent
                    Label {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: "Edit investments in technologies, then tap simulate to see the result of these new alloctions"
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }
                    Rectangle {
                        Layout.preferredHeight: 2
                        Layout.fillWidth: true
                        color: Material.dividerColor
                    }
                    Button {
                        text: "Simulate"
                        Layout.fillWidth: true
                        onClicked: sim_result_model.ask_run_scenario()
                    }
                }
            }

            Item {
                ColumnLayout {
                    anchors.fill: parent

                    Label {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: "Select metric optimization goals, then tap optimize to see allocations"
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }

                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "Portfolio limit"
                        text: Util.format_money(sim_result_model.opt_portfolio_amount)

                        validator: IntValidator {
                            bottom: 0
                            top: sim_result_model.max_opt_portfolio_amount
                        }

                        onAccepted:  {
                            sim_result_model.opt_portfolio_amount = parseInt(text)
                        }
                    }

                    Slider {
                        Layout.fillWidth: true
                        from: 0
                        to: sim_result_model.max_opt_portfolio_amount

                        onMoved: {
                            sim_result_model.opt_portfolio_amount = value
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

            Item {
                GridLayout {
                    columns: 2
                    anchors.fill: parent

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
                        flat: true
                    }
                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
}
