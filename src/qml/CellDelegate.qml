import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts


import "Global"
import "./utility.js" as Util

Rectangle {
    id: root

    color: "transparent"

    border.color: Material.dividerColor
    border.width: 1

    property color fill_color: Qt.white

    property int view_index: 0

    property real data_min_bound: 0
    property real data_max_bound: 1

    FadingStack {
        id: stack_view
        anchors.fill: parent
        anchors.margins: 10

        currentIndex: {
            if (raw_data.length === 0) {
                return 0
            }
            return view_index + 1
        }

        TransparentRectangle {
            background_opacity: .50
        }

        BarChart {
            id: bar_chart
            data_global_min: root.data_min_bound
            data_global_max: root.data_max_bound
            data_array: box_structure
            line_color: Material.foreground
            fill_color: root.fill_color
        }

        ViolinChart {
            id: violin_chart
            data_global_min: root.data_min_bound
            data_global_max: root.data_max_bound
            x_array: violin_x
            line_array: violin_y
            line_color: Material.foreground
            fill_color: root.fill_color
        }

    }
}
