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
        GroupBox {
            Layout.fillWidth: true

            title: "Screenshot"
            ColumnLayout {
                anchors.fill: parent
                Button {
                    text: "Save Image"
                    onClicked: {
                        let start_result = explore_root_view.grabToImage(function(result){
                            sim_result_model.ask_save_image(result.image)
                        })

                        console.log("Start image save result", start_result)
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true

            title: "Data"
            ColumnLayout {
                anchors.fill: parent
                Button {
                    text: "Save Data as..."
                    onClicked: {
                        archive_model.ask_save_data(archive_view.currentIndex);
                    }
                }
            }
        }
    }
}
