import QtQuick 2.0
// import QtAudioEngine 1.1 // not supported yet

import battlecity.engine 1.0 as Engine


Item {
    id: tankExplosion
    signal finished();
    width: 32; height: 32;
    BaseSprite {
        anchors.centerIn: parent
        id: smallExplosion
        frameWidth: 16;
        frameHeight: 16;
        frameX: 256; frameY: 128;
        frameCount: 3;
        loops: 1;
        onRunningChanged: {
            if (!running) {
                // run huge explosion animation
                hugeExplosion.visible = true
                hugeExplosion.running = true
            }
        }
    }
    BaseSprite {
        id: hugeExplosion
        visible: false
        running: false
        frameWidth: 32;
        frameHeight: 32;
        frameX: 304; frameY: 128;
        frameCount: 2;
        loops: 1;
        onRunningChanged: {
            if (!running) {
                finished();
            }
        }
    }
}
