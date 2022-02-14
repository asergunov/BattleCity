import QtQuick 2.0
// import QtAudioEngine 1.1 // not supported yet

import battlecity.engine 1.0 as Engine


Tank {
    id: player
    property int index: 0
    property int level: 0
    property alias unsetroyable: undestroyable.visible
    property alias controller: input.player
    color: index === 0 ? 0 : 2
    type: level

    Keys.forwardTo: input

    PlayerInput {
        id: input
    }

    BaseSprite {
        id: undestroyable
        running: visible
        frameCount: 2
        frameWidth: 16; frameHeight: 16
        frameX: 256; frameY: 144
    }
}
