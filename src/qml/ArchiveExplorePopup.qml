import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Material

Dialog {
    margins: 1

    title: "Archived Runs"

    standardButtons: Dialog.Close

    ColumnLayout {
        TextField {
            id: archive_name
            text: large_archive_view.currentItem ? large_archive_view.currentItem.name_from_model : ""

            placeholderText: "Result Name"

            Layout.fillWidth: true

            onTextEdited: {
                if (large_archive_view.currentItem) {
                    large_archive_view.currentItem.user_updated_name(text)
                }
            }
        }

        Frame {
            Layout.preferredWidth: 300
            Layout.preferredHeight: 300

            GridView {
                id: large_archive_view
                anchors.fill: parent

                //currentIndex: archive_view.currentIndex

                onCurrentIndexChanged: {
                    archive_model.select_run(currentIndex)
                }

                Component.onCompleted: {
                    archive_model.new_run_ready.connect(function(){
                        currentIndex = count - 1
                    })
                }

                model: archive_model

                clip: true

                cellWidth: 121
                cellHeight: 121

                delegate: ArchiveDelegate {
                    id: del
                    width: 120
                    height: 120

                    border.color: Material.dividerColor
                    border.width: 1

                    onClicked: {
                        large_archive_view.currentIndex = index
                    }

                    onAskDelete: {
                        archive_model.delete_run(index)
                    }

                    show_delete: edit_stack.currentIndex > 0

                    state: del.GridView.isCurrentItem ? "selected" : ""
                }
            }
        }

        FadingStack {
            id: edit_stack
            Layout.fillWidth: true
            Button {
                Layout.alignment: Qt.AlignRight
                flat: true
                text: "Select"

                onClicked: edit_stack.currentIndex += 1
            }

            RowLayout {
                Button {
                    Material.accent: Material.Red
                    highlighted: true
                    text: "Remove All"

                    onClicked: {
                        archive_model.delete_all_runs()
                        edit_stack.currentIndex = 0
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
                Button {
                    flat: true
                    text: "Done"

                    onClicked: edit_stack.currentIndex = 0
                }
            }
        }

    }
}
