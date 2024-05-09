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

            function startEditing() {
                ListView.view.currentIndex = model.index
                nameEditor.startEditing()
            }

            Repeater {
                model: delegate.level
                delegate: Item { implicitWidth: Style.width.shevron }
            }

            IconButton {
                implicitWidth: Style.width.shevron
                hideIconOnDisable: true
                enabled: model.hasChildren
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
                visible: delegate.hovered && !nameEditor.edited && delegate.level > 0
                iconName: "Rename"
                onClicked: delegate.startEditing()
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                enabled: false
                visible: delegate.hovered && !nameEditor.edited && delegate.level > 0
                iconName: "Remove"
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                visible:  delegate.hovered && !nameEditor.edited
                iconName: "Append"
                onClicked: globalController.appendCategory(model.name, delegate.level + 1,  model.income,  model.id)
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
