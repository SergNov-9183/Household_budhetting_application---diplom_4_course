import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

FrameItem {
    id: root

    property alias model: listView.model

    color: "#000000"

    ListView {
        id: listView
        anchors {
            fill: parent
            margins: 6
        }
        clip: true
        delegate: ListViewDelegate {
            id: delegate

            property int level: model.level
            property real iconWidth: Style.size.icon - 1.5 * contentSpacing

            Repeater {
                model: delegate.level
                delegate: Item { implicitWidth: Style.width.shevron }
            }

            IconButton {
                implicitWidth: Style.width.shevron
                hideIconOnDisable: true
                enabled: model.hasChildren
                hoveredIcon: "Shevron-" + (model.isExpanded  ? "Down" : "Right") + "-Hover"
                normalIcon: "Shevron-" + (model.isExpanded  ? "Down" : "Right")
                onClicked: root.model.shevronClisked(model.id)
            }

            EditLabel {
                id: accountName
                Layout.fillWidth: true
                text: model.name
                onRename: (text) => globalController.renameAccount(text, model.id)
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                enabled: delegate.level > 0
                visible: delegate.hovered
                hoveredIcon: "Rename-Hover"
                normalIcon: "Rename"
                onClicked: accountName.edited = !accountName.edited
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                enabled: false
                visible: delegate.hovered
                hoveredIcon: "Delete-Hover"
                normalIcon: "Delete"
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                visible: delegate.hovered
                hoveredIcon: "Add-Hover"
                normalIcon: "Add"
                onClicked: {
                    addAccountWindow.parentAccountName = model.name
                    addAccountWindow.parentId = model.id
                    addAccountWindow.type = model.type
                    addAccountWindow.show()
                }
            }
        }
    }
}
