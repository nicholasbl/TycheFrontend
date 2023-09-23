.pragma library


function color_with_alpha(color, alpha) {
    return Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, alpha)
}

class SIInfo {
    constructor(long_suffix, short_suffix, scalar) {
        this.long_suffix = long_suffix
        this.short_suffix = short_suffix
        this.scalar = scalar
    }
}

const si_lookup_table = [
    new SIInfo ( "yocto", "y", 1.0 / Math.pow(10, -8 * 3) ), // -8
    new SIInfo ( "zepto", "z", 1.0 / Math.pow(10, -7 * 3) ), // -7
    new SIInfo ( "atto", "a", 1.0 / Math.pow(10, -6 * 3) ),  // -6
    new SIInfo ( "femto", "f", 1.0 / Math.pow(10, -5 * 3) ), // -5
    new SIInfo ( "pico", "p", 1.0 / Math.pow(10, -4 * 3) ),  // -4
    new SIInfo ( "nano", "n", 1.0 / Math.pow(10, -3 * 3) ),  // -3
    new SIInfo ( "micro", "µ", 1.0 / Math.pow(10, -2 * 3) ), // -2
    new SIInfo ( "milli", "m", 1.0 / Math.pow(10, -1 * 3) ), // -1
    new SIInfo ( "", "", 1 ),                      // 0
    new SIInfo ( "kilo", "k", 1.0 / Math.pow(10, 1 * 3) ),   // 1
    new SIInfo ( "mega", "M", 1.0 / Math.pow(10, 2 * 3) ),   // 2
    new SIInfo ( "giga", "G", 1.0 / Math.pow(10, 3 * 3) ),   // 3
    new SIInfo ( "tera", "T", 1.0 / Math.pow(10, 4 * 3) ),   // 4
    new SIInfo ( "peta", "P", 1.0 / Math.pow(10, 5 * 3) ),   // 5
    new SIInfo ( "exa", "E", 1.0 / Math.pow(10, 6 * 3) ),    // 6
    new SIInfo ( "zetta", "Z", 1.0 / Math.pow(10, 7 * 3) ),  // 7
    new SIInfo ( "yotta", "Y", 1.0 / Math.pow(10, 8 * 3) )  // 8
]

function lookup_si(value) {
    value = Math.abs(value);

    if (value === 0.0) return si_lookup_table[8]

    let exponent = Math.floor(Math.log10(value));
    let group    = Math.floor(Math.min(Math.max(exponent / 3, -8), 8));

    return si_lookup_table[group + 8];
}

function format_money(value) {
    const si = lookup_si(value)
    return Number(value * si.scalar).toLocaleCurrencyString(Qt.locale("en_US")) + si.short_suffix
}

