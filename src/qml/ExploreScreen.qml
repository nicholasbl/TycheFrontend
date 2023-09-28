import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import "Global"
import "./utility.js" as Util

FlowScreen {
    id: root

    header_text: "Explore Scenario"

    show_footer_continue: false

    custom_bar: ToolButton {
        text: "\uf08e"
        font: loader.font
        Layout.fillHeight: true
    }

    property bool in_edit_mode: false


    RowLayout {
        id: root_view
        anchors.fill: parent

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5


            TableView {
                id: grid_view

                Layout.fillHeight: true
                Layout.fillWidth: true

                model: sim_result_model

                clip: true

                property real column_width: (grid_view.width) / (columns + 1)
                property real row_height: column_width / 3

                leftMargin: column_width
                topMargin: row_height

                property bool layout_recompute : {
                    width
                    height
                    forceLayout()
                    return true
                }


                delegate: CellDelegate {
                    implicitHeight: grid_view.row_height
                    implicitWidth: grid_view.column_width
                    view_index: view_selection.currentIndex
                    fill_color: Util.color_with_alpha(Material.color(Constants.all_colors[column]), .60)
                }

                Row {
                    id: col_header
                    y: grid_view.contentY
                    z: 2
                    Repeater {
                        model: selected_metric_model

                        delegate: ExploreColumnHeader {
                            width: grid_view.column_width
                            height: grid_view.row_height
                        }
                    }
                }

                Column {
                    id: row_header
                    x: grid_view.contentX
                    z: 2
                    Repeater {
                        model: selected_category_model

                        delegate : ExploreRowHeader {
                            height: grid_view.row_height
                            width: grid_view.column_width

                           in_edit_mode: root.in_edit_mode
                        }
                    }
                }

                ScrollIndicator.horizontal: ScrollIndicator { }
                ScrollIndicator.vertical: ScrollIndicator { }

            }

            TransparentRectangle {
                id: sum_footer
                Layout.fillWidth: true
                Layout.preferredHeight: grid_view.row_height
                background_opacity: .75

                RowLayout {
                    anchors.fill: parent
                    spacing: 0
                    Item {
                        id: sum_spacer
                        Layout.minimumWidth: grid_view.column_width
                        Layout.maximumWidth: grid_view.column_width
                        Layout.fillHeight: true
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            Label {
                                text: "Total Portfolio"
                            }

                            Label {
                                font.pointSize: 18
                                text: Util.format_money(sim_result_model.total_value)
                                Layout.alignment: Qt.AlignRight
                            }
                        }
                    }

                    Row {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Repeater {
                            anchors.fill: parent
                            model: sim_result_sum_model
                            delegate: CellDelegate {

                                height: grid_view.row_height
                                width: grid_view.column_width - 1
                                view_index: view_selection.currentIndex
                                fill_color: Util.color_with_alpha(Material.color(Constants.all_colors[index]), .60)
                                border.width: 0

                                Rectangle {
                                    width: 1
                                    height: parent.height
                                    anchors.left: parent.left
                                    color: Material.dividerColor
                                }

                            }
                        }
                    }


                }


            }

        }

        TransparentPane {
            id: side_bar
            Layout.fillHeight: true
            Layout.preferredWidth: 180
            Layout.margins: 10

            background_opacity: .75

            ColumnLayout {
                anchors.fill: parent
                StackLayout {
                    id: edit_stack

                    Layout.fillWidth: true

                    RoundButton {
                        flat: true
                        text: "\uf044"
                        font: loader.font

                        onClicked: {
                            edit_stack.currentIndex = 1
                            root.in_edit_mode = true
                        }
                    }

                    Item {
                        ColumnLayout {
                            anchors.fill: parent
                            Label {
                                Layout.fillWidth: true
                                text: "Edit investments in technologies, then tap simulate to see the result of these new alloctions"
                                wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                            }
                            Button {
                                text: "Simulate"
                                onClicked: sim_result_model.ask_run_scenario()
                            }
                        }
                    }

                }



                ComboBox {
                    id: view_selection
                    Layout.fillWidth: true
                    model: ["Boxes", "Violin"]
                }
            }
        }
    }

}
