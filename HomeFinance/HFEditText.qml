import QtQuick
import QtQuick.Controls

import HomeFinance

TextField {
    id: root

    function activate() {
        selectAll()
        forceActiveFocus()
    }

    font: Style.fonts.n16
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
            color: Style.colors.focused
        }
        Rectangle {
            anchors {
                fill: parent
                bottomMargin: blue.radius
            }
            border {
                width: Style.width.border
                color: "#2A3332"
            }
            color: "#1D1F1F"
        }
    }
}
