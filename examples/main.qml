import QtQuick 2.2
import AudioItem 1.1

Rectangle {
    id: audio
    width: 1280
    height: 422
    color: "black"

    Text {
        text: "Car Audio"
        color: "red"
        font.pointSize: 24
        anchors.centerIn: parent
    }

    AudioItem {
        id: audioI
        x: 50
        y: 50
    }
}
