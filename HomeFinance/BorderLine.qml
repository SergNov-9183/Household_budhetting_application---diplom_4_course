import QtQuick

import HomeFinance

Rectangle {
    id: root

    property bool isHorizontal: true

    anchors {
        leftMargin: isHorizontal ? Style.margins.base : undefined
        topMargin: isHorizontal ? undefined : Style.margins.base
        rightMargin: isHorizontal ? Style.margins.base : undefined
        bottomMargin: isHorizontal ? undefined : Style.margins.base
    }
    height: Style.width.border
    width: Style.width.border
    color: Style.colors.lineColor
}
