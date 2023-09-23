import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0

TableModel {
    //    ListElement {
    //        line_data: [1,2,3,4,5,2,4,9]
    //        box_data: [-1, 0, 5, 6, 9]
    //    }
    //    ListElement {
    //        line_data: [1,2,3,4,5,2,4,9]
    //        box_data: [-1, 0, 5, 6, 9]
    //    }
    //    ListElement {
    //        line_data: [1,2,3,4,5,2,4,9]
    //        box_data: [-1, 0, 5, 6, 9]
    //    }
    //    ListElement {
    //        line_data: [1,2,3,4,5,2,4,9]
    //        box_data: [-1, 0, 5, 6, 9]
    //    }



    property bool completed: false

        Component.onCompleted: {
            appendRow({
                       "line_data" : [1,2,3,4,5,2,4,9],
                       "box_data"  : [-1, 0, 5, 6, 9],
                   })

            appendRow({
                       "line_data" : [1,2,3,4,5,2,4,9],
                       "box_data"  : [-1, 0, 5, 6, 9],
                   })
            appendRow({
                       "line_data" : [1,2,3,4,5,2,4,9],
                       "box_data"  : [-1, 0, 5, 6, 9],
                   })
            appendRow({
                       "line_data" : [1,2,3,4,5,2,4,9],
                       "box_data"  : [-1, 0, 5, 6, 9],
                   })
            appendRow({
                       "line_data" : [1,2,3,4,5,2,4,9],
                       "box_data"  : [-1, 0, 5, 6, 9],
                   })

            completed = true
        }
}
