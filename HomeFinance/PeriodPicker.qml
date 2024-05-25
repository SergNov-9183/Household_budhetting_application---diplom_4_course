import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

Popup {
    id: root

    signal clicked(int periodType)

    function show(position) {
        x = position.x
        y = position.y - implicitHeight
        open()
    }

    function getPeriodName(periodType) {
        switch(periodType) {
            case PeriodType.Year:   return qsTr("Текущий год")
            case PeriodType.Month:  return qsTr("Текущий месяц")
            case PeriodType.Week:   return qsTr("Текущая неделя")
            case PeriodType.Custom: return qsTr("Указанный период")
            default:                return "";
        }
    }

    implicitHeight: column.implicitHeight + 2 * Style.margins.base + topPadding + bottomPadding
    implicitWidth: 300
    background: FrameItem {}

    QtObject {
        id: internal

        property ListModel periodModel: ListModel {
            ListElement { periodType: PeriodType.Year  }
            ListElement { periodType: PeriodType.Month }
            ListElement { periodType: PeriodType.Week  }
        }
    }

    Column {
        id: column
        anchors {
            fill: parent
            margins: Style.margins.base
        }

        Repeater {
            model: internal.periodModel
            delegate: ItemDelegate {
                id: delegate
                implicitHeight: Style.height.base
                implicitWidth: column.width
                hoverEnabled: true

                background: Rectangle {
                    radius: Style.radius.base
                    color: delegate.hovered ? Style.colors.hoveredBackground : "transparent"

                    HFLabel {
                        anchors.verticalCenter: parent.verticalCenter
                        text: root.getPeriodName(model.periodType)
                    }
                }

                onClicked: {
                    close()
                    root.clicked(model.periodType)
                }
            }
        }
    }
}
