import QtQuick 6.5
import QtQuick.Layouts
import QtQuick.Controls 6.5
import QtQuick.Controls.Material

ApplicationWindow {
    width: 960
    height: 540

    visible: true
    title: "Tyche Frontend"

    FontLoader {
        id: loader
        source: "qrc:/fonts/fa_solid_6.otf"
        Component.onCompleted: console.log("Loaded:", name)
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


            width: Math.min(main_stack_view.width * .9, 640)
            height: Math.min(main_stack_view.width * .9, 480)
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            onOnNext: main_stack_view.push(sim_path)
        }

        Component {
            id: sim_path
            ApplicationFlow {

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

