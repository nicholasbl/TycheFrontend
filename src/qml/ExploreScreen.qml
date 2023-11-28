import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import "Global"
import "./utility.js" as Util

FlowScreen {
    id: root

    header_text: "Explore Scenario: " + sim_result_model.current_scenario_name

    show_footer_continue: false

    property bool in_edit_mode: side_bar.edit_mode === 0
    property bool in_optim_mode: side_bar.edit_mode === 1


    RowLayout {
        id: explore_root_view
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

                property real column_width: Math.min((grid_view.width) / (columns + 1), 300)
                property real row_height: column_width / 3

                leftMargin: column_width
                topMargin: row_height

                //onContentXChanged: {
                //    console.log("CX", contentX)
                //}

                property bool layout_recompute : {
                    width
                    height
                    forceLayout()
                    console.log("Recomputing layout")
                    return true
                }


                delegate: CellDelegate {
                    implicitHeight: grid_view.row_height
                    implicitWidth: grid_view.column_width
                    view_index: side_bar.view_type
                    fill_color: Util.color_with_alpha(Material.color(Constants.all_colors[column]), .60)

                    data_min_bound: sim_result_model.metric_summary[column*2]
                    data_max_bound: sim_result_model.metric_summary[column*2 + 1]
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

                            enable_editing: root.in_optim_mode
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
                                font.bold: true
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
                                view_index: side_bar.view_type
                                fill_color: Util.color_with_alpha(Material.color(Constants.all_colors[index]), .60)
                                border.width: 0

                                data_min_bound: sim_result_model.metric_summary[index*2]
                                data_max_bound: sim_result_model.metric_summary[index*2 + 1]

                                //data_min_bound: sim_result_model.all_cell_stats[0]
                                //data_max_bound: sim_result_model.all_cell_stats[1]

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

        ExploreSideBar {
            id: side_bar
            Layout.fillHeight: true
            Layout.margins: 10
            Layout.preferredWidth: 200
        }

    }

    Dialog {
        id: sim_fail_pop

        anchors.centerIn: Overlay.overlay

        title: "Error"

        standardButtons: Dialog.Close

        RowLayout {
            Label {
                text: "\uf071"
                font: loader.font

                color: Material.color(Material.Red)

                Component.onCompleted: {
                    font.pointSize = 24
                }
            }

            Label {
                Layout.maximumWidth: 250
                id: failure_reason
                text: "Unknown error"
                wrapMode: Label.WrapAtWordBoundaryOrAnywhere
            }
        }


    }

    Component.onCompleted: {
        archive_model.error_from_sim.connect(function(err_str){
            failure_reason.text = "An error occurred while executing the requested simulation. The reason given was: <b>" + err_str + "</b>. You can try again; if the issue persists, please contact the relevant administrators."
            sim_fail_pop.open()
        })
    }

}
