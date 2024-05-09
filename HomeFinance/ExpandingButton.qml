import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

AbstractButton {
    id: root

    property alias image: imageIcon.source
    property alias layoutDirection: rowLayout.layoutDirection
    readonly property real contentWidth: internal.thinMargin + imageIcon.width  + label.implicitWidth + 2 * Style.margins.base
    property bool expanded: root.hovered
    property real expandedWidth: contentWidth
    property bool selected: false

    implicitHeight: Style.height.base

    QtObject {
        id: internal

        readonly property real thinMargin: (Style.height.base - Style.size.icon) / 2
        readonly property real wideMargin: root.expanded ? Style.margins.base : thinMargin
        readonly property real isLeftToRight: rowLayout.layoutDirection === Qt.LeftToRight
        readonly property real leftMargin: isLeftToRight ? thinMargin : wideMargin
        readonly property real rightMargin: isLeftToRight ? wideMargin : thinMargin

        property color borderColor
    }

    background: Rectangle {
        border {
            width: Style.width.border
            color: internal.borderColor
        }
        radius: Style.radius.base
        color: root.hovered ? Style.expandingButton.hoveredColor : "transparent"

        Rectangle {
            anchors {
                verticalCenter: parent.verticalCenter
                right: internal.isLeftToRight ? parent.left : undefined
                left: internal.isLeftToRight ? undefined : parent.right
                margins: 2 * Style.width.border
            }
            height: root.height - Style.margins.base
            width: 3
            radius: width / 2
            color: Style.expandingButton.selectedColor
            visible: root.selected
        }

        RowLayout {
            id: rowLayout
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: internal.leftMargin
                right: parent.right
                rightMargin: internal.rightMargin
            }
            spacing: Style.margins.base

            Image { id: imageIcon }

            HFLabel {
                id: label
                Layout.fillWidth: true
                text: root.text
                visible: root.expanded
            }
        }
    }

    states: [
        State {
            name: "Expanded"
            when: root.expanded
            PropertyChanges { target: root;     implicitWidth: expandedWidth }
            PropertyChanges { target: label;    opacity: 1 }
            PropertyChanges { target: internal; borderColor: Style.expandingButton.borderColor }
        },
        State {
            name: "NotExpanded"
            when: !root.expanded
            PropertyChanges { target: root;     implicitWidth: Style.height.base }
            PropertyChanges { target: label;    opacity: 0 }
            PropertyChanges { target: internal; borderColor: "transparent" }
        }
    ]
    transitions: [
        Transition {
            from: "NotExpanded"
            SequentialAnimation{
                ParallelAnimation {
                    ColorAnimation  { target: internal; duration: 2 * Style.animationDuration; property: "borderColor"  }
                    NumberAnimation { target: root;     duration: Style.animationDuration;     property: "implicitWidth" }
                }
                NumberAnimation { target: label; duration: Style.animationDuration; property: "opacity" }
            }
        },
        Transition {
            from: "Expanded"
            SequentialAnimation{
                NumberAnimation { target: label; duration: Style.animationDuration; property: "opacity" }
                ParallelAnimation {
                    ColorAnimation  { target: internal; duration: 2 * Style.animationDuration; property: "borderColor"  }
                    NumberAnimation { target: root;     duration: Style.animationDuration;     property: "implicitWidth" }
                }
            }
        }
    ]
}
