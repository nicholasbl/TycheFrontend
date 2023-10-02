import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

Popup {
    id: root
    margins: 1

    ColumnLayout {
        Label {
            text: "Export"
            font.pointSize: 32
        }
        ColumnLayout {
            Layout.preferredWidth: 250
            TabBar {
                id: export_selection
                TabButton {
                    text: "Image"
                }
                TabButton {
                    text: "Data"
                }
            }
            FadingStack {
                currentIndex: export_selection.currentIndex
                Item {
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

                Item {
                    Button {
                        text: "Save Data"
                    }
                }

            }
        }

        RowLayout {
            Button {
                text: "Close"
                flat: true
                highlighted: true
                onClicked: root.close()
            }
        }
    }
}
