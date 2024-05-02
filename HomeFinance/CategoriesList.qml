import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

FrameItem {
    id: root

    property alias model: listView.model

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
                id: categoryName
                Layout.fillWidth: true
                text: model.name
                onRename: (text) => globalController.renameCategory(text, model.id)
            }

            IconButton {
                implicitWidth: Style.width.shevron
                enabled: delegate.level > 0
                visible: delegate.hovered
                hoveredIcon: "Rename-Hover"
                normalIcon: "Rename"
                onClicked: categoryName.edited = !categoryName.edited
            }

            IconButton {
                implicitWidth: Style.width.shevron
                enabled: false
                visible: delegate.hovered
                hoveredIcon: "Delete-Hover"
                normalIcon: "Delete"
            }

            IconButton {
                implicitWidth: Style.width.shevron
                visible: delegate.hovered
                hoveredIcon: "Add-Hover"
                normalIcon: "Add"
                onClicked: {
                    addCategoryWindow.parentCategoryName = model.name
                    addCategoryWindow.parentId = model.id
                    addCategoryWindow.level = model.level + 1
                    addCategoryWindow.income = model.income
                    addCategoryWindow.show()
                }
            }
        }
    }
}
