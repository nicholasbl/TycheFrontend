import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts

TransparentPane {
    id: root

    signal onNext

    ColumnLayout {
        anchors.fill: parent

        Image {
            id: nRELlogoreversed
            source: Material.theme === Material.Dark ? "qrc:/images/NREL-logo-reversed.png" : "qrc:/images/NREL-logo.png"
            mipmap: true
            Layout.maximumHeight: 128
            fillMode: Image.PreserveAspectFit
            Layout.preferredWidth: 256
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Label {
            id: tyche_intro_name
            y: 200
            text: qsTr("Tyche")
            font.pointSize: 32
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Label {
            id: tyche_explain_label
            text: qsTr("This is some basic text to let people know what this tool is about, what it can do for them, basic knowledge required, etc.")
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("Begin")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            highlighted: true
            flat: true
            onClicked: {
                root.onNext()
            }
        }
    }
}
