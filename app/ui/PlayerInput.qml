import QtQuick 2.0
import battlecity.engine 1.0 as Engine

Item {
    id: playerInput
    property QtObject player

    property int key_up: Qt.Key_Up;
    property int key_down: Qt.Key_Down;
    property int key_left: Qt.Key_Left;
    property int key_right: Qt.Key_Right
    property int key_fire: Qt.Key_Space;

    Keys.enabled: player

    QtObject {
        id: data
        property int directionKeysPressed: 0
    }
    
    Keys.onPressed: {
        switch(event.key) {
        case key_left:
            player.turn(Engine.ActorList.Left);
            break;
        case key_right:
            player.turn(Engine.ActorList.Right);
            break;
        case key_up:
            player.turn(Engine.ActorList.Up);
            break;
        case key_down:
            player.turn(Engine.ActorList.Down);
            break;
        case key_fire:
            player.fire();
            return;
        default:
            event.accepted = false;
            return;
        }
        
        data.directionKeysPressed++;
        player.move();
        event.accepted = true;
    }
    
    Keys.onReleased: {
        switch(event.key) {
        case key_left:
        case key_right:
        case key_up:
        case key_down:
        case key_fire:
            break;
        default:
            event.accepted = false;
            return;
        }


        // we will not recive up events for rejected
        event.accepted = true;
        if(event.key === key_fire) {
            return;
        }
        
        data.directionKeysPressed--;
        if(!data.directionKeysPressed)
            player.stop();
    }
}
