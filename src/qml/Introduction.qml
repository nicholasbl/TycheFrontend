import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts

Item {
    id: root

    signal onNext

    TransparentPane {
        width: Math.min(parent.width * .9, 640)
        height: Math.min(parent.height * .9, 480)
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        padding: 25

        ColumnLayout {
            anchors.fill: parent

            Image {
                id: nRELlogoreversed
                source: Material.theme === Material.Dark ? "qrc:/images/NREL-logo-reversed.png" : "qrc:/images/NREL-logo.png"
                mipmap: true
                Layout.maximumHeight: 128
                fillMode: Image.PreserveAspectFit
                Layout.maximumWidth: 256
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            Label {
                text: qsTr("Tyche")
                font.pointSize: 32
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            Label {
                text: qsTr("<i>T</i>echnolog<i>y</i> <i>Ch</i>aracterization and <i>E</i>valuation")
                font.pointSize: 18
                horizontalAlignment: Qt.AlignHCenter
                Layout.fillWidth: true
                elide: Label.ElideMiddle
            }

            Rectangle {
                Layout.preferredHeight: 1
                Layout.fillWidth: true
                color: Material.dividerColor
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            Label {
                id: tyche_explain_label
                text: qsTr("The Tyche tool provides a consistent and systematic methodology for evaluating alternative R&D investments in a technology or technology system and for comparing the impacts of these investments on metrics and outcomes of interest. Tyche is intended to provide analytical support for funding decision-makers as they consider how to meet their overall goals with various R&D investment strategies.")
                //horizontalAlignment: Text.AlignJustify
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Rectangle {
                Layout.preferredHeight: 1
                Layout.fillWidth: true
                color: Material.dividerColor
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            Button {
                flat: true
                text: qsTr("Begin ") + loader.icon_string("\uf054")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Material.elevation: 1
                onClicked: {
                    root.onNext()
                }

                contentItem: Label {
                    text: parent.text
                    textFormat: Label.RichText
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}


