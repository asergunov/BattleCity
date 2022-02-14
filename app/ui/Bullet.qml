import QtQuick 2.0

import battlecity.engine 1.0 as Engine

BaseSprite {
    id: bullet
    property int direction: Angine.Actor.Up

    frameCount: 4;
    frameX: 320; frameY: 100;
    frameWidth: 8; frameHeight: 8;
    running: false;
    currentFrame: {
        switch(direction) {
        case Engine.ActorList.Up:
            return 0;
        case Engine.ActorList.Left:
            return 1;
        case Engine.ActorList.Down:
            return 2;
        case Engine.ActorList .Right:
            return 3;
        }
    }
}
