import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material

import "./utility.js" as Util

Rectangle {
    id: background_item
    color: Util.color_with_alpha(background_color, background_opacity)
    radius: 15
    border.width: use_border
    border.color: Util.color_with_alpha(Material.dividerColor, background_opacity / 2)
    property color background_color: Material.backgroundColor
    property real background_opacity: .9
    property bool use_border: false
}
