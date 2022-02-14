import QtQuick 2.0

Item {
    id: tank;
    property int color : 1 // it's 4 colors there gold, gray, green, red
    property int type  : 0 // it's 8 types here 4 for players and 4 for for enemies
    property int direction  : 0
    property alias moving: sprite.running
    width: 16; height: 16;
    BaseSprite {
        id: sprite
        frameCount: 2;
        frameWidth: 16; frameHeight: 16;
        frameX: tank.color%2*128            + tank.direction*32
        frameY: Math.floor(tank.color/2)*128 + tank.type*16
    }
}
