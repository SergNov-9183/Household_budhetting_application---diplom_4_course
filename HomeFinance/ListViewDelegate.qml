import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

ItemDelegate {
    id: root

    default property alias contents: placeholder.data
    property alias contentSpacing: placeholder.spacing
    property alias dragActive: dragArea.drag.active
    property alias containsDrag: dropArea.containsDrag
    property alias drag: dropArea.drag
    property alias modelId: contentItemWrapper.modelId
    property bool isCurrent: ListView.view.currentIndex === model.index
    property Item treeView: ListView.view
    property bool acceptDrop: false

    signal itemClicked()
    signal dropEvent(Item target)

    background: Rectangle {
        property bool dropMode: dropArea.containsDrag
        border {
            width: root.acceptDrop ? 1 : 0
            color: root.acceptDrop ? "#55A5EF" : "transparent"
        }

        radius: Style.radius.base
        color: root.hovered ? Style.colors.hoveredBackground : root.isCurrent ? Style.colors.selectedBackground : "transparent"

        Item {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.left
                rightMargin: placeholder.spacing
            }

            height: root.height - Style.margins.base
            width: 3

            Rectangle {
                anchors.fill: parent
                visible: root.isCurrent
                radius: width / 2
                color: Style.colors.focused
            }
        }

    }
    x: Style.size.listViewDelegateX
    width: ListView.view.width - 2 * x
    height: Style.height.base
    hoverEnabled: true

    Item {
        id: contentItemWrapper
        property int modelId: -1
        Drag.active: dragArea.drag.active
        Drag.hotSpot.x: root.width / 2
        Drag.hotSpot.y: root.height / 2
        anchors.fill: parent

        MouseArea {
            id: dragArea
            property real mapMouseY: mapToItem(treeView, Qt.point(mouseX, mouseY)).y
            anchors.fill: parent
            drag {
                target: parent
                axis: Drag.YAxis
                smoothed: false
                minimumY: -contentItemWrapper.height/2
                maximumY: treeView.height - contentItemWrapper.height/2
            }
            acceptedButtons: Qt.LeftButton
            preventStealing: true
            hoverEnabled: true

            onClicked: {
                root.treeView.currentIndex = model.index
                root.itemClicked()
            }
            onReleased: {
                if (drag.active) {
                    root.dropEvent(contentItemWrapper.Drag.target)
                }
            }
        }

        RowLayout {
            id: placeholder
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                margins: Style.margins.listViewDelegate
            }
        }
    }

    DropArea {
        id: dropArea
        property int modelId: root.modelId
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: root.verticalCenter
        }
        height: root.height / 3
    }

    states: [
        State {
            when: dragArea.drag.active
            name: "dragging"

            ParentChange {
                target: contentItemWrapper
                parent: treeView
            }
            PropertyChanges {
                target: contentItemWrapper
                opacity: 0.5
                anchors.fill: undefined
                width: root.width
                height: root.height
            }
        }
    ]
}
