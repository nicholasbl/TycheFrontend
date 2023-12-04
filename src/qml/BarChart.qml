import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes
import QtQuick.Controls.Material

import "./utility.js" as Util

Item {
    id: root

    function lerp(x, x0, x1, y0, y1) {
        return y0 + (x-x0)*(y1 - y0)/(x1-x0)
    }

    property real data_global_min: 0
    property real data_global_max: 10

    property var data_array: [1,2,5,6,9]

    // computed below

    property var computed_x:  {
        if (!data_array || data_array.length < 5) {
            return [0,1,1,1,2]
        }

        var ret = []
        for (let i of data_array) {
            ret.push(lerp(i, data_global_min, data_global_max, 0.0, width))
        }
        return ret
    }

    property bool width_not_very_small: {
        var delta = center_line.width

        return delta > 10
    }

    property bool width_not_kinda_small: {
        var delta = center_line.width

        return delta > 30
    }

    property real height_scale: .9

    property real line_heights: height * height_scale

    property real line_opacity: .75

    property color line_color: "white"
    property color fill_color: "green"

    property color resolved_line_color: Util.color_with_alpha(line_color, line_opacity)

    Rectangle {
        id: center_line
        height: 4
        color: resolved_line_color
        anchors.verticalCenter: parent.verticalCenter
        width: (computed_x[4] - computed_x[0])
        x: computed_x[0]
    }

    Rectangle {
        id: left_bar
        width: 1
        height: root.line_heights
        color: resolved_line_color
        anchors.verticalCenter: parent.verticalCenter

        x: computed_x[0]

        Label {
            text: data_array[0].toPrecision(3)
            anchors.top: parent.bottom
            anchors.right: parent.left
            opacity: root_mouse.containsMouse && width_not_very_small

            font.pointSize: 8

            Behavior on opacity {
                NumberAnimation {
                    duration: 100
                }
            }
        }
    }

    Rectangle {
        id: right_bar
        width: 1
        height: root.line_heights
        color: resolved_line_color
        anchors.verticalCenter: parent.verticalCenter

        x: computed_x[4]

        Label {
            text: data_array[4].toPrecision(3)
            anchors.top: parent.bottom
            anchors.left: parent.right
            opacity: root_mouse.containsMouse && width_not_very_small

            font.pointSize: 8

            Behavior on opacity {
                NumberAnimation {
                    duration: 100
                }
            }
        }
    }

    Rectangle {
        id: mid_bar
        width: (computed_x[3] - computed_x[1])
        height: root.line_heights
        color: fill_color
        x: computed_x[1]
        anchors.verticalCenter: parent.verticalCenter
        border.color: line_color
        border.width: 1
        radius: 5

        Label {
            text: data_array[1].toPrecision(3)
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: implicitHeight
            anchors.right: parent.left
            opacity: root_mouse.containsMouse && width_not_kinda_small

            font.pointSize: 8

            Behavior on opacity {
                NumberAnimation {
                    duration: 100
                }
            }
        }

        Label {
            text: data_array[3].toPrecision(3)
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: implicitHeight
            anchors.left: parent.right
            opacity: root_mouse.containsMouse && width_not_kinda_small

            font.pointSize: 8

            Behavior on opacity {
                NumberAnimation {
                    duration: 100
                }
            }
        }
    }

    Rectangle {
        id: mid_line
        width: 1
        height: root.line_heights
        color: line_color
        anchors.verticalCenter: parent.verticalCenter

        x: computed_x[2]

        Label {
            text: data_array[2].toPrecision(3)
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: root_mouse.containsMouse

            font.pointSize: 8

            Behavior on opacity {
                NumberAnimation {
                    duration: 100
                }
            }
        }
    }

    MouseArea {
        id: root_mouse
        anchors.fill: parent
        hoverEnabled: true
    }
}
