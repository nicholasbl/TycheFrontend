import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material

import "Global"
import "./utility.js" as Util

Item {
    id: root

    property bool enable_editing: false

    property bool round_all: false

    SelectableRectangle {
        anchors.fill: parent
        anchors.margins: 1
        anchors.bottomMargin: 0
        upper_left: 10
        upper_right: 10
        lower_left: round_all ? 10 : 0
        lower_right: round_all ? 10 : 0

        border_color: Material.dividerColor
        border_width: 1

        fill_color: Constants.get_tinted_color(index, Material.background, .75, .50)

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
            anchors.margins: 3
            spacing: 3

            EditLabel {
                id: column_name

                text: name

                font.bold: true
                font.pointSize: 14

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                // disabled for now
                //editable: enable_editing

                onClicked: edit_opts.open()
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
            id: edit_opts
            margins: 1

            ColumnLayout {
                spacing: 5

                RowLayout {
                    Label {
                        Layout.fillWidth: true
                        text: "Optimization options for"

                        color: Material.backgroundDimColor
                    }
                    Label {
                        text: name
                        font.pointSize: 18
                    }
                }

                CheckBox {
                    id: limit_check
                    text: "Set limits"

                    checked: bound_type === "lower" || bound_type === "upper"

                    onClicked: {
                        if (!checked) {
                            bound_type = ""
                        } else {
                            bound_type = "lower"
                        }
                    }
                }

                ColumnLayout {
                    id: extra_opts
                    enabled: limit_check.checked


                    Rectangle {
                        Layout.preferredHeight: 1
                        Layout.fillWidth: true
                        color: Material.dividerColor
                    }

                    SpinBox {
                        Layout.fillWidth: true

                        editable: true

                        from: -1000
                        to: 1000

                        value: optim_value
                        onValueModified: {
                            optim_value = value
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true

                        ComboBox {
                            id: bound_selection_box
                            textRole: "text"
                            valueRole: "value"
                            Layout.fillWidth: true
                            // Sync this in sim result model!
                            model: [
                                {value: "lower", text: "Lower"},
                                {value: "upper", text: "Upper"},
                            ]
                            currentIndex: indexOfValue(bound_type)
                            onActivated: function(index){
                                bound_type = model[index].value
                            }
                        }

                        Label{
                            text: "bound"
                        }
                    }
                }
            }
        }
    }
}
