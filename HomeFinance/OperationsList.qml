import QtQuick

FrameItem {
    id: root

    property alias model: listView.model

    color: "#000000"

    ListView {
        id: listView
        anchors.fill: parent
        clip: true
    }
}
