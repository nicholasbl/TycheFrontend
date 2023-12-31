import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls 6.5
import QtQuick.Controls.Material

ApplicationWindow {
    width: 1152
    height: 648

    visible: true
    title: "Tyche Frontend"

    FontLoader {
        id: loader
        source: "qrc:/fonts/fa_solid_6.otf"
        Component.onCompleted: console.log("Loaded:", name, loader.font.family)

        function icon_string(icn) {
            return "<span style=\"font-family:Font Awesome 6 Free;\">" + icn + "</span>"
        }
    }

    Item {
        anchors.fill: parent
        Image {
            id: image
            anchors.fill: parent
            source: "qrc:/images/67859_blur_25.webp"
            fillMode: Image.PreserveAspectCrop
            visible: Material.theme == Material.Dark
        }
        Image {
            id: image_light
            anchors.fill: parent
            source: "qrc:/images/geothermal_bores_conus.webp"
            fillMode: Image.PreserveAspectCrop
            visible: Material.theme == Material.Light
        }
        Rectangle {
            anchors.fill: parent
            color: Material.theme == Material.Light ? "white" : "black"
            opacity: .5
        }
    }

    StackView {
        id: main_stack_view
        anchors.fill: parent

        initialItem: Introduction {
            id: intro_screen

            onOnNext: main_stack_view.push(sim_path)
        }

        Component {
            id: sim_path
            ApplicationFlow {
                StackView.onActivated: {
                    scenario_model.refresh_scenario_list();
                }
            }
        }

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }
    }


}

