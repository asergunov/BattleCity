import QtQuick 2.0
// import QtAudioEngine 1.1 // not supported yet

Item {
    id: enemy
    property bool spawning: true
    property bool bonus: false
    property int health: 1

    property alias type: tank.type
    property alias moving: tank.moving
    property alias direction: tank.direction

    width: tank.width; height: tank.height

    onBonusChanged: {
        bonusBlink.running = bonus
        if(!bonus) {
            tank.color = tank.baseColor
        }
    }

    Tank {
        property int baseColor: enemy.health <= 1
                                ? 1
                                : (enemy.health <= 3
                                  ? 2
                                  : 0)

        id: tank
        visible: !spawning
        color: baseColor

        SequentialAnimation {
            id: bonusBlink
            running: false
            loops: Animation.Infinite
            ScriptAction { script: tank.color = tank.baseColor; }
            PauseAnimation { duration: 60 }
            ScriptAction { script: tank.color = 3; }
            PauseAnimation { duration: 60 }
        }
    }

    Item {
        id: spawn
        width: sprite.width; height: sprite.height
        visible: enemy.spawning
        onVisibleChanged: sprite.running = visible;
        BaseSprite {
            id: sprite
            frameWidth: 16; frameHeight: 16
            frameX: 256; frameY: 96
            frameCount: 4;
            loops: 1
            frameRate: 30;
            onRunningChanged: {
                if(running || !spawn.visible)
                    return;
                reverse = !reverse;
                running = true;
            }
        }
    }
}
