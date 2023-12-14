import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material

import "Global"
import "./utility.js" as Util

Item {
    id: root
    property bool in_edit_mode: false

    property bool in_opt_mode: false

    SelectableRectangle {
        anchors.fill: parent
        anchors.margins: 1
        anchors.bottomMargin: 0
        lower_left: 10
        upper_left: 10

        fill_color: Constants.get_tinted_color(-index, Material.background, .75, .50)
    }

    RoundedImage {
        anchors.fill: parent
        anchors.margins: 2
        asynchronous: true
        source: image
        autoTransform: true
        fillMode: Image.PreserveAspectCrop
        radius: 10

        opacity: .1

        BusyIndicator {
            anchors.fill: parent
            running: parent.status === Image.Loading
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        Label {
            id: column_name
            text: name
            font.bold: true
            font.pointSize: 14
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true

            EditLabel {
                id: bounding_investment_label
                font.pointSize: 16

                text: loader.icon_string("\ue4c2 ") + Util.format_money(opt_limit >= 0 ? opt_limit : 0)
                textFormat: EditLabel.RichText

                visible: in_opt_mode && sim_result_model.opt_portfolio_amount
                editable: in_opt_mode && sim_result_model.opt_portfolio_amount

                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 5

                onClicked: opt_popup.open()
            }

            Item {
                Layout.fillWidth: true
            }

            EditLabel {
                id: current_investment_label
                font.pointSize: 18
                text: Util.format_money(investment)

                editable: in_edit_mode

                Layout.alignment: Qt.AlignRight

                onClicked: edit_popup.open()
            }
        }
    }

    Label {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        text: "\uf05a"
        font: loader.font
        color: Material.backgroundDimColor

        MouseArea {
            id: info_label
            anchors.fill: parent

            hoverEnabled: true
        }

        ToolTip {
            visible: info_label.containsMouse || info_label.containsPress
            text: description
            timeout: 5000
        }
    }

    Popup {
        id: edit_popup
        margins: 1

        property int last_value: 0

        onOpened: {
            last_value = investment
            invest_slider.value = investment
        }

        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                Label {
                    Layout.fillWidth: true
                    text: "Adjust investment"

                    color: Material.backgroundDimColor
                }
                Label {
                    text: name
                    font.pointSize: 18
                }
            }

            Rectangle {
                Layout.preferredHeight: 1
                Layout.fillWidth: true
                color: Material.dividerColor
            }

            RowLayout {
                Layout.fillWidth: true


                Slider {
                    id: invest_slider
                    from: 0
                    to: max_investment
                    stepSize: 1
                    value: investment

                    onMoved: {
                        investment = value
                    }
                }

                Label {
                    text: Util.format_money(max_investment)
                }

            }

            RowLayout {
                Layout.fillWidth: true
                RoundButton {
                    flat: true
                    font: loader.font
                    text: "\uf2ea"
                    highlighted: true

                    onClicked: {
                        investment = edit_popup.last_value
                        invest_slider.value = edit_popup.last_value
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Label {
                    font.pointSize: 18
                    text: Util.format_money(investment)
                    Layout.alignment: Qt.AlignRight
                }
            }
        }
    }

    Popup {
        id: opt_popup
        margins: 1

        property int last_value: 0

        onOpened: {
            last_value = opt_limit
            invest_slider.value = opt_limit
        }

        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                Label {
                    Layout.fillWidth: true
                    text: "Investment limit"

                    color: Material.backgroundDimColor
                }
                Label {
                    text: name
                    font.pointSize: 18
                }
            }

            Rectangle {
                Layout.preferredHeight: 1
                Layout.fillWidth: true
                color: Material.dividerColor
            }

            RowLayout {
                Layout.fillWidth: true


                Slider {
                    id: opt_slider
                    from: 0
                    to: {
                        var max_port = sim_result_model.opt_portfolio_amount

                        if (max_port < 1) {
                            return max_investment
                        }

                        return max_port
                    }

                    stepSize: 1
                    value: opt_limit >= 0 ? opt_limit : 0

                    onMoved: {
                        opt_limit = value
                    }
                }

                Label {
                    text: Util.format_money(opt_slider.to)
                }

            }

            RowLayout {
                Layout.fillWidth: true
                RoundButton {
                    flat: true
                    font: loader.font
                    text: "\uf2ea"
                    highlighted: true

                    onClicked: {
                        opt_limit = edit_popup.last_value
                        invest_slider.value = edit_popup.last_value
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Label {
                    font.pointSize: 18
                    text: Util.format_money(opt_slider.value)
                    Layout.alignment: Qt.AlignRight
                }
            }
        }
    }
}
