import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import com.calculator.backend 1.0

Window {
    id: window
    width: 360
    height: 640
    minimumWidth: 320
    minimumHeight: 520
    visible: true
    title: "Simple Calculator"

    Rectangle {
        id: root
        anchors.fill: parent

        property color bgColor: "#1a1b28"
        property color operatorBtnColor: "#52c9dc"
        property color digitBtnColor: "#1e2435"
        property color mainTextColor: "#ffffff"
        property color secondaryTextColor: "#ffffff"
        readonly property color accentColor: "#52c9dc"

        color: bgColor
        state: "dark"

        states: [
            State {
                name: "light"
                PropertyChanges {
                    target: root
                    bgColor: "#ffffff"
                    digitBtnColor: "#f2f8fc"
                    mainTextColor: "#454545"
                    secondaryTextColor: "#454545"
                }
            },
            State {
                name: "dark"
            }
        ]

        transitions: Transition {
            ColorAnimation {
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        CalculatorPresenter {
            id: presenter
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.35

                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.margins: 15
                    width: 40; height: 40
                    radius: 20
                    color: root.digitBtnColor

                    Image {
                        anchors.centerIn: parent
                        width: 24; height: 24
                        source: root.state === "dark" ? "icons/moon.svg" : "icons/sun.svg"
                        smooth: true
                        sourceSize.width: width
                        sourceSize.height: height
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.state = (root.state === "dark" ? "light" : "dark")
                    }
                }

                Column {
                    anchors.fill: parent
                    anchors.margins: 25
                    anchors.topMargin: 50
                    spacing: 12

                    RowLayout {
                        width: parent.width
                        spacing: 12

                        Text {
                            text: "="
                            color: root.accentColor
                            font.pixelSize: 40
                        }

                        Text {
                            id: resultText
                            text: presenter.display
                            color: root.mainTextColor
                            font.pixelSize: 72
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignRight
                            fontSizeMode: Text.Fit
                            minimumPixelSize: 40
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: root.digitBtnColor
                    }

                    Text {
                        property string rawHistory: presenter.history
                        text: {
                            let h = rawHistory.replace(/\*/g, "×");
                            return h.replace(/([+\-×/])/g, "<font color='" + root.accentColor + "'>$1</font>");
                        }
                        color: root.mainTextColor
                        font.pixelSize: 21
                        width: parent.width
                        textFormat: Text.RichText
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }

            GridLayout {
                id: buttonGrid
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 15
                columns: 4
                rowSpacing: 13
                columnSpacing: 13

                readonly property var operators: ["CE", "+/-", "%", "/", "*", "-", "+", "="]

                Repeater {
                    model: ["CE", "+/-", "%", "/", "7", "8", "9", "*", "4", "5", "6", "-", "1", "2", "3", "+", "0", ".", "="]

                    CalculatorButton {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.rowSpan: modelData === "+" ? 2 : 1
                        text: modelData

                        readonly property bool isOp: buttonGrid.operators.indexOf(modelData) !== -1
                        baseColor: isOp ? root.operatorBtnColor : root.digitBtnColor
                        textColor: isOp ? "#ffffff" : root.secondaryTextColor

                        onClicked: presenter.processInput(modelData)
                    }
                }
            }
        }
    }
}
