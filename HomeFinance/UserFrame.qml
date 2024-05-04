import QtQuick
import QtQuick.Controls

import HomeFinance

AbstractButton {
    id: root
    implicitHeight: photo.height + Style.margins.base
    implicitWidth: photo.width + label.implicitWidth + 3 * Style.margins.base

    background: Rectangle {
        radius: Style.radius.window
        color: hovered ? "#2C2D30" : "transparent"

        Image {
            id: photo
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: Style.margins.base
            }
            width: Style.size.photo
            height: Style.size.photo
            source: "qrc:/Images/User.svg"
        }

        Column {
            anchors {
                verticalCenter: parent.verticalCenter
                left: photo.right
                leftMargin: Style.margins.base
            }

            HFLabel {
                id: label
                text: qsTr("Пользователь:")
                font: Style.fonts.n14
            }
            HFLabel {
                width: label.implicitWidth
                text: globalController.userName
                font: Style.fonts.b14
            }
        }
    }
}
