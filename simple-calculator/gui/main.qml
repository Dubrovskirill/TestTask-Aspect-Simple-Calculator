import QtQuick 2.15
import QtQuick.Window 2.15
import com.calculator.backend 1.0

Window {
    width: 400
    height: 600
    visible: true
    title: "Calculator Test"
    color: "#1b1c1d"


    CalculatorPresenter {
        id: presenter
    }


    CalculatorButton {
        anchors.centerIn: parent
        width: 100
        height: 100
        text: "7"


        baseColor: "#242933"
        textColor: "#ffffff"
        radius: 15


        onClicked: {
            console.log("Нажата кнопка:", text)
            presenter.processInput(text)
        }
    }
}
