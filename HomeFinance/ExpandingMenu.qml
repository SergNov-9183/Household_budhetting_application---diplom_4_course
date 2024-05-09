import QtQuick

import HomeFinance

MouseArea {
    id: root

    readonly property bool hovered: containsMouse || userButton.hovered || accountsButton.hovered || categoriesButton.hovered
    readonly property real expandedWidth: internal.maxWidth + rightLine.width + 2 * Style.margins.base
    property alias accountsSelected: accountsButton.selected
    property alias categoriesSelected: categoriesButton.selected
    property bool expanded: hovered

    signal userButtonClicked()
    signal accountsButtonClicked()
    signal categoriesButtonClicked()

    hoverEnabled: true

    QtObject {
        id: internal

        readonly property real maxWidth: Math.max(userButton.contentWidth, accountsButton.contentWidth, categoriesButton.contentWidth)

        property real fakeProperty
    }

    ExpandingButton {
        id: userButton
        anchors {
            top: parent.top
            right: parent.right
            margins: Style.margins.base
        }
        image: "qrc:/Images/User.svg"
        text: globalController.userName
        expandedWidth: internal.maxWidth
        expanded: root.expanded
        onClicked: userButtonClicked()
    }

    BorderLine {
        id: userArea
        anchors {
            left: parent.left
            top: userButton.bottom
            topMargin: Style.margins.base
            right: rightLine.left
        }
    }

    ExpandingButton {
        id: accountsButton
        anchors {
            top: userArea.bottom
            right: parent.right
            margins: Style.margins.base
        }
        image: "qrc:/Images/Accounts.svg"
        text: qsTr("Счета")
        expandedWidth: internal.maxWidth
        expanded: root.expanded
        onClicked: accountsButtonClicked()
    }

    ExpandingButton {
        id: categoriesButton
        anchors {
            top: accountsButton.bottom
            right: parent.right
            margins: Style.margins.base
        }
        image: "qrc:/Images/Categories.svg"
        text: qsTr("Категории")
        expandedWidth: internal.maxWidth
        expanded: root.expanded
        onClicked: categoriesButtonClicked()
    }

    BorderLine {
        id: rightLine
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        isHorizontal: false
    }

    states: [
        State {
            name: "Expanded"
            when: root.expanded
            PropertyChanges { target: root;     implicitWidth: expandedWidth }
            PropertyChanges { target: internal; fakeProperty: 1 }
        },
        State {
            name: "NotExpanded"
            when: !root.expanded
            PropertyChanges { target: root;     implicitWidth: Style.height.base + rightLine.width + 2 * Style.margins.base }
            PropertyChanges { target: internal; fakeProperty: 0 }
        }
    ]
    transitions: [
        Transition {
            from: "NotExpanded"
            SequentialAnimation{
                NumberAnimation { target: root;     duration: Style.animationDuration; property: "implicitWidth" }
                NumberAnimation { target: internal; duration: Style.animationDuration; property: "fakeProperty" }
            }
        },
        Transition {
            from: "Expanded"
            SequentialAnimation{
                NumberAnimation { target: internal; duration: Style.animationDuration; property: "fakeProperty" }
                NumberAnimation { target: root;     duration: Style.animationDuration; property: "implicitWidth" }
            }
        }
    ]
}
