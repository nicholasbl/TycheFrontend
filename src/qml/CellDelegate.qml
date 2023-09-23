import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes

Rectangle {
    id: root
    //color: "black"
    //width: Math.max(GridView.view.width / 3, 100)
    //height: Math.max(GridView.view.width / 3, 100)
    //width: GridView.view.cellWidth
    //height: GridView.view.cellHeight

    color: "transparent"

    border.color: Material.dividerColor
    border.width: 1

    property var line_array: [1,2,3,4,3,2,7]
    property var box_array: [1,2,3,4,5]


    property var line_points: {

        let data_max = 0
        {
            for (const data_point of line_array) {
                data_max = Math.max(data_max, data_point);
            }
        }

        let ret = []
        for (var i = 0; i < line_array.length; i++) {
            let w = width * (i)/(line_array.length - 1)
            let h = height/2 - line_array[i]/data_max * height / 2
            ret.push( [w, h] )
        }

        // lower part
        for (i = line_array.length - 1; i >= 0 ; i--) {
            ret.push( [ret[i][0] + 0, ret[i][1] * -1 + height] )
        }

        return ret;
    }


    function make_lines(host) {
        var ret = []
        host.startX = line_points[0][0]
        host.startY = line_points[0][1]
        for (let i = 1; i < line_points.length; i++) {
            // midpoint should be a move?
            var pos = line_points[i]
            var path_line_obj = Qt.createQmlObject('import QtQuick 2.15; PathLine {}', host);
            path_line_obj.x = pos[0]
            path_line_obj.y = pos[1]
            ret.push(path_line_obj)
        }
        return ret
    }

    StackView {
        anchors.fill: parent

        initialItem: bar_chart

        Component {
            id: line_chart

            Shape {
                ShapePath {
                    id: shape_path
                    fillColor: "red"
                    strokeWidth: 1
                    strokeColor: "red"
                    joinStyle: ShapePath.RoundJoin
                    startX: 0; startY: 0
                    pathElements: make_lines(shape_path)
                }
            }
        }

        BarChart {
            id: bar_chart
            data_global_min: 0
            data_global_max: 10
            data_array: box_array
        }
    }


}
