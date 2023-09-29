import QtQuick 2.15
import QtQuick.Layouts

// Cobbled together from stackoverflow
// ex. https://stackoverflow.com/a/68008004
StackLayout {
    id: root
    property int previousIndex: 0
    property var items: children
    property Item previousItem: items[previousIndex]
    property Item currentItem: items[currentIndex]

    Component.onCompleted: {
        previousIndex = currentIndex

        for(var i = 0; i < count; ++i) {
            children[i].opacity = (i === currentIndex)
        }
    }

    Component {
        id: cross_fade_anim
        SequentialAnimation {
            property Item fade_out_target
            property Item fade_in_target

            ParallelAnimation {

                NumberAnimation {
                    target: fade_out_target
                    property: "opacity"
                    to: 0
                    duration: 300
                }

                NumberAnimation {
                    target: fade_in_target
                    property: "opacity"
                    to: 1
                    duration: 300
                }
            }
            ScriptAction {
                script: fade_out_target.visible = false
            }
        }
    }

    onCurrentIndexChanged: {
        items = root.children

        if(previousItem && currentItem && (previousItem != currentItem)) {
            previousItem.visible = true
            currentItem.visible = true
            var crossFaderAnim = cross_fade_anim.createObject(parent,
                                                              {
                                                                  fade_out_target: previousItem,
                                                                  fade_in_target: currentItem
                                                              })
            crossFaderAnim.restart()
        }
        previousIndex = currentIndex
    }
}
