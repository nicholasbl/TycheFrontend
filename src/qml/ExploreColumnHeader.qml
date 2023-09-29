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
                RowLayout {
                    Label {
                        text: optim_min
                    }

                    Slider {
                        value: optim_value
                        from: optim_min
                        to: optim_max

                        stepSize: 1

                        onMoved: {
                            optim_value = value
                        }
                    }

                    Label {
                        text: optim_max
                    }
                }
            }
        }
    }
}
