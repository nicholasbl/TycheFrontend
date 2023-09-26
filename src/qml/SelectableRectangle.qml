import QtQuick 2.15
import QtQuick.Shapes

Shape {
    id: root

    property real upper_left: 0.0
    property real lower_left: 0.0
    property real upper_right: 0.0
    property real lower_right: 0.0

    property color border_color: "white"
    property real border_width: -1

    property color fill_color: "white"

    ShapePath {
        strokeColor: border_color
        strokeWidth: border_width
        fillColor: fill_color

        startX: 0; startY: root.upper_left
        PathArc {
            x: root.upper_left; y: 0
            radiusX: root.upper_left; radiusY: root.upper_left
            useLargeArc: false
        }
        PathLine {
            x: root.width - root.upper_right; y: 0
        }
        PathArc {
            x: root.width; y: root.upper_right
            radiusX: root.upper_right; radiusY: root.upper_right
            useLargeArc: false
        }
        PathLine {
            x: root.width; y: root.height - root.lower_right
        }
        PathArc {
            x: root.width - root.lower_right; y: root.height
            radiusX: root.lower_right; radiusY: root.lower_right
            useLargeArc: false
        }
        PathLine {
            x: root.lower_left; y: root.height
        }
        PathArc {
            x: 0; y: root.height - root.lower_left
            radiusX: root.lower_left; radiusY: root.lower_left
            useLargeArc: false
        }
        PathLine {
            x: 0; y: root.upper_left
        }
    }
}
