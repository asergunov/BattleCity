import QtQuick 2.0
// import QtAudioEngine 1.1 // not supported yet

import battlecity.engine 1.0 as Engine


Grid {
    property int value
    id: enemyCounter
    columns: 2
    Repeater {
        model: value
        delegate: BaseSprite {
            frameCount: 1
            frameWidth: 8; frameHeight: 8
            frameX: 320; frameY: 192;
        }
    }
}
