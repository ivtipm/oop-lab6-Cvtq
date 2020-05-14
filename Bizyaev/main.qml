import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: window
    Keys.forwardTo: [keysHandler]
    visible: true
    width: 320
    height: 472
    color: "#1f1f1f"
    title: "Калькулятор"
    minimumHeight: 472
    minimumWidth: 320

    Rectangle{
        id: keysHandler
        focus: true
        Keys.onPressed: {
                switch (event.key) {
                    case Qt.Key_Plus:  BackEndQML.submitEvent("OPER.PLUS")
                    case Qt.Key_Minus: BackEndQML.submitEvent("OPER.MINUS")
                    case Qt.Key_division: BackEndQML.submitEvent("OPER.DIV")
                    case Qt.Key_multiply: BackEndQML.submitEvent("OPER.MULTIPLY")
                    case Qt.Key_Equal: BackEndQML.submitEvent("EQUALS")
                    //default: window.title = event.key
                }
        }
        Keys.onDigit0Pressed: BackEndQML.submitEvent("DIGIT.0")
        Keys.onDigit1Pressed: BackEndQML.submitEvent("DIGIT.1")
        Keys.onDigit2Pressed: BackEndQML.submitEvent("DIGIT.2")
        Keys.onDigit3Pressed: BackEndQML.submitEvent("DIGIT.3")
        Keys.onDigit4Pressed: BackEndQML.submitEvent("DIGIT.4")
        Keys.onDigit5Pressed: BackEndQML.submitEvent("DIGIT.5")
        Keys.onDigit6Pressed: BackEndQML.submitEvent("DIGIT.6")
        Keys.onDigit7Pressed: BackEndQML.submitEvent("DIGIT.7")
        Keys.onDigit8Pressed: BackEndQML.submitEvent("DIGIT.8")
        Keys.onDigit9Pressed: BackEndQML.submitEvent("DIGIT.9")

    }
    Rectangle {
        id: resultArea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height * 3 / 8 - 10
        border.width: 0
        color: "#1f1f1f"
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        Text {
            id: result
            x: -10
            y: 0
            width: parent.width
            height: parent.height
            anchors.leftMargin: buttons.implicitMargin
            anchors.rightMargin: buttons.implicitMargin
           //anchors.fill: parent
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            text: BackEndQML.result
            color: "white"
            font.bold: true
            font.pixelSize: window.height * 3 / 32
            font.family: "Segoe UI Symbol"
            fontSizeMode: Text.Fit
        }
        Text {
            id: input
            x: -10
            y: 0
            width: parent.width
            height: parent.height
            anchors.leftMargin: buttons.implicitMargin
            anchors.rightMargin: buttons.implicitMargin
           //anchors.fill: parent
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            text: BackEndQML.input
            color: "white"
            font.bold: true
            font.pixelSize: window.height * 3 / 32
            font.family: "Segoe UI Symbol"
            fontSizeMode: Text.Fit
        }

        Text {
            id: expression
            x: 0
            y: 0
            width: parent.width
            height: parent.height / 2
            color: "#ffffff"
            text: BackEndQML.fakeExpression
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            fontSizeMode: Text.Fit
           //  anchors.fill: parent
            font.family: "Segoe UI Symbol"
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: window.height * 2.5 / 32
        }
    }

    Item {
        id: buttons
        anchors.top: resultArea.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        property real implicitMargin: {
            var ret = 0;
            for (var i = 0; i < visibleChildren.length; ++i) {
                var child = visibleChildren[i];
                ret += (child.implicitMargin || 0);
            }
            return ret / visibleChildren.length;
        }
        anchors.leftMargin: 0

        Repeater {
            id: operations
            model: ["÷", "×", "-", "+"]

            Button {
                y: (index+1) * height
                x: parent.width - width //index * width
                width: parent.width / 4
                height: parent.height / 6
                color: pressed ? "#343434" : "#131313"
                text: modelData
                fontHeight: 0.6
                fontBold: false
                onClicked: BackEndQML.submitEvent(eventName)
                property string eventName: {
                    switch (text) {
                    case "÷": return "OPER.DIV"
                    case "×": return "OPER.MULTIPLY"
                    case "+": return "OPER.PLUS"
                    case "-": return "OPER.MINUS"
                    }
                }
            }
        }

        Repeater {
            id: digits
            model: ["7", "8", "9", "4", "5", "6", "1", "2", "3", "0", ".", "C"]
            Button {
                x: (index % 3) * width
                y:  Math.floor(index / 3 + 2) * height //Math.floor(index / 3 + 1) * height
                width: parent.width / 4
                height: parent.height / 6
                color: pressed ? "#343434" : "#060606"
                text: modelData
                onClicked: BackEndQML.submitEvent(eventName)
                fontBold: true
                property string eventName: {
               switch (text) {
                     case ".": return "POINT"
                     case "C": return "C"
                     default: return "DIGIT." + text
                    }
                }
            }
        }
        Repeater {
            id: functions
            model: ["sin", "cos", "tan", "exp", "ln", "sqrt","power","(",")","e","π","","","",""]
            Button {
                x: (index % 5) * width
                y:  Math.floor(index / 5) * height
                width: parent.width / 6.665
                height: parent.height / 9
                color: pressed ? "#343434" : "#131313"
                text: modelData
                onClicked: BackEndQML.submitEvent(eventName)
                fontBold: true
                property string eventName: {
                    switch (text) {
                    case "sin":  return "TRIG.SIN"
                    case "cos":  return "TRIG.COS"
                    case "tan":  return "TRIG.TAN"
                    case "exp":  return "FUNC.EXP"
                    case "ln":   return "FUNC.LN"
                    case "sqrt": return "FUNC.SQRT"
                    case "power": return "FUNC.CARET"
                    case "(": return "BRACKET.LEFT"
                    case ")": return "BRACKET.RIGHT"
                    default: return  text     // pi, e
                    }
                }
            }
        }

        Button {
            id: backspace
            x: 3 * width
            fontHeight: 0.4
            width: parent.width / 4
            height: parent.height / 6
            color: pressed ? "#3B3B3B" : "#2C2C2C"
            text: "<"
            onClicked: BackEndQML.submitEvent("CLEAR")
        }

        Button {
            id: resultButton
            x: 3 * width
            y: parent.height - height
            fontHeight: 0.4
            width: parent.width / 4
            height: parent.height / 6
            color: pressed ? "#3B3B3B" : "#2C2C2C"
            text: "="
            onClicked: BackEndQML.submitEvent("EQUALS")
        }
    }
}
