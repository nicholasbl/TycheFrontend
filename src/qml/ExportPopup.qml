import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Dialog {
    id: root
    margins: 1

    title: "Export"

    standardButtons: Dialog.Close

    ColumnLayout {
        spacing: 5

        Rectangle {
            Layout.preferredHeight: 1
            Layout.fillWidth: true
            color: Material.dividerColor
        }

        Label {
            Layout.fillWidth: true
            text: "Tap below to save an image of the plot area as a PNG image"
        }

        Button {
            Layout.fillWidth: true

            text: "Save Image"
            onClicked: {
                let start_result = explore_root_view.grabToImage(function(result){
                    sim_result_model.ask_save_image(result.image)
                })

                console.log("Start image save result", start_result)
            }
        }

        Rectangle {
            Layout.preferredHeight: 1
            Layout.fillWidth: true
            color: Material.dividerColor
        }

        Label {
            Layout.fillWidth: true
            text: "Tap below to save the current dataset as a JSON document"
        }

        Button {
            Layout.fillWidth: true
            text: "Save Data as..."
            onClicked: {
                archive_model.ask_save_data(archive_view.currentIndex);
            }
        }
    }
}
