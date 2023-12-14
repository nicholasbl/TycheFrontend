import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import "Global"
import "./utility.js" as Util

TransparentPane {
    id: side_bar
    background_opacity: .75

    property alias edit_mode: edit_stack.currentIndex

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Label {
                text: "Current Run"
                Layout.fillWidth: true
            }
            EditLabel {
                text: "\uf1da"
                font: loader.font

                editable: true

                onClicked: {
                    archive_explore_pop.open()
                }

                ArchiveExplorePopup {
                    id: archive_explore_pop
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Layout.preferredHeight: 74
            Layout.maximumHeight: 74

            EditLabel {
                Layout.fillHeight: true
                text: "\uf053"
                font.family: loader.font.family
                font.pointSize: 18
                editable: archive_view.count
                verticalAlignment: Qt.AlignVCenter
                onClicked: {
                    if (archive_view.currentIndex > 0){
                        archive_view.currentIndex = archive_view.currentIndex - 1
                    }
                }
            }

            ListView {
                id: archive_view
                Layout.fillWidth: true
                Layout.preferredHeight: 74
                Layout.maximumHeight: 74
                orientation: ListView.Horizontal
                highlightFollowsCurrentItem: true
                spacing: 5

                model: archive_model

                clip: true

                delegate: ArchiveDelegate {
                    id: del
                    width: ListView.view.width
                    height: 72

                    onClicked: {
                        archive_view.currentIndex = index
                    }

                    state: del.ListView.isCurrentItem ? "selected" : ""
                }

                Label {
                    anchors.fill: parent
                    horizontalAlignment: Label.AlignHCenter
                    verticalAlignment: Label.AlignVCenter
                    text: "No results"
                    opacity: archive_view.count === 0
                }

                onCurrentIndexChanged: {
                    archive_model.select_run(currentIndex)
                }

                Component.onCompleted: {
                    archive_model.new_run_ready.connect(function(){
                        archive_view.currentIndex = archive_view.count - 1
                    })
                }

            }

            EditLabel {
                Layout.fillHeight: true
                text: "\uf054"
                font.family: loader.font.family
                font.pointSize: 18
                editable: archive_view.count
                verticalAlignment: Qt.AlignVCenter
                onClicked: {
                    if (archive_view.currentIndex < archive_view.count - 1){
                        archive_view.currentIndex = archive_view.currentIndex + 1
                    }
                }
            }
        }

        TabBar {
            id: mode_bar
            Layout.fillWidth: true

            TabButton {
                text: "Simulate"
            }
            TabButton {
                text: "Optimize"
            }
        }

        FadingStack {
            id: edit_stack

            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: mode_bar.currentIndex

            ColumnLayout {
                    Label {
                        Layout.fillWidth: true
                        text: "Edit investments in technologies, then tap simulate to see the result of these new alloctions"
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                    }

                    Frame {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredHeight: 200
                        Material.roundedScale: Material.SmallScale
                        Row {
                            id: distro_bar_chart
                            anchors.fill: parent

                            spacing: 5

                            Repeater {
                                id: bar_repeater
                                model: selected_category_model

                                property int bar_width: Math.max(distro_bar_chart.width / bar_repeater.count - 5, 1)


                                delegate: Rectangle {
                                    width: bar_repeater.bar_width
                                    height: {
                                        var total = Math.max(
                                                selected_category_model.maximum_investment,
                                                1
                                            );

                                        Math.max(distro_bar_chart.height * investment / total, 1)
                                    }
                                    radius: 5

                                    color: Util.color_with_alpha(Material.color(Constants.all_colors[index]), .60)

                                    anchors.bottom: distro_bar_chart.bottom

                                    Label {
                                        text: name
                                        rotation: -90
                                        transformOrigin: Item.Left
                                        anchors.left: parent.left
                                        anchors.leftMargin: height
                                        anchors.bottom: parent.bottom
                                    }
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
                        text: "Simulate"
                        Layout.fillWidth: true
                        onClicked: sim_result_model.ask_run_scenario()
                    }
                }

            ColumnLayout {

                    property bool is_current_pane: edit_stack.currentIndex === 1

                    onIs_current_paneChanged: {
                        if (is_current_pane){
                            if (target_metric_combo.currentIndex < 0) {
                                console.log("No target metric selected, fixup")
                                target_metric_combo.currentIndex = 0
                            }
                        }
                    }

                    Label {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: "Select a metric to optimize, metric optimization goals, as well as portfolio optimization bounds. Then tap optimize to see allocations"
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                        elide: Label.ElideRight
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

                    RowLayout {
                        Layout.fillWidth: true
                        Label {
                            Layout.fillWidth: true
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
    }
}
