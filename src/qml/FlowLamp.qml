import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material

Rectangle {
    id: rectangle
    width: height
    radius: width / 2.0

    property alias icon_text: icon.text
    property alias text: lamp_label.text

    color: Material.backgroundColor

    Label {
        id: icon
        anchors.fill: parent

        anchors.margins: parent.radius / 3

        text: "\uf015"

        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter

        font.family: "Font Awesome 6 Free"
        font.pixelSize: 256
        fontSizeMode: Text.Fit
        opacity: .25
    }

    Label {
        id: lamp_label

        text: "Example Text"

        anchors.fill: parent

        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter

        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    }
    states: [
        State {
            name: "highlighted"
            PropertyChanges {
                target: rectangle
                color: Material.color(Material.Green)
            }
        }
    ]
    transitions: [
        Transition {
            id: transition
            ParallelAnimation {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 50
                    }

                    PropertyAnimation {
                        target: rectangle
                        property: "color"
                        duration: 150
                    }
                }
            }
            to: "*"
            from: "*"
        }
    ]

}

/*##^##
Designer {
    D{i:0}D{i:5;transitionDuration:2000}
}
##^##*/
