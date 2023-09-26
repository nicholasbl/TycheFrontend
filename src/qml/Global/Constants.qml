pragma Singleton
import QtQuick 2.15
import QtQuick.Controls.Material

import "../utility.js" as Util

QtObject {
    readonly property var all_colors: [
        Material.Red,
        Material.Pink,
        Material.Purple,
        Material.DeepPurple,
        Material.Indigo,
        Material.Blue,
        Material.LightBlue,
        Material.Cyan,
        Material.Teal,
        Material.Green,
        Material.LightGreen,
        Material.Lime,
        Material.Yellow,
        Material.Amber,
        Material.Orange,
        Material.DeepOrange,
    ]

    function wrapping_mod(v, len) {
        return ((v % len) + len) % len;
    }

    function get_color(index) {
        return Material.color(all_colors[wrapping_mod(index, all_colors.length)])
    }

    function get_tinted_color(index, tint, fraction, opacity = 1.0) {
        return Util.color_with_alpha(
                    Qt.tint(
                        get_color(index),
                        Util.color_with_alpha(tint, fraction)
                    ),
                opacity
            )
    }
}
