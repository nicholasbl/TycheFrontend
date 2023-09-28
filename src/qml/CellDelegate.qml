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

    FadingStack {
    //StackLayout {
        id: stack_view
        anchors.fill: parent
        anchors.margins: 10

        currentIndex: view_index

        BarChart {
            id: bar_chart
            data_global_min: sim_result_model.all_cell_stats[0]
            data_global_max: sim_result_model.all_cell_stats[1]
            data_array: box_structure
            line_color: Material.foreground
            fill_color: root.fill_color
        }

        ViolinChart {
            id: violin_chart
            data_global_min: sim_result_model.all_cell_stats[0]
            data_global_max: sim_result_model.all_cell_stats[1]
            x_array: violin_x
            line_array: violin_y
            line_color: Material.foreground
            fill_color: root.fill_color
        }

    }
}
