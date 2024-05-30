import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts

import HomeFinance

Item {
    id: root
    Layout.fillHeight: true
    Layout.fillWidth: true

    property PriceAnalyticsProxyModel analyticsModel: PriceAnalyticsProxyModel {
        operations: operationsModel
        categories: categoriesModel
    }

    HFLabel {
        id: title
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 2 * Style.margins.base
        }
        text: qsTr("Изменение стоимости выбранной категории на протяжении выбранного периода")
    }

    CategoriesList {
        id: categoriesList
        anchors {
            left: parent.left
            top: title.bottom
            bottom: parent.bottom
            margins: Style.margins.base
            topMargin: 2 * Style.margins.base
        }
        width: 300
        model: categoriesModel.allCategories
        onClicked: (id) => analyticsModel.currentCategory = id
    }

    Item {
        id: barChart
        anchors {
            left: categoriesList.right
            top: title.bottom
            right: parent.right
            bottom: parent.bottom
            margins: 2 * Style.margins.base
        }

        HFLabel {
            anchors.centerIn: parent
            visible: barRepeater.count < 1
            text: analyticsModel.currentCategory === 0 ? qsTr("Выберите категорию") : qsTr("По данной категории нет расходов, выберите другую категорию")
        }

        Item {
            id: axisY
            property int count: 3
            property real stepY: height / (count + 1)
            anchors {
                left: parent.left
                top: parent.top
                bottom: axisX.top
                bottomMargin: -1
            }
            width: maxPrice.implicitWidth + 5 + Style.margins.base

            Rectangle {
                anchors {
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                }
                width: 1
                color: "white"
            }

            Repeater {
                model: axisY.count
                delegate: Rectangle {
                    id: dashDelegate
                    property real value: (model.index + 1) * axisY.stepY
                    anchors.right: parent.right
                    y: axisY.height - (value - width / 2)
                    x: axisY.width - width
                    width: 10
                    height: 1
                    color: "white"

                    HFLabel {
                        anchors {
                            right: dashDelegate.left
                            verticalCenter: dashDelegate.verticalCenter
                            margins: Style.margins.base
                        }
                        text: ((barCanvas.maxPrice * value) / axisY.height).toFixed(2).replace(".", ",")
                    }
                }
            }

            Rectangle {
                id: maxDash
                anchors {
                    top: parent.top
                    right: parent.right
                }
                width: 10
                height: 1
                color: "white"
            }
            HFLabel {
                id: maxPrice
                anchors {
                    right: maxDash.left
                    verticalCenter: maxDash.verticalCenter
                    margins: Style.margins.base
                }
                text: barCanvas.maxPrice.toFixed(2).replace(".", ",")
            }
            Rectangle {
                id: minDash
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                }
                width: 10
                height: 1
                color: "white"
            }
            HFLabel {
                id: minPrice
                anchors {
                    right: minDash.left
                    verticalCenter: minDash.verticalCenter
                    margins: Style.margins.base
                }
                text: "0,00"
            }
        }

        Item {
            id: axisX
            anchors {
                left: axisY.right
                right: parent.right
                bottom: parent.bottom
            }
            height: Style.height.base

            Rectangle {
                anchors {
                    left: parent.left
                    top: parent.top
                    right: parent.right
                }
                height: 1
                color: "white"
            }
        }

        Item {
            id: barCanvas
            property real stepX: width / (barRepeater.count + 1)
            property real barWidth: Math.max(2, stepX / 2)
            property real maxPrice: 10

            anchors {
                left: axisY.right
                top: parent.top
                right: parent.right
                bottom: axisX.top
            }

            Repeater {
                id: barRepeater
                model: analyticsModel
                delegate: Rectangle {
                    id: barDelegate
                    color: "#55a5ef"
                    x: (model.index + 1) * barCanvas.stepX - width / 2
                    y: barCanvas.height - height
                    width: barCanvas.barWidth
                    height: barCanvas.height * model.price / barCanvas.maxPrice

                    HFLabel {
                        anchors {
                            top: parent.top
                            horizontalCenter: parent.horizontalCenter
                            margins: Style.margins.base
                        }
                        visible: implicitWidth + 2 * Style.width.border < barDelegate.width && implicitHeight + 2 * Style.margins.base < barDelegate.height
                        text: model.price.toFixed(2).replace(".", ",")
                    }

                    HFLabel {
                        anchors {
                            top: parent.bottom
                            horizontalCenter: parent.horizontalCenter
                            margins: Style.margins.base
                        }
                        visible: implicitWidth + 2 * Style.width.border < barCanvas.stepX
                        text: model.date
                    }

                    Component.onCompleted: {
                        if (model.price > barCanvas.maxPrice) {
                            barCanvas.maxPrice = model.price
                        }
                    }
                }
                onCountChanged: {
                    if (count === 0) {
                        barCanvas.maxPrice = 10
                    }
                }
            }
        }
    }
}
