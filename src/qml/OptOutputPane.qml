import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import "./utility.js" as Util
import "Global"

TransparentPane {
    id: root
    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Label {
                text: "Category Results"
                font.bold: true
            }

            ListView {
                id: cat_view
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: opt_archive_model.cat_result()
                spacing: 5
                delegate: TransparentRectangle {
                    width: ListView.view.width
                    height: 48
                    background_color:  Constants.get_tinted_color(index, Material.background, .75, .50)
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 5
                        Label {
                            text: name
                            font.bold: true
                        }
                        Label {
                            text: Util.format_money(value)
                            font.bold: true
                            Layout.alignment: Qt.AlignRight
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Label {
                text: "Metric Results"
                font.bold: true
            }
            ListView {
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: opt_archive_model.met_result()
                spacing: 5
                delegate: TransparentRectangle {
                    width: ListView.view.width
                    height: 48
                    background_color:  Constants.get_tinted_color(index, Material.background, .75, .50)
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 5
                        Label {
                            text: name
                            font.bold: true
                        }
                        Label {
                            text: value.toPrecision(3)
                            font.bold: true
                            Layout.alignment: Qt.AlignRight
                        }
                    }
                }
            }
        }

        SpiderPlot{
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredWidth: 180
            fill_color: Util.color_with_alpha(Material.accentColor, .5)

            function rebuild() {
                let source = opt_archive_model.cat_result()
                let ret = []

                model.clear()

                for (let i = 0; i < cat_view.count; i++) {
                    var v = {
                        "label" : cat_view.model.get_ui_data(i, "name"),
                        "value" : cat_view.model.get_ui_data(i, "value")
                    }

                    model.append(v)
                }

            }

            //values: make_values()

            Component.onCompleted: {
                opt_archive_model.cat_result().content_changed.connect(function(){
                    rebuild()
                })
            }


        }
    }
}
