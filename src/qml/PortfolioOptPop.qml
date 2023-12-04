import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 6.6
import QtQuick.Controls.Material

import "./utility.js" as Util

Dialog {
    title: "Portfolio Optimization"

    ColumnLayout {
        Label {
            Layout.fillWidth: true
            text: "Select a minimum or maximum portfolio bound for the optimizer"
        }
        ComboBox {
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

        TextField {
            Layout.fillWidth: true
            placeholderText: "Portfolio limit"
            text: Util.format_money(sim_result_model.opt_portfolio_amount)

            validator: IntValidator {
                bottom: 0
                top: sim_result_model.max_opt_portfolio_amount
            }

            onAccepted:  {
                sim_result_model.opt_portfolio_amount = parseInt(text)
            }
        }

        RowLayout {
            Slider {
                Layout.fillWidth: true
                from: 0
                to: sim_result_model.max_opt_portfolio_amount
                stepSize: 1

                onMoved: {
                    sim_result_model.opt_portfolio_amount = value
                }
            }
            Label {
                text: Util.format_money(sim_result_model.max_opt_portfolio_amount)
            }
        }


    }

    standardButtons: Dialog.Close
}
