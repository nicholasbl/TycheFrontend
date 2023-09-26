import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

Pane {
    id: control
    property real background_opacity: .8
    property color background_color: Material.backgroundColor

    Material.roundedScale: Material.MediumScale
    Material.elevation: 2

    background: Rectangle {
        color: background_color
        radius: control.Material.roundedScale

        opacity: background_opacity

        layer.enabled: control.enabled && control.Material.elevation > 0
        layer.effect: RoundedElevationEffect {
            elevation: control.Material.elevation
            roundedScale: control.background.radius
        }
    }
}
