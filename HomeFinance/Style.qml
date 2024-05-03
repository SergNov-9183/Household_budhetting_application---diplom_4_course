pragma Singleton
import QtQuick

QtObject {
    readonly property QtObject colors: QtObject {
        readonly property color backgroundWindow: "#1B2127"
        readonly property color background: "#292B2F"
        readonly property color hovered: "#292B2F"
        readonly property color selected: "#2A2D31"
        readonly property color font: "#FFFFFF"
        readonly property color lineColor: "#393939"
        readonly property color frameBorder: "#1D1D1D"
        readonly property color selectedBackground: "#2A333D"
        readonly property color hoveredBackground: "#25292E"
        readonly property color frameBackground: "#292C2E"//14171A"
        readonly property color focused: "#40BDFF"
    }

    readonly property QtObject fonts: QtObject {
        readonly property string fontName: 'Roboto' //'Manrope'

        readonly property font n14: Qt.font({
            family: fontName,
            weight: Font.Normal,
            pixelSize: 14
        })
        readonly property font b14: Qt.font({
            family: fontName,
            weight: Font.Bold,
            pixelSize: 14
        })
        readonly property font n16: Qt.font({
            family: fontName,
            weight: Font.Normal,
            pixelSize: 16
        })
        readonly property font b16: Qt.font({
            family: fontName,
            weight: Font.Bold,
            pixelSize: 16
        })
    }

    readonly property QtObject height: QtObject {
        readonly property real base: 32
    }

    readonly property QtObject width: QtObject {
        readonly property real base: 32
        readonly property real shevron: 12
        readonly property real border: 1
    }

    readonly property QtObject size: QtObject {
        readonly property real icon: 24
        readonly property real listViewDelegateX: 8
        readonly property real photo: 60
    }

    readonly property QtObject opacity: QtObject {
        readonly property real enabled: 1
        readonly property real disabled: 0.4
    }

    readonly property QtObject radius: QtObject {
        readonly property real base: 3
        readonly property real window: 7
    }

    readonly property QtObject margins: QtObject {
        readonly property real base: 10
        readonly property real listViewDelegate: 5
    }
}
