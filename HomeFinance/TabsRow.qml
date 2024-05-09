import QtQuick
import QtQuick.Layouts

import HomeFinance

Row {
    id: root

    property StackLayout contents
    property alias model: repeater.model

    anchors.leftMargin: Style.margins.base
    spacing: Style.margins.base

    Repeater {
        id: repeater

        delegate: HFTabButton {
            image: "qrc:/Images/" + model.icon + ".svg"
            text: model.name
            selected: root.contents && root.contents.currentIndex === model.index
            onClicked: root.contents.currentIndex = model.index
        }
    }
}

