import QtQuick 2.0
// import QtAudioEngine 1.1 // not supported yet

import battlecity.engine 1.0 as Engine


Item {
    id: levelIndicator
    property alias value: number.value
    BaseSprite {
        id: flag
        running: false
        frameWidth: 16; frameHeight: 16
        frameX: 376; frameY: 184;
    }
    
    Number {
        id: number
        anchors.top: flag.bottom
        anchors.right: flag.right
    }
    
}
