import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 6.6
import QtQuick.Controls.Material

import "./utility.js" as Util

Dialog {
    title: "Portfolio Optimization"



    ColumnLayout {
        anchors.fill: parent


        RowLayout {
            Layout.fillWidth: true

        }

        // RowLayout {
        //     Slider {
        //         id: portfolio_amount_slider
        //         Layout.fillWidth: true
        //         from: 0
        //         to: sim_result_model.max_opt_portfolio_amount
        //         stepSize: 1

        //         onMoved: {
        //             sim_result_model.opt_portfolio_amount = value
        //         }
        //     }
        //     Label {
        //         text: Util.format_money(sim_result_model.max_opt_portfolio_amount)
        //     }
        // }


    }

    standardButtons: Dialog.Close
}
