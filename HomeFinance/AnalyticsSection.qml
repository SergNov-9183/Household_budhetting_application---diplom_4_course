import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts

import HomeFinance

Item {
    id: root

    property AnalyticsProxyModel analyticsModel: AnalyticsProxyModel {
        model: categoriesModel
        onCurrentCategoryChanged: {
            let count = rowCount();
            pieSeries.clear()
            for(let i = 0; i < count; ++i) {
                let idx = index(i, 0);
                let categoryId = data(idx, Qt.UserRole + 1)
                let name       = data(idx, Qt.DisplayRole)
                let sum        = data(idx, Qt.UserRole + 8)
                if (currentCategory === 0) {
                    title.text                    = qsTr("Доходы и расходы")
                    backButton.previousCategoryId = 0
                }
                else if (categoryId === currentCategory) {
                    title.text                    = qsTr("Категория: ") + name + " (" + sum.toFixed(2).replace(".", ",") + ")"
                    backButton.previousCategoryId = data(idx, Qt.UserRole + 4)
                    name                          = qsTr("Без категории")
                    sum                           = data(idx, Qt.UserRole + 7)
                }
                if (sum > 0) {
                    let pieSlice          = pieSeries.append(name + " (" + sum.toFixed(2).replace(".", ",") + ")", sum)
                    pieSlice.labelColor   = "#6C7783"
                    pieSlice.labelFont    = Style.fonts.b16
                    pieSlice.labelVisible = true
                    pieSlice.borderColor  = "#6C7783"
                    //pieSlice.hovered.connect((state) => { borderColor = state ? Style.colors.font : "#6C7783" })
                    Object.defineProperty(pieSlice, 'categoryId', {
                                              enumerable: false,
                                              configurable: false,
                                              writable: false,
                                              value: categoryId})
                }
                analyticsImage.visible = false
                chart.visible          = true
            }
        }
    }

    Layout.fillHeight: true
    Layout.fillWidth: true

    PeriodPicker {
        id: periodPicker
        onClicked: (periodType) => categoriesModel.setPeriodType(periodType)
    }

    Image {
        id: analyticsImage
        anchors.centerIn: parent
        source: "qrc:/Images/FinancialAnalysis.png"
    }

    Connections {
        target: categoriesModel
        function onAnalyzeDataComplited() { analyticsModel.currentCategory = 0 }
    }

    ChartView {
        id: chart
        property var sliceMap
        anchors {
            left: parent.left
            top: parent.top
            right: tableView.left
            bottom: controlPanel.top
        }
        legend.visible: false
        antialiasing: true
        backgroundColor: "transparent"
        visible: false

        PieSeries {
            id: pieSeries
            onClicked: (slice) => {
                if (slice.categoryId !== analyticsModel.currentCategory) {
                    analyticsModel.currentCategory = slice.categoryId
                }
            }
            onHovered: (slice, state) => {
                slice.borderColor = state ? Style.colors.font : "#6C7783"
                slice.borderWidth = state ? 2 : 1
                slice.labelColor  = slice.borderColor
                slice.exploded    = state
            }
        }
    }

    FrameItem {
        id: tableView
        anchors {
            top: rowTitle.bottom
            right: parent.right
            bottom: controlPanel.top
            margins: Style.margins.base
            topMargin: 2 * Style.margins.base
        }
        width: 300
        color: "#000000"
        visible: chart.visible

        ListView {
            id: listView
            anchors {
                fill: parent
                margins: 6
            }
            model: root.analyticsModel
            clip: true
            delegate: ItemDelegate {
                id: delegate

                property string name: model.id === root.analyticsModel.currentCategory
                                      ? qsTr("Без категории")
                                      : model.name ? model.name : ""
                property real sum: model.id === root.analyticsModel.currentCategory
                                   ? model.sum ? model.sum : 0.0
                                   : model.totalSum ? model.totalSum : 0.0

                width: listView.width
                height: rowLayout.implicitHeight + delegateLine.implicitHeight + 12
                background: Item {}
                RowLayout {
                    id: rowLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    spacing: 4
                    LabelButton {
                        Layout.fillWidth: true
                        text: delegate.name
                        selectable: model.id !== root.analyticsModel.currentCategory && delegate.sum > 0
                        onClicked: root.analyticsModel.currentCategory = model.id
                    }
                    HFLabel {

                        text: delegate.sum.toFixed(2).replace(".", ",")
                    }
                }
                BorderLine {
                    id: delegateLine
                    anchors {
                        left: parent.left
                        top: rowLayout.bottom
                        right: parent.right
                        leftMargin: 0
                        topMargin: 6
                        rightMargin: 0
                    }
                }
            }
        }
    }

    Row {
        id: rowTitle
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 2 * Style.margins.base
        }
        visible: chart.visible
        spacing: Style.margins.base

        IconButton {
            id: backButton

            property int previousCategoryId: 0

            iconName: "Back"
            visible: previousCategoryId !== analyticsModel.currentCategory
            onClicked: analyticsModel.currentCategory = previousCategoryId
        }
        HFLabel {
            id: title
            anchors.verticalCenter: parent.verticalCenter
        }
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
                text: periodPicker.getPeriodName(categoriesModel.periodType)
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
