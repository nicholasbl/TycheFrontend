import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes
import QtQuick.Controls.Material

Item {
    id: root

    function lerp(x, x0, x1, y0, y1) {
        return y0 + (x-x0)*(y1 - y0)/(x1-x0)
    }

    property real data_global_min: 0
    property real data_global_max: 10

    property var data_array: [1,2,5,6,9]

    // computed below

    property var computed_ratios:  {
        var ret = []
        for (let i of data_array) {
            ret.push(lerp(i, data_global_min, data_global_max, 0.0, 1.0))
        }
        return ret
    }

    property real line_heights: height / 2

    property color line_color: "white"
    property color bar_color: "green"

    Rectangle {
        id: center_line
        height: 4
        color: line_color
        anchors.verticalCenter: parent.verticalCenter
        width: (computed_ratios[4] - computed_ratios[0]) * parent.width
        x: root.width * computed_ratios[0]
    }

    Rectangle {
        id: left_bar
        width: 2
        height: root.line_heights
        color: line_color
        anchors.verticalCenter: parent.verticalCenter

        x: root.width * computed_ratios[0]

        Label {
            text: data_array[0]
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle {
        id: right_bar
        width: 2
        height: root.line_heights
        color: line_color
        anchors.verticalCenter: parent.verticalCenter

        x: root.width * computed_ratios[4]

        Label {
            text: data_array[4]
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle {
        id: mid_bar
        width: (computed_ratios[3] - computed_ratios[1]) * root.width
        height: root.line_heights
        color: bar_color
        x: root.width * computed_ratios[1]
        anchors.verticalCenter: parent.verticalCenter
        border.color: line_color
        border.width: 2
        radius: 5

        Label {
            text: data_array[1]
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -implicitHeight
            anchors.right: parent.left
        }

        Label {
            text: data_array[3]
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -implicitHeight
            anchors.left: parent.right
        }
    }

    Rectangle {
        id: mid_line
        width: 2
        height: root.line_heights
        color: line_color
        anchors.verticalCenter: parent.verticalCenter

        x: root.width * computed_ratios[2]

        Label {
            text: data_array[2]
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
