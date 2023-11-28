import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material

import "Global"
import "./utility.js" as Util

Item {
    property bool enable_editing: false

    SelectableRectangle {
        anchors.fill: parent
        anchors.margins: 1
        anchors.bottomMargin: 0
        upper_left: 10
        upper_right: 10

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

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                editable: enable_editing

                onClicked: edit_opts.open()
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
                        text: "Adjust optimization"

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

                TextField {
                    Layout.fillWidth: true
                    Layout.topMargin: 4
                    placeholderText: "Optimization value"
                    text: optim_value
                    onEditingFinished: {
                        optim_value = text
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Label{
                        text: "Bound"
                    }
                    ComboBox {
                        id: bound_selection_box
                        Layout.fillWidth: true
                        // Sync this in sim result model!
                        model: ["Above", "Below"]
                        currentIndex: bound_type
                        onActivated: {
                            bound_type = currentIndex
                        }
                    }
                }
            }
        }
    }
}
