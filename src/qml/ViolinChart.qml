import QtQuick 2.15
import QtQuick.Shapes

Item {
    id: root

    property real data_global_min: 0
    property real data_global_max: 10

    property color line_color: "white"
    property color fill_color: "green"

    property var x_array: [1,2,3,4,5,6,7]
    property var line_array: [1,2,3,4,3,2,7]

    function lerp(x, x0, x1, y0, y1) {
        return y0 + (x-x0)*(y1 - y0)/(x1-x0)
    }

    // Computed to give us the outline
    property var line_points: {

        if (Math.abs(data_global_max - data_global_min) < .0000001) {
            return []
        }

        let data_max = line_array[0]
        let data_min = line_array[0]
        {
            for (const data_point of line_array) {
                data_max = Math.max(data_max, data_point);
                data_min = Math.min(data_min, data_point);
            }
        }

        let x_min = x_array[0]
        let x_max = x_array[x_array.length - 1]

        let ret = []
        for (var i = 0; i < Math.min(line_array.length, x_array.length); i++) {
            let data_x = x_array[i];
            let data_y = line_array[i];

            let w = lerp(data_x, data_global_min, data_global_max, 0, width)
            let h = lerp(data_y, 0, data_max, height/2, 0)
            ret.push( [w, h] )
        }

        // now we can do a sampling if this array is too long; we dont want to
        // generate a TON of geometry


        // lower part
        for (i = line_array.length - 1; i >= 0 ; i--) {
            //print(ret[i][1], ret[i][1] * -1, ret[i][1] * -1 + height)
            ret.push( [ret[i][0] + 0, -ret[i][1] + height] )
        }

        return ret;
    }

    function make_lines_svg(host) {
        if (line_points.length === 0) {
            return ""
        }

        var ret = ""
        host.startX = line_points[0][0]
        host.startY = line_points[0][1]

        // we go over the length and use a mod to close off this path
        for (let i = 1; i < line_points.length + 1; i++) {
            var pos = line_points[i % line_points.length]
            ret += " L " + pos[0] + " " + pos[1]
        }

        ret += " z"

        return ret
    }

    Shape {
        anchors.fill: parent
        ShapePath {
            id: shape_path
            fillColor: root.fill_color
            strokeWidth: 1
            strokeColor: root.line_color
            joinStyle: ShapePath.RoundJoin
            startX: 0; startY: 0
            PathSvg {
                path: make_lines_svg(shape_path)
            }
        }
    }
}
