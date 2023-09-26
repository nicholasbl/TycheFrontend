.pragma library


function color_with_alpha(color, alpha) {
    return Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, alpha)
}

class UnitInfo {
    constructor(long_suffix, short_suffix, scalar) {
        this.long_suffix = long_suffix
        this.short_suffix = short_suffix
        this.scalar = scalar
    }
}

const unit_lookup_table = [
    new UnitInfo ( "", "", 1 ),                               // 0
    new UnitInfo ( "kilo",  "k", 1.0 / Math.pow(10, 1 * 3) ), // 1
    new UnitInfo ( "mega",  "M", 1.0 / Math.pow(10, 2 * 3) ), // 2
    new UnitInfo ( "giga",  "G", 1.0 / Math.pow(10, 3 * 3) ), // 3
    new UnitInfo ( "tera",  "T", 1.0 / Math.pow(10, 4 * 3) ), // 4
    new UnitInfo ( "peta",  "P", 1.0 / Math.pow(10, 5 * 3) ), // 5
    new UnitInfo ( "exa",   "E", 1.0 / Math.pow(10, 6 * 3) ), // 6
    new UnitInfo ( "zetta", "Z", 1.0 / Math.pow(10, 7 * 3) ), // 7
    new UnitInfo ( "yotta", "Y", 1.0 / Math.pow(10, 8 * 3) )  // 8
]

function lookup_si(value) {
    value = Math.abs(value);

    let exponent = Math.floor(Math.log10(value));
    let group    = Math.floor(Math.min(Math.max(exponent / 3, 0), 8));

    return unit_lookup_table[group];
}

function format_money(value) {
    const si = lookup_si(value)
    const num = Number(value * si.scalar).toLocaleCurrencyString(Qt.locale("en_US"))
    return num + " " + si.short_suffix
}

