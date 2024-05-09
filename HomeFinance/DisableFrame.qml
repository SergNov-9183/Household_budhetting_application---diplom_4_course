import QtQuick
import Qt5Compat.GraphicalEffects

import HomeFinance

MouseArea {
    id: root
    anchors.fill: parent
    hoverEnabled: true

    Rectangle{
        anchors.fill: parent
        color: "#000000"
        opacity: 0.6
        radius: Style.radius.window
    }

    onClicked:         (mouse) => mouse.accepted = true
    onDoubleClicked:   (mouse) => mouse.accepted = true
    onPositionChanged: (mouse) => mouse.accepted = true
    onPressAndHold:    (mouse) => mouse.accepted = true
    onPressed:         (mouse) => mouse.accepted = true
    onReleased:        (mouse) => mouse.accepted = true
    onWheel:           (mouse) => mouse.accepted = true
}

