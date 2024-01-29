import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import "Global"
import "./utility.js" as Util

FlowScreen {
    id: root

    header_text: "Explore Scenario: " + sim_result_model.current_scenario_name

    show_footer_continue: false

    onTool_button_clicked: settings_pop.open()
    enable_tool_button: true

    help_text: "Explore the scenario using two methods: manual investment and optimized investment. In manual 'Simulate' mode, you can change sliders on investments and run simulations to see the results. In 'Optimize' mode, you can set some boundaries on metrics and let the system determine optimal investments for you."

    SwipeView {
        anchors.fill: parent

        currentIndex: view_selector.currentIndex

        SimulateView {
            id: explore_root_view

        }

        OptimizationView {
            id: optimization_root_view
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

    SettingsPopup {
        id: settings_pop
        anchors.centerIn: Overlay.overlay
        margins: 1
    }

    Component.onCompleted: {
        archive_model.sim_system_failure.connect(function(err_str){
            failure_reason.text = "An error occurred while executing the request. The reason given was: <b>" + err_str + "</b>. You can try again; if the issue persists, please contact the relevant administrators."
            sim_fail_pop.open()
        })

        archive_model.sim_exception.connect(function(err_str){
            failure_reason.text = "Unable to complete request: <b>" + err_str + "</b>."
            sim_fail_pop.open()
        })

        opt_archive_model.sim_system_failure.connect(function(err_str){
            failure_reason.text = "An error occurred while executing the request. The reason given was: <b>" + err_str + "</b>. You can try again; if the issue persists, please contact the relevant administrators."
            sim_fail_pop.open()
        })

        opt_archive_model.sim_exception.connect(function(err_str){
            failure_reason.text = "Unable to complete request: <b>" + err_str + "</b>."
            sim_fail_pop.open()
        })
    }

    footer: TransparentRectangle {
        id: footer_bar
        implicitHeight: 48
        radius: 0
        Material.elevation: 0

        TabBar {
            id: view_selector
            TabButton {
                text: "Simulate"
                width: implicitWidth
            }
            TabButton {
                text: "Optimize"
                width: implicitWidth
            }

            anchors.centerIn: parent
        }

    }

    // footer: Item {
    //     id: footer_bar
    //     implicitHeight: 48

    //     TabBar {
    //         TabButton {
    //             text: "Simulate"
    //         }
    //         TabButton {
    //             text: "Optimize"
    //         }
    //     }
    // }

}
