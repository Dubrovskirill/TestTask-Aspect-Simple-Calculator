import QtQuick 2.15

Rectangle {
    id: control

    property string text: ""
    property color baseColor: "#2c313c"
    property color pressedColor: Qt.darker(baseColor, 1.25)
    property color textColor: "#ffffff"
    property int fontWeight: Font.Medium

    signal clicked(string val)

    radius: 12
    color: mouseArea.pressed ? pressedColor : baseColor

    Behavior on color {
        ColorAnimation { duration: 80 }
    }

    Text {
        anchors.centerIn: parent
        text: control.text
        color: control.textColor
        font.pixelSize: parent.height * 0.45
        font.family: "Roboto, Segoe UI, sans-serif"
        font.weight: control.fontWeight
        font.letterSpacing: control.text === "+" ? -2 : 0
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: control.clicked(control.text)
    }
}
