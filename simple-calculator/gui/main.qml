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
    color: "#1a1b28"

    CalculatorPresenter {
        id: presenter
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Блок отображения (35% высоты примерно)
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.35

            Column {
                anchors.fill: parent
                anchors.margins: 25
                anchors.topMargin: 50
                spacing: 12

                RowLayout {
                    width: parent.width
                    spacing: 12

                    Text {
                        id: equalsSign
                        text: "="
                        color: "#52c9dc"
                        font.pixelSize: 40
                        font.letterSpacing: -3
                        font.weight: Font.Light
                        Layout.alignment: Qt.AlignVCenter
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
                        minimumPixelSize: 40
                        maximumLineCount: 1
                        elide: Text.ElideRight
                    }
                }

                // Разделитель
                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#1e2435"
                }

                Text {
                    property string rawHistory: presenter.history
                    text: {
                        let h = rawHistory.replace(/\*/g, "×");
                        return h.replace(/([+\-×/])/g, "<font color='#52c9dc'>$1</font>");
                    }
                    color: "#ffffff"
                    font.pixelSize: 21
                    width: parent.width
                    textFormat: Text.RichText
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        // Блок кнопок
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
                model: [
                    "CE", "+/-", "%", "/",
                    "7", "8", "9", "*",
                    "4", "5", "6", "-",
                    "1", "2", "3", "+",
                    "0", ".", "="
                ]

                CalculatorButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // Логика объединения ячеек для "+"
                    Layout.rowSpan: modelData === "+" ? 2 : 1

                    text: modelData
                    baseColor: buttonGrid.operators.indexOf(modelData) !== -1 ? "#52c9dc" : "#1e2435"

                    onClicked: presenter.processInput(modelData)
                }
            }
        }
    }
}
