import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Shapes

import "./utility.js" as Util

Item {
    id: root

    property color fill_color: "red"

    property real axis_max : {
        //console.log("COUNT", model.count)

        var max = model.count > 0 ? model.get(0).value : 0

        for (var i = 0; i < model.count; i++) {
            max = Math.max(max, model.get(i).value)
        }

        return max
    }

    property var model: ListModel {
        // ListElement {
        //     label: "A"
        //     value: 1
        // }
    }

    function lerp(x, x0, x1, y0, y1) {
        return y0 + (x-x0)*(y1 - y0)/(x1-x0)
    }

    Rectangle {
        color: "transparent"
        //border.color: "black"
        //border.width: 1

        radius: 5

        width: Math.min(parent.width, parent.height)
        height: width


        Shape {
            id: center_shape

            anchors.fill: parent


            // Computed to give us the outline
            property var line_points: {

                let cnt = root.model.count
                if (cnt < 1) {
                    cnt = 1
                }

                let angle = (2 * Math.PI) / cnt
                let center_x = width / 2
                let center_y = height / 2

                //console.log("CENTER", center_x, center_y)

                let ret = []

                let max_radius = width / 2

                for (let i = 0; i < root.model.count ; i++) {
                    let radius = lerp(root.model.get(i).value, 0, root.axis_max, 0, max_radius)
                    let x = Math.sin(angle * i) * radius;
                    let y = Math.cos(angle * i) * radius;

                    ret.push([ center_x + x, center_y + y ])
                }

                //console.log("HERE", ret)

                return ret;
            }

            function make_lines_svg(host) {
                var ret = ""

                if (line_points.length === 0) {
                    return ""
                }

                host.startX = line_points[0][0]
                host.startY = line_points[0][1]

                for (let i = 1; i < line_points.length; i++) {
                    var pos = line_points[i]
                    ret += "L " + pos[0] + " " + pos[1]
                }

                ret += " z"

                return ret
            }

            function make_spikes_svg(host) {
                var ret = ""

                let center_x = width / 2
                let center_y = height / 2

                host.startX = center_x
                host.startY = center_y

                for (let i = 0; i < line_points.length; i++) {
                    var pos = line_points[i]
                    ret += "L " + pos[0] + " " + pos[1]
                    ret += "M " + center_x + " " + center_y
                }

                ret += " z"

                return ret
            }

            ShapePath {
                id: shape_path
                fillColor: root.fill_color
                strokeColor: Util.color_with_alpha(Qt.black, .5)
                startX: 0; startY: 0
                PathSvg {
                    path: center_shape.make_lines_svg(shape_path)
                }
            }

            ShapePath {
                id: spokes
                fillColor: "transparent"
                strokeColor: Material.dividerColor
                startX: 0; startY: 0
                PathSvg {
                    path: center_shape.make_spikes_svg(spokes)
                }
            }


            Rectangle {
                anchors.centerIn: parent
                width: 5
                height: width
                radius: width / 2
                border.color: Material.foreground
                opacity: .5
            }

            Rectangle {
                anchors.centerIn: parent
                width: parent.width / 2
                height: width
                radius: width / 2
                border.color: Material.foreground
                border.width: 1
                color: "transparent"
                opacity: .5
            }

            Rectangle {
                anchors.centerIn: parent
                width: parent.width
                height: width
                radius: width / 2
                border.color: Material.foreground
                border.width: 1
                color: "transparent"
                opacity: .5
            }

            Repeater {
                model: root.model

                Item {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    Label {
                        id: plot_label
                        anchors.left: flip ? undefined : parent.left
                        anchors.right: flip ? parent.right : undefined
                        anchors.verticalCenter: parent.verticalCenter

                        leftPadding: 5
                        rightPadding: 5

                        width: center_shape.width / 2
                        elide: Label.ElideRight

                        verticalAlignment: Label.AlignVCenter

                        transformOrigin: flip ? Item.Right : Item.Left

                        font.pointSize: 10

                        property real ang: {
                            let cnt = root.model.count
                            if (cnt < 1) {
                                cnt = 1
                            }

                            //let angle = (2 * Math.PI) / cnt
                            let angle = -360 / cnt
                            angle = angle * index + 90

                            return angle
                        }

                        rotation: flip ? ang + 180 : ang

                        //onAngChanged: console.log("ANGLE", ang)

                        property bool flip: ang < -90

                        text: label
                    }
                }
            }
        }

        Label {
            id: max_label
            text: Util.format_money(axis_max < 0 ? 0 : axis_max)

            opacity: .5
            font.pointSize: 10

            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label {
            id: half_label
            text: Util.format_money((axis_max < 0 ? 0 : axis_max) / 2)

            opacity: .5
            font.pointSize: 10

            y: max_label.y + parent.height / 4
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
