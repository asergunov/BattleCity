import QtQuick 2.6
import QtQuick.Window 2.2

import battlecity.engine 1.0 as Engine


Item {
    id: levelSelector
    property variant scene
    property int level: 0
    property int levelCount: 10
    signal levelSelected(int level);
    
    Keys.enabled: state === ""

    Keys.onUpPressed: {
        level = Math.min(levelCount-1, level+1);
    }
    Keys.onDownPressed: {
        level = Math.max(0, level-1);
    }
    Keys.onReturnPressed: {
        levelSelected(level);
        state = "inactive";
    }
    
    state: "inactive"
    
    Component.onCompleted: state = ""
    
    Rectangle {
        id: up
        color: "#636363"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height/2
    }
    
    Rectangle {
        id: down
        color: "#636363"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height/2
    }
    
    Item {
        id: text
        anchors.centerIn: parent
        width: 40+3*8; height: 8
        BaseSprite {
            frameCount: 1
            frameWidth: 40; frameHeight: 8
            frameX: 328; frameY: 176
        }
        
        Number {
            anchors.right: parent.right
            value: levelSelector.level+1
        }
    }
    
    states: [
        State {
            name: "inactive"
            AnchorChanges {
                target: up
                anchors.top: undefined
                anchors.bottom: levelSelector.top
            }
            AnchorChanges {
                target: down
                anchors.bottom: undefined
                anchors.top: levelSelector.bottom
            }
            PropertyChanges {
                target: text
                visible: false
            }
        }
    ]
    
    transitions: [
        Transition {
            from: "inactive"
            SequentialAnimation {
                AnchorAnimation { duration: 1000 }
                PropertyAction { target: text; properties: "visible" }
            }
        },
        Transition {
            to: "inactive"
            SequentialAnimation {
                PropertyAction { target: text; properties: "visible" }
                AnchorAnimation { duration: 1000 }
            }
        }
    ]
    
}
