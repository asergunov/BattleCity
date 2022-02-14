import QtQuick 2.0
// import QtAudioEngine 1.1 // not supported yet

import battlecity.engine 1.0 as Engine


Row {
    id: number
    property int value: 0
    property int base: 10

    function digitCount(base, value) {
        return value < base ? 1 : 1+Math.floor(Math.log(value) / Math.log(base))
    }

    function digit(base, value, index) {
        index = digitCount(base, value) - index - 1;
        return Math.floor((value/Math.pow(base, index)) % base);
    }

    Repeater{
        model: number.digitCount(base, value)
        Digit {
            value: number.digit(number.base, number.value, index)
        }
    }
}
