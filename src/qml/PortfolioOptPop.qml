import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 6.6
import QtQuick.Controls.Material

import "./utility.js" as Util

Dialog {
    title: "Portfolio Optimization"

    onOpened: {
        if (sim_result_model.opt_portfolio_amount < 1) {
            let new_val = sim_result_model.max_opt_portfolio_amount / 2;
            if (new_val > 1){
                sim_result_model.opt_portfolio_amount = new_val
                //portfolio_amount_slider.value = new_val
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        TextField {
            // Turn to text box
            id: portfolio_limit_text
            Layout.fillWidth: true
            placeholderText: "Portfolio Investment Limit"
            text: Util.format_money(sim_result_model.opt_portfolio_amount)

            onAccepted:  {
                if (text.length === 0) {
                    return
                }

                var regexp = /\$?(\d+\.?\d*)\s*([kmb])?/i

                var matches = regexp.exec(text)

                console.log(text, matches)

                // this finds the first int it can get
                var val = parseFloat(matches[1])

                // pull out any mulipliers...
                if (matches.length > 2){
                    var last = matches[2].toLowerCase()

                    if (last === 'k') {
                        val *= 1E3
                    } else if (last === 'm') {
                        val *= 1E6
                    } else if (last === 'b') {
                        val *= 1E9
                    }
                }

                sim_result_model.opt_portfolio_amount = val
            }
        }

        RowLayout {
            Layout.fillWidth: true
            RoundButton {
                text: "\uf522"
                font: loader.font
                flat: true

                onClicked: {
                    // pick a nice random value between the max opt amount, and something larger than zero
                    sim_result_model.opt_portfolio_amount = Math.random() * sim_result_model.max_opt_portfolio_amount + 10000
                }
            }
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
