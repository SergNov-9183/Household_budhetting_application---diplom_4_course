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

            property string name: model.name
            property int nodeId: model.id
            property int level: model.level
            property int type: model.type
            property int parentId: model.parentId
            property bool isExpanded: model.isExpanded
            property bool hasChildren: model.hasChildren
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
                enabled: delegate.hasChildren
                iconName: "Shevron-" + (delegate.isExpanded  ? "Down" : "Right")
                onClicked: root.model.shevronClisked(delegate.nodeId)
            }

            EditLabel {
                id: nameEditor
                Layout.fillWidth: true
                text: delegate.name
                onRename: (text) => globalController.renameAccount(text, delegate.nodeId)
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                visible: delegate.hovered && !nameEditor.edited && delegate.level > 0
                iconName: "Rename"
                onClicked: nameEditor.edited = !nameEditor.edited
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                enabled: false
                visible: delegate.hovered && !nameEditor.edited && delegate.level > 0
                iconName: "Remove"
            }

            IconButton {
                implicitWidth: delegate.iconWidth
                visible: delegate.hovered && delegate.level < 1
                iconName: "Append"
                onClicked: globalController.appendAccount(delegate.name, delegate.type, delegate.nodeId)
            }

            Connections {
                target: accountsModel
                function onStartRename(id) {
                    if (id === delegate.nodeId) {
                        delegate.startEditing()
                    }
                }
            }
        }
    }
}
