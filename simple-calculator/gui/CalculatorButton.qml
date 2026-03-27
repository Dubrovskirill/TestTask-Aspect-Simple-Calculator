import QtQuick 2.15

Rectangle {
    id: control

    property string text: ""
    property color baseColor: "#2c313c"
    property color pressedColor: Qt.darker(baseColor, 1.2)
    property color textColor: "#ffffff"
    property real fontHeight: 0.35

    signal clicked(string val)

    implicitWidth: 80
    implicitHeight: 80
    radius: 12


    color: mouseArea.pressed ? pressedColor : baseColor


    Behavior on color {
        ColorAnimation { duration: 100 }
    }

    Text {
        id: buttonText
        text: control.text
        anchors.centerIn: parent
        color: control.textColor
        font.pixelSize: parent.height * fontHeight
        font.family: "Roboto, Segoe UI, sans-serif"
        font.weight: Font.Normal
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: control.clicked(control.text)
    }
}
