import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

FlowScreen {
    id: item1

    header_text: "Select metrics for this scenario"

    help_text: "Pick investment categories and metrics to use for your scenario. You can pick as many as you'd like, but three is a nice optimal number. If you are not sure what to pick, tap continue and the tool will pick some for you."

    before_page_forward: function() {
        category_model.finalize_choices()
        metric_model.finalize_choices()
    }

    RowLayout {
        anchors.fill: parent

        TransparentPane {
            Layout.margins: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.horizontalStretchFactor: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Label {
                    text: "Metrics"
                    font.bold: true
                    Layout.fillWidth: true
                }
                Label {
                    text: "First select metrics to evaluate"
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    clip: true

                    spacing: 4

                    delegate: MetricSelectDelegate {
                        height: 64
                        width: ListView.view.width

                        metric_name: model.name
                        metric_description: model.description
                        metric_image: model.image

                        metric_selected: model.selected

                        onSelection_changed: function(state){
                            model.selected = state
                        }
                    }

                    model: metric_model
                }
            }
        }

        TransparentPane {
            Layout.margins: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.horizontalStretchFactor: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Label {
                    text: "Technologies"
                    font.bold: true
                    Layout.fillWidth: true
                }
                Label {
                    text: "Now select a number of technologies to explore"
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    clip: true

                    spacing: 4

                    delegate: MetricSelectDelegate {
                        height: 64
                        width: ListView.view.width

                        metric_name: model.name
                        metric_description: model.description
                        metric_image: model.image

                        metric_selected: model.selected

                        onSelection_changed: function(state){
                            model.selected = state
                        }
                    }

                    model: category_model
                }
            }
        }
    }
}
