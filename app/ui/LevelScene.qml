import QtQuick 2.0
// import QtAudioEngine 1.1 // not supported yet

import battlecity.engine 1.0 as Engine

Rectangle {
    id: levelScene

    property variant scene;
    signal gameOver();
    signal levelCompleted();

    color: "#636363"
    width: field.width+(2+4)*scene.cellSize; height: field.height+2*scene.cellSize;

    Keys.onEscapePressed: {
        scene.paused = !scene.paused
    }

    Component {
        id: bulletExplosionComponent
        BulletExplosion {
            id: bulletExplosion
            loops: 1;
            onRunningChanged: {
                if (!running) {
                    bulletExplosion.destroy();
                }
            }
            z: 150
        }
    }

    Component {
        id: tankExplosionComponent
        TankExplosion {
            id: tankExplosion
            onFinished: tankExplosion.destroy();
            z: 150
        }
    }

    Component {
        id: pointsBubbleComponent
        BaseSprite {
            id: sprite
            frameWidth: 16; frameHeight: 16
            frameCount: 5
            frameX: 288; frameY: 160
            running: false;
            SequentialAnimation {
                running: true
                NumberAnimation { target: sprite; property: "opacity"; from: 0; to: 1.0; duration: 200 }
                PauseAnimation { duration: 1000 }
                NumberAnimation { target: sprite; property: "opacity"; to: 0; duration: 200 }
                ScriptAction { script: sprite.destroy(); }
            }
            z: 200
        }
    }

    Connections {
        target: scene;
        onBulletExplosion: {
            bulletExplosionComponent.createObject(field, {
                "x": position.x-scene.cellSize/2,
                "y": position.y-scene.cellSize/2
            });
        }

        onTankExplosion: {
            tankExplosionComponent.createObject(field, {
                "x": position.x-scene.cellSize,
                "y": position.y-scene.cellSize
            });
        }

        onScore: {
            pointsBubbleComponent.createObject(field, {
               "x": position.x,
               "y": position.y,
               "currentFrame": (score-1)/100
           });
        }

        onGameOver: {
            state = "gameOver"
        }

        onLevelCompleted: {
            levelCompleted();
        }
    }

    Rectangle {
        // z-level here
        // 0 for cells and tanks
        // 1 for the bullets
        // 5 for the bush
        // 100 for bonuses
        // 150 for explosions
        // 200 for score

        id: field
        x: 2*scene.cellSize; y:scene.cellSize;
        width: scene.field.size.width * scene.cellSize
        height: scene.field.size.height * scene.cellSize
        color: "#000"


        Repeater {
            id: players
            model: scene.players
            delegate: Player {
                id: player
                controller: model.controller
                direction: model.direction
                index: index
                level: model.level
                moving: model.moving
                unsetroyable: model.undestroyable
                x: position.x
                y: position.y
                visible: !model.killed

                Component.onCompleted: {
                    console.log("Player completed");

                    // workaround of missing push function in Keys.forwardTo
                    var list = [];
                    for(var i =0; i < levelScene.Keys.forwardTo.length; ++i)
                        list.push(levelScene.Keys.forwardTo[i]);
                    list.push(player);
                    levelScene.Keys.forwardTo = list;
                }

                Component.onDestruction: {
                    console.log("Player destrucion");

                    var list = [];
                    for(var i =0; i < levelScene.Keys.forwardTo.length; ++i) {
                        var item = levelScene.Keys.forwardTo[i];
                        if(item !== player)
                            list.push(item);
                    }
                    levelScene.Keys.forwardTo = list;
                }
            }
        }

        Repeater {
            model: scene.field
            delegate: FieldCell {
                height: scene.cellSize
                width: scene.cellSize
            }
        }

        Repeater {
            model: scene.enemies
            delegate: Enemy {
                id: enemy
                direction: model.direction
                moving: model.moving
                type: model.type+4
                x: position.x
                y: position.y
                spawning: model.spawning
                health: model.health
                bonus: model.bonus
            }
        }

        Repeater {
            model: scene.bullets
            delegate: Bullet {
                id: bullet
                x: position.x
                y: position.y
                direction: model.direction
                z: 1
            }
        }


        Repeater {
            model: scene.bonuses
            delegate: BaseSprite {
                id: bonus
                x: position.x
                y: position.y
                frameCount: 7
                frameWidth: 16; frameHeight: 16
                frameX: 256; frameY: 112
                running: false;
                currentFrame: model.type
                z: 100
            }
        }

        BaseSprite {
            id: eagle
            running: false
            frameCount: 2
            frameWidth: 16; frameHeight: 16
            frameX: 304; frameY: 32
            currentFrame: scene.eagle.killed ? 1 : 0
            x: scene.eagle.position.x; y: scene.eagle.position.y
        }
    }

    Item {
        width: 32; height: 216
        anchors.left: field.right
        anchors.top: field.top

        Column {
            x: 8; y: 8*15
            spacing: 8
            Repeater {
                id: playerLife
                model: scene.players
                BaseSprite {
                    running: false
                    currentFrame: index
                    frameWidth: 16; frameHeight: 16
                    frameX: 288; frameY: 208;
                    Digit {
                        x: 8; y: 8
                        value: lifeCount
                    }
                }
            }
        }

        LevelIndicator {
            id: levelIndicator
            x: 8; y:8*21
            value: scene.level
        }

        EnemyCounter {
            id: enemyCounter
            x: 8; y: 8
            value: scene.enemiesRemain
        }
    }

    BaseSprite {
        id: gameOverSign
        frameWidth: 32; frameHeight: 16
        frameX: 288; frameY: 184

        anchors.horizontalCenter: field.horizontalCenter
        y: levelScene.height
    }

    BaseSprite {
        id: pauseSign
        visible: scene.paused;
        frameWidth: 40; frameHeight: 8
        frameX: 288; frameY: 176
        anchors.centerIn: field

        SequentialAnimation {
            running: pauseSign.visible
            loops: Animation.Infinite
            NumberAnimation  {
                target: pauseSign
                property: "opacity"
                duration: 500
                easing.type: Easing.InOutQuad
                from: 1; to: 0
            }

            NumberAnimation {
                target: pauseSign
                property: "opacity"
                duration: 500
                easing.type: Easing.InOutQuad
                from: 0; to: 1
            }
        }
    }

    states: [
        State {
            name: "gameOver"
            PropertyChanges {
                target: gameOverSign
                y: (field.height-gameOverSign.height)/2
            }
        }
    ]

    transitions: [
        Transition {
            to: "gameOver"
            SequentialAnimation {
                NumberAnimation { properties: "y"; duration: 5000 }
                ScriptAction { script: levelScene.gameOver(); }
            }
        }
    ]

}
