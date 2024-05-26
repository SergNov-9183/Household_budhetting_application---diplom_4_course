import QtQuick
import QtQuick.Layouts

import HomeFinance

Item {
    id: root
    Layout.fillHeight: true
    Layout.fillWidth: true

    PeriodPicker {
        id: periodPicker
        onClicked: (periodType) => categoriesModel.setPeriodType(periodType, periodPicker.beginDate, periodPicker.endDate)
    }

    Image {
        id: analyticsImage
        anchors.centerIn: parent
        source: "qrc:/Images/FinancialAnalysis.png"
    }

    TabsRow {
        id: tabsRow
        anchors {
            left: parent.left
            top: parent.top
            margins: Style.margins.base
        }
        visible: !analyticsImage.visible
        contents: stackLayout
        model: ListModel {
            ListElement {
                name: qsTr("Общая")
                icon: "CommonAnalytics"
            }
            ListElement {
                name: qsTr("Стоимость")
                icon: "PriceAnalytics"
            }
        }
    }

    BorderLine {
        id: tabsRowLine
        anchors {
            left: parent.left
            top: tabsRow.bottom
            right: parent.right
            topMargin: Style.margins.base
        }
        visible: !analyticsImage.visible
    }

    StackLayout {
        id: stackLayout
        anchors {
            left: parent.left
            top: tabsRowLine.bottom
            right: parent.right
            bottom: controlPanel.top
        }
        visible: !analyticsImage.visible
        CommonAnalyticsSection { onShowAnalytics: analyticsImage.visible = false }
        PriceAnalyticsSection  {}
    }

    Item {
        id: controlPanel
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 2 * Style.margins.base + controlPanelRow.implicitHeight

        BorderLine {
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
        }

        RowLayout {
            id: controlPanelRow
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                margins: Style.margins.base
            }

            TextButton {
                visible: !analyticsImage.visible
                text: periodPicker.getPeriodName(categoriesModel.periodType) + ": " + categoriesModel.beginPeriodDate + " - " + categoriesModel.endPeriodDate
                onClicked: periodPicker.show(mapToItem(root, 0, 0))
            }

            Item { Layout.fillWidth: true }


            ExpandingButton {
                layoutDirection: Qt.RightToLeft
                image: "qrc:/Images/Analytics.svg"
                text: qsTr("Анализировать данные")
                onClicked: categoriesModel.analyzeData()
            }
        }
    }
}
