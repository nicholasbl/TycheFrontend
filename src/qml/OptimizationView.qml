import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import "./utility.js" as Util

RowLayout {
    id: root
    ColumnLayout {
        Layout.margins: 10
        Layout.fillWidth: true
        Layout.fillHeight: true

        OptInputPane {
            Layout.fillWidth: true
            Layout.preferredHeight: root.height / 2 - down_arrow.implicitHeight / 2
            //Layout.verticalStretchFactor: 2
        }

        Label {
            id: down_arrow
            text: "\uf0d7"
            font.family: loader.font.family
            font.pointSize: 32
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 0
        }

        OptOutputPane {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 2
        }

        // RowLayout {
        //     id: content_row

        //     Layout.fillHeight: true
        //     Layout.fillWidth: true
        //     spacing: 10
        //     ColumnLayout {
        //         id: left_column

        //         spacing: 10

        //         Layout.preferredWidth: root.width / 2
        //         Layout.maximumWidth: root.width / 2
        //         Layout.fillHeight: true

        //         RowLayout {
        //             spacing: 5


        //         }

        //         Rectangle {
        //             Layout.fillWidth: true
        //             Layout.preferredHeight: 1
        //             color: Material.dividerColor
        //         }

        //         Label {
        //             Layout.fillWidth: true
        //             text: "Choose individual category investments"
        //             wrapMode: Label.WrapAtWordBoundaryOrAnywhere
        //             elide: Label.ElideRight
        //             font.bold: true
        //         }


        //     }

        //     Frame {
        //         Layout.fillHeight: true
        //         Layout.fillWidth: true

        //         ColumnLayout {
        //             anchors.fill: parent



        //             ListView {
        //                 Layout.fillHeight: true
        //                 Layout.fillWidth: true
        //                 model: opt_archive_model.met_result()
        //                 delegate: ItemDelegate {
        //                     text: name + ": " + value
        //                 }
        //             }
        //         }


        //     }
        // }
    }

    OptimizeSideBar {
        id: side_bar
        Layout.fillHeight: true
        Layout.margins: 10
        Layout.preferredWidth: 250
    }
}


// Dialog {
//     id: root
//     title: "Optimize"

//     parent: Overlay.overlay
//     anchors.centerIn: Overlay.overlay

//     width: parent.width * .9
//     height: parent.height * .9

//     margins: 1


//     onOpened: {
//         if (sim_result_model.opt_portfolio_amount < 1) {
//             let new_val = sim_result_model.max_opt_portfolio_amount / 2;
//             if (new_val > 1){
//                 sim_result_model.opt_portfolio_amount = new_val
//                 //portfolio_amount_slider.value = new_val
//             }
//         }

//         if (target_metric_combo.currentIndex < 0) {
//             console.log("No target metric selected, fixup")
//             target_metric_combo.currentIndex = 0
//         }
//     }
// }
