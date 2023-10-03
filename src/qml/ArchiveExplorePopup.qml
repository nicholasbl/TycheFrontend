import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Material

Dialog {
    margins: 1

    title: "Archived Runs"

    standardButtons: Dialog.Close

    ColumnLayout {
        GridView {
            id: large_archive_view
            Layout.preferredWidth: 300
            Layout.preferredHeight: 300

            currentIndex: archive_view.currentIndex

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

            onCurrentIndexChanged: {
                archive_view.currentIndex = currentIndex
            }
        }

        FadingStack {
            id: edit_stack
            Layout.fillWidth: true
            Button {
                flat: true
                highlighted: true
                text: "Edit"

                onClicked: edit_stack.currentIndex += 1
            }

            RowLayout {
                DelayButton {
                    Material.accent: Material.Red
                    Material.elevation: 0
                    text: "Remove All"

                    onClicked: {
                        archive_model.delete_all_runs()
                        edit_stack.currentIndex = 0
                    }
                }
                Button {
                    flat: true
                    highlighted: true
                    text: "Done"

                    onClicked: edit_stack.currentIndex = 0
                }
            }
        }


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



    }
}
