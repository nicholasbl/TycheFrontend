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

                Row {
                    anchors.fill: parent
                    Item {
                        id: sum_spacer
                        width: grid_view.column_width
                        height: grid_view.row_height
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 3
                            Label {
                                text: "Total Portfolio"
                            }

                            Label {
                                font.pointSize: 20
                                text: Util.format_money(sim_result_model.total_value)
                                Layout.alignment: Qt.AlignRight
                            }
                        }
                    }

                    Rectangle {
                        width: 1
                        height: sum_footer.height
                        color: Material.dividerColor
                    }

                    Repeater {
                        model: sim_result_sum_model
                        delegate: CellDelegate {
                            height: grid_view.row_height
                            width: grid_view.column_width
                            view_index: view_selection.currentIndex
                            fill_color: Util.color_with_alpha(Material.color(Constants.all_colors[index]), .60)
                            border.width: 0
                        }
                    }
                }


            }

        }

        Pane {
            id: side_bar
            Layout.fillHeight: true
            Layout.preferredWidth: 180

            Material.roundedScale: Material.MediumScale
            Material.elevation: 5

            Component.onCompleted: {
                background.opacity = .5
            }

            ColumnLayout {
                anchors.fill: parent
                RoundButton {
                    checkable: true
                    flat: true
                    text: "\uf044"
                    font: loader.font

                    onClicked: {
                        checked =! checked
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
