import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

FrameItem {
    id: root

    property alias model: listView.model

    signal clicked(int id)

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

            function startEditing() {
                ListView.view.currentIndex = model.index
                nameEditor.startEditing()
            }

            modelId: model.id
            acceptDrop: categoriesModel.canDrop(drag.source ? drag.source.modelId : -1, model.id)

            onItemClicked: root.clicked(model.id)
            onDropEvent: (target) => {
                if (target) {
                    globalController.moveCategory(target.modelId, model.id)
                }
            }

            Repeater {
                model: delegate.level
                delegate: Item { implicitWidth: Style.width.shevron }
            }

            IconButton {
                implicitWidth: Style.width.shevron
                hideIconOnDisable: true
                enabled: model.hasChildren && !delegate.dragActive
                iconName: "Shevron-" + (model.isExpanded  ? "Down" : "Right")
                onClicked: root.model.shevronClisked(model.id)
            }

            EditLabel {
                id: nameEditor
                Layout.fillWidth: true
                text: model.name
                onRename: (text) => globalController.renameCategory(text, model.id)
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                visible: delegate.hovered && !nameEditor.edited && delegate.level > 0 && !delegate.dragActive
                iconName: "Rename"
                onClicked: delegate.startEditing()
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                enabled: false
                visible: delegate.hovered && !nameEditor.edited && delegate.level > 0 && !delegate.dragActive
                iconName: "Remove"
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                visible:  delegate.hovered && !nameEditor.edited && !delegate.dragActive
                iconName: "Append"
                onClicked: globalController.appendCategory(model.name, model.income,  model.id)
            }

            Connections {
                target: categoriesModel
                function onStartRename(id) {
                    if (id === model.id) {
                        delegate.startEditing()
                    }
                }
            }
        }
    }
}
