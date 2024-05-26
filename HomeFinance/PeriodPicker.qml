import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import HomeFinance

Popup {
    id: root

    property var beginDate
    property var endDate
    signal clicked(int periodType)
    signal monthChanged()

    function show(position) {
        calendar.setMonth(0)
        internal.x = position.x
        internal.y = position.y
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
    x: internal.x
    y: internal.y - implicitHeight
    background: FrameItem {}

    QtObject {
        id: internal

        property real x: 0
        property real y: 0

        property ListModel periodModel: ListModel {
            ListElement { periodType: PeriodType.Year  }
            ListElement { periodType: PeriodType.Month }
            ListElement { periodType: PeriodType.Week  }
        }

        function getDate(date) {
            let result = new Date()
            result.setFullYear(date.getFullYear(), date.getMonth(), date.getDate())
            result.setHours(0, 0, 0, 0)
            return result
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
                        anchors{
                            left: parent.left
                            verticalCenter: parent.verticalCenter
                            leftMargin: Style.margins.base
                        }
                        text: root.getPeriodName(model.periodType)
                    }
                }

                onClicked: {
                    close()
                    root.clicked(model.periodType)
                }
            }
        }

        Rectangle {
            id: calendar
            property var currentDate

            function setMonth(shift) {
                if (shift === 0) {
                    currentDate    = internal.getDate(new Date())
                    root.beginDate = internal.getDate(new Date())
                    root.endDate   = internal.getDate(new Date())
                }
                else {
                    let month = currentDate.getMonth() + shift
                    if (month < 0) {
                        month = 11
                        currentDate.setFullYear(currentDate.getFullYear() - 1)
                    }
                    else if (month > 11) {
                        month = 0
                        currentDate.setFullYear(currentDate.getFullYear() + 1)
                    }
                    currentDate.setMonth(month)
                }
                const months = [qsTr("Январь"), qsTr("Февраль"), qsTr("Март"), qsTr("Апрель"), qsTr("Май"), qsTr("Июнь"), qsTr("Июль"), qsTr("Август"), qsTr("Сентябрь"), qsTr("Октябрь"), qsTr("Ноябрь"), qsTr("Декабрь")]
                yearLabel.text  = currentDate.getFullYear()
                monthGrid.year  = currentDate.getFullYear()
                monthGrid.month = currentDate.getMonth()
                monthLabel.text = months[currentDate.getMonth()]
                root.monthChanged()
            }

            border {
                color: Style.colors.frameBorder
                width: Style.width.border
            }
            implicitHeight: columnCalendar.implicitHeight + 2 * Style.width.border + Style.margins.base
            implicitWidth: column.width
            radius: Style.radius.base
            color: "transparent"

            Column {
                id: columnCalendar
                anchors {
                    left: parent.left
                    top: parent.top
                    right: parent.right
                    margins: Style.width.border
                }

                AbstractButton {
                    id: customButton
                    implicitHeight: Style.height.base
                    implicitWidth: column.width
                    hoverEnabled: true

                    background: Rectangle {
                        radius: Style.radius.base
                        color: customButton.hovered ? Style.colors.hoveredBackground : "transparent"

                        HFLabel {
                            anchors{
                                left: parent.left
                                verticalCenter: parent.verticalCenter
                                leftMargin: Style.margins.base
                            }
                            text: root.getPeriodName(PeriodType.Custom)
                        }
                    }
                    onClicked: {
                        close()
                        root.clicked(PeriodType.Custom)
                    }
                }

                Item {
                    width: columnCalendar.width
                    height: Style.height.base

                    RowLayout {
                        anchors {
                            fill: parent
                            leftMargin: Style.margins.base
                            rightMargin: Style.margins.base
                        }
                        IconButton {
                            iconName: "Back"
                            onClicked: calendar.setMonth(-1)
                        }
                        Item {
                            Layout.fillWidth: true
                            implicitHeight: Style.height.base

                            Row {
                                anchors.centerIn: parent
                                spacing: Style.margins.base

                                HFLabel { id: monthLabel }
                                HFLabel { id: yearLabel  }
                            }
                        }
                        IconButton {
                            iconName: "Forward"
                            onClicked: calendar.setMonth(1)
                        }
                    }
                }

                BorderLine {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }

                DayOfWeekRow {
                    anchors {
                        left: parent.left
                        right: parent.right
                        margins: Style.margins.base
                    }
                    height: Style.height.base
                    locale: monthGrid.locale
                    delegate: HFLabel {
                        text: shortName
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                BorderLine {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }

                MonthGrid {
                    id: monthGrid
                    anchors {
                        left: parent.left
                        right: parent.right
                        margins: Style.margins.base
                    }
                    height: 5 * Style.height.base
                    locale: Qt.locale("ru_RU")
                    delegate: ItemDelegate {
                        id: monthDelegate

                        required property var model
                        property bool selected: false
                        property bool inRange: false
                        function updateSelected() {
                            let value  = internal.getDate(model.date).valueOf()
                            let eEqual = false
                            let bEqual = false
                            let isMore = false
                            let isLess = false
                            if (root.beginDate) {
                                bEqual = value === root.beginDate.valueOf()
                                isMore = value > root.beginDate.valueOf()
                            }
                            if (root.endDate) {
                                eEqual = value === root.endDate.valueOf()
                                isLess = value < root.endDate.valueOf()
                            }
                            selected = bEqual || eEqual
                            inRange = isMore && isLess;
                        }

                        hoverEnabled: true
                        background: Rectangle {
                            border {
                                width: monthDelegate.hovered ? 1 : 0
                                color: monthDelegate.hovered ? "#55A5EF" : "transparent"
                            }
                            radius: Style.radius.base
                            opacity: monthDelegate.inRange && !monthDelegate.selected ? 0.5 : 1
                            color: monthDelegate.selected || monthDelegate.inRange ? "#09345d" : "transparent"

                            HFLabel {
                                anchors.centerIn: parent
                                opacity: model.month === monthGrid.month ? 1 : 0.5
                                text: model.day
                            }
                        }
                        onClicked: {
                            let date    = internal.getDate(model.date)
                            let cValue  = date.valueOf()
                            let bValue  = root.beginDate.valueOf()
                            let eValue  = root.endDate.valueOf()
                            if (cValue < bValue) {
                                root.beginDate = date
                            }
                            else if (cValue > eValue) {
                                root.endDate = date
                            }
                            else {
                                if (cValue - bValue < eValue - cValue) {
                                    root.beginDate = date
                                }
                                else {
                                    root.endDate = date
                                }
                            }
                        }
                        Connections {
                            target: root
                            function onBeginDateChanged() { monthDelegate.updateSelected() }
                            function onEndDateChanged()   { monthDelegate.updateSelected() }
                            function onMonthChanged()     { monthDelegate.updateSelected() }
                        }
                    }
                }
            }
        }
    }
}
