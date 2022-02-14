import QtQuick 2.6
import QtQuick.Window 2.2

import battlecity.engine 1.0 as Engine


Item {
    id: content
    width: 256; height: 224
    
    Engine.LevelScene {
        id: engineScene;
    }

    focus: true;

    Keys.forwardTo: [levelSelector, scene]

    LevelScene {
        id: scene
        scene: engineScene
        onGameOver: {
            levelSelector.state = "";
        }

        onLevelCompleted: {
            engineScene.startNextLevel();
        }
    }

    LevelSelector {
        id: levelSelector
        anchors.fill: parent
        scene: engineScene
        levelCount: engineScene.levelCount
        onLevelSelected: {
            engineScene.startLevel(level, 1);
            scene.state = "";
        }
    }
}
