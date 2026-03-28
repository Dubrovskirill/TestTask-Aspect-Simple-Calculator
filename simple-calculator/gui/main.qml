import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import com.calculator.backend 1.0

Window {
    id: window
    width: 360
    height: 640
    visible: true
    title: "Modern Calculator"
    color: "#1b1c21"

    CalculatorPresenter {
        id: presenter
    }

    Column {
        anchors.fill: parent
        spacing: 0


        Rectangle {
            width: parent.width
            height: parent.height * 0.35
            color: "transparent"

            Column {
                anchors.fill: parent
                anchors.margins: 25
                anchors.topMargin: 70
                spacing: 12

                RowLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 18

                    Text {
                        id: equalsSign
                        text: "="
                        color: "#51b9c1"
                        font.pixelSize: 72
                        font.weight: Font.Light

                        Layout.alignment: Qt.AlignBottom
                    }

                    Text {
                        id: resultText
                        text: presenter.display
                        color: "#ffffff"
                        font.pixelSize: 72
                        font.weight: Font.DemiBold
                        font.family: "Roboto"

                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignRight
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 45
                        maximumLineCount: 1
                        elide: Text.ElideRight
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#3a3f4b"

                }

                Text {
                    text: presenter.history
                    color: "#7e8492"
                    font.pixelSize: 21
                    anchors.right: parent.right
                    font.family: "Roboto"
                }
            }
        }

        Item {
            width: parent.width
            height: parent.height * 0.65

            GridLayout {
                id: buttonGrid
                anchors.fill: parent
                anchors.margins: 15
                columns: 4
                rows: 5
                rowSpacing: 13
                columnSpacing: 13

                function isOperator(val) {
                    return ["CE", "+/-", "%", "/", "*", "-", "+", "="].indexOf(val) !== -1
                }

                Repeater {
                    model: [
                        "CE", "+/-", "%", "/",
                        "7", "8", "9", "*",
                        "4", "5", "6", "-",
                        "1", "2", "3", "+",
                        "0", ".", "=", ""
                    ]

                    CalculatorButton {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.rowSpan: modelData === "+" ? 2 : 1
                        Layout.column: modelData === "=" ? 2 : -1
                        Layout.row: modelData === "=" ? 4 : -1

                        visible: modelData !== ""
                        enabled: modelData !== ""

                        text: modelData
                        baseColor: buttonGrid.isOperator(modelData) ? "#51b9c1" : "#242933"
                        textColor: "#ffffff"
                        radius: 12
                        fontWeight: Font.Medium

                        onClicked: if (modelData !== "") presenter.processInput(modelData)
                    }
                }
            }
        }
    }
}
