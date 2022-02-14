import QtQuick 2.0

import battlecity.engine 1.0 as Engine

BaseSprite {
    x: cell.x*scene.cellSize; y: cell.y*scene.cellSize;
    currentFrame: {
        if(type !== Engine.Field.Brick)
            return 0;

        switch(destruction) {
        case Engine.Field.Up:      return 2;
        case Engine.Field.Down:    return 4;
        case Engine.Field.Left:    return 1;
        case Engine.Field.Right:   return 3;

        case Engine.Field.Up | Engine.Field.Left:       return 7;
        case Engine.Field.Up | Engine.Field.Right:      return 8;
        case Engine.Field.Down | Engine.Field.Left:     return 6;
        case Engine.Field.Down | Engine.Field.Right:    return 5;

        default:            return 0;
        }
    }
    Component.onCompleted: {
        var baseX = 256;
        var baseY = 64;
        frameWidth = 8;
        frameHeight = 8;
        switch(type) {
        case Engine.Field.Stone:
            frameX = baseX;
            frameY = baseY + 8;
            break;
        case Engine.Field.Brick:
            frameX = baseX;
            frameY = baseY;
            frameCount = 9;
            running = false;
            break;
        case Engine.Field.Bush:
            frameX = baseX + 8;
            frameY = baseY + 8;
            z = 5;
            break;
        case Engine.Field.Ice:
            frameX = baseX + 16;
            frameY = baseY + 8;
            break;
        case Engine.Field.Water:
            frameX = baseX + 0;
            frameY = baseY + 16;
            frameCount = 3;
            break;
        }
    }
    
}
