import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    visible: true
    width: sceneView.width
    height: sceneView.height

    title: qsTr("Battle City")


    Content {
        id: content
    }


    TVEffect {
        id: sceneView
        width: content.width*2; height: content.height*2
        source: ShaderEffectSource {
            sourceItem: content
            live: true
            hideSource: true
        }
    }

}
