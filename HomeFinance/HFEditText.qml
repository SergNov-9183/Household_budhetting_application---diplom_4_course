import QtQuick
import QtQuick.Controls

import HomeFinance

TextField {
    id: root

    function activate() {
        selectAll()
        forceActiveFocus()
    }

    font: Style.fonts.n14
    color: Style.colors.font
    background: Item {
        Rectangle {
            id: blue
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            height: 4
            radius: height/2
            color: "#40BDFF"
        }
        Rectangle {
            anchors {
                fill: parent
                bottomMargin: blue.radius
            }
            border {
                width: 1
                color: "#2A3332"
            }
            color: "#1D1F1F"
        }
    }
}
