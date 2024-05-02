import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

ItemDelegate {
    id: root

    default property alias contents: placeholder.data
    property bool isCurrent: ListView.view.currentIndex === model.index

    background: Rectangle {
        radius: Style.radius.base
        color: root.hovered ? Style.colors.hoveredBackground : root.isCurrent ? Style.colors.selectedBackground : "transparent"
    }
    x: Style.size.listViewDelegateX
    width: ListView.view.width - 2 * x
    height: Style.height.base
    hoverEnabled: true

    RowLayout {
        id: placeholder
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
            margins: Style.margins.listViewDelegate
        }
    }

    onClicked: ListView.view.currentIndex = model.index
}
