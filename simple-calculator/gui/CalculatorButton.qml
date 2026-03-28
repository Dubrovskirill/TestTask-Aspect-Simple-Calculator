import QtQuick 2.15

Rectangle {
    id: control

    property string text: ""
    property color baseColor: "#1e2435"
    property color pressedColor: Qt.lighter(baseColor, 1.2)
    property color textColor: "#ffffff"

    signal clicked(string val)

    radius: 12
    color: mouseArea.pressed ? pressedColor : baseColor

    Behavior on color {
        ColorAnimation { duration: 100 }
    }

    Text {
        anchors.centerIn: parent
        text: control.text
        color: control.textColor
        font.pixelSize: Math.min(parent.width, parent.height) * 0.35
        font.family: "Roboto, Sans-Serif"
        font.weight: Font.Medium
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: control.clicked(control.text)
    }
}
