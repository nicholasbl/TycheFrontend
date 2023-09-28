import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material

import "Global"
import "./utility.js" as Util

Item {
    id: root
    property bool in_edit_mode: false

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

            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }

        Label {
            id: current_investment_label
            font.pointSize: 18
            text: Util.format_money(investment)

            Material.foreground: in_edit_mode ? Material.Blue : parent.Material.foreground

            MouseArea {
                id: edit_current_investment_area
                anchors.fill: parent

                onClicked: edit_popup.open()
            }
        }
    }

    Popup {
        id: edit_popup

        property int last_value: 0

        margins: 1

        onOpened: {
            last_value = investment
            invest_slider.value = investment
        }

        ColumnLayout {
            anchors.fill: parent
            RowLayout {
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
                Label {
                    text: name + ":"
                    font.bold: true
                }

                Label {
                    font.pointSize: 18
                    text: Util.format_money(investment)
                    Layout.alignment: Qt.AlignRight
                }
            }
        }
    }
}
