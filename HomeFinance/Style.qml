pragma Singleton
import QtQuick

QtObject {
    readonly property QtObject colors: QtObject {
        readonly property color backgroundWindow: "#1B2127"
        readonly property color background: "#292B2F"
        readonly property color hovered: "#292B2F"
        readonly property color selected: "#2A2D31"
        readonly property color font: "#FFFFFF"
        readonly property color lineColor: "#393939"//"3C3935"
        readonly property color frameBorder: "#1D1D1D"//"1A1E21"
        readonly property color selectedBackground: "#2A333D"
        readonly property color hoveredBackground: "#25292E"
        readonly property color frameBackground: "#14171A"
    }

    readonly property QtObject fonts: QtObject {
        readonly property string fontName: 'Roboto' //'Manrope'

        readonly property font n14: Qt.font({
            family: fontName,
            weight: Font.Normal,
            pixelSize: 14
        })
        readonly property font n16: Qt.font({
            family: fontName,
            weight: Font.Normal,
            pixelSize: 16
        })
    }

    readonly property QtObject height: QtObject {
        readonly property real base: 24
    }

    readonly property QtObject width: QtObject {
        readonly property real base: 24
        readonly property real shevron: 12
        readonly property real frameBorder: 1
    }

    readonly property QtObject size: QtObject {
        readonly property real listViewDelegateX: 8
    }

    readonly property QtObject opacity: QtObject {
        readonly property real enabled: 1
        readonly property real disabled: 0.4
    }

    readonly property QtObject radius: QtObject {
        readonly property real base: 3
        readonly property real frameBorder: 8
    }

    readonly property QtObject margins: QtObject {
        readonly property real listViewDelegate: 5
    }
}
