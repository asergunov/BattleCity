import QtQuick 2.0

BaseSprite {
    id: digit
    property int value: 0
    currentFrame: value
    running: false
    frameCount: 10
    frameWidth: 8; frameHeight: 8
    frameX: 288; frameY: 200
}
