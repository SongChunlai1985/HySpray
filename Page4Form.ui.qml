import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0

Page {
    id: pf4
    width: appwin.width
    height: appwin.height
    property alias buttons16: buttons16
    property alias buttons15: buttons15
    property alias buttons14: buttons14
    property alias buttons13: buttons13
    property alias buttons12: buttons12
    property alias buttons11: buttons11
    property alias buttons10: buttons10
    property alias buttons9: buttons9
    property alias buttons8: buttons8
    property alias buttons7: buttons7
    property alias buttons6: buttons6
    property alias buttons5: buttons5
    property alias buttons4: buttons4
    property alias buttons3: buttons3
    property alias buttons2: buttons2
    property alias buttons1: buttons1

    property alias buttonf3: buttonf3
    property alias buttonf2: buttonf2
    property alias buttonf1: buttonf1
    property alias button2: button2
    property alias pf4: pf4

    property bool b2: true
    property bool b3: false
    property bool b4: true
    property bool b5: false
    property bool b6: true
    property bool b7: false

    property string t03: ""
    property string t04: ""
    property string t05: ""
    property string t06: ""
    property string t13: ""
    property string t14: ""
    property string t15: ""
    property string t16: ""
    property string t23: ""
    property string t24: ""
    property string t25: ""
    property string t26: ""

    property int numplan: 0

    property string str: ""
    property string currentTextField: ""

    property int numOfSpray: 0
     property bool returnVal: true

    header: Label {
        text: qsTr("喷雾方案")
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        padding: 10
        color: "#FFFFFF"
        background: Image {
            id: label0bg
            anchors.fill: parent
            source: "img/hdb.png"
        }
    }

    Connections {
        target: mw
        onShowPlan: {
            numplan = num
            buttonf1.checked = false
            buttonf2.checked = false
            buttonf3.checked = false
            if (num == 0)
            {
                buttonf1.checked = true
            }
            if (num == 1)
            {
                buttonf2.checked = true
            }
            if (num == 2)
            {
                buttonf3.checked = true
            }

            button2.checked = pb2
            button3.checked = pb3
            button4.checked = pb4
            button5.checked = pb5
            button6.checked = pb6
            button7.checked = pb7

            element0v3.text = pt03.substring(0, 5)
            element0v4.text = pt04.substring(0, 5)
            element0v5.text = pt05.substring(0, 5)
            element0v6.text = pt06.substring(0, 5)
            element1v3.text = pt13.substring(0, 5)
            element1v4.text = pt14.substring(0, 5)
            element1v5.text = pt15.substring(0, 5)
            element1v6.text = pt16.substring(0, 5)
            element2v3.text = pt23.substring(0, 5)
            element2v4.text = pt24.substring(0, 5)
            element2v5.text = pt25.substring(0, 5)
            element2v6.text = pt26.substring(0, 5)

            buttons1.checked = s00
            buttons2.checked = s01
            buttons3.checked = s02
            buttons4.checked = s03
            buttons5.checked = s04
            buttons6.checked = s05
            buttons7.checked = s06
            buttons8.checked = s07

            buttons9.checked = s10
            buttons10.checked = s11
            buttons11.checked = s12
            buttons12.checked = s13
            buttons13.checked = s14
            buttons14.checked = s15
            buttons15.checked = s16
            buttons16.checked = s17
        }
        onDeviceInfoArrive: {
            numOfSpray = NumberOfSpray
            for (var i = 1; i < 16 + 1; i++) {
                pf4["buttons" + i].y = -1600
            }
            for (var j = 1; j < numOfSpray + 1; j++) {
                pf4["buttons" + sprayIds[j - 1]].y = sprayIds[j - 1] > 8 ? 66 : 29
            }
        }
    }

    Rectangle {
        id: rectangle
        y: 7
        width: parent.width
        height: parent.height * 515 / 616
        transformOrigin: Item.Top
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            id: groupBox1
            x: 0
            y: 0
            width: parent.width
            height: 23
            anchors.top: parent.top
            anchors.topMargin: (pf4.height - 616) / 4 + 12

            Button {
                id: buttonf1
                y: -7
                width: parent.width * 73 / 360
                height: 23
                text: qsTr("方案一")
                anchors.left: parent.left
                anchors.leftMargin: 32
                palette {
                    buttonText: "white"
                }
                Connections {
                    target: buttonf1
                    onClicked: {

                        returnVal = mw.cachePlan(
                                    numplan, button2.checked, button3.checked, button4.checked,
                                    button5.checked, button6.checked, button7.checked, element0v3.text
                                    == "" ? "" : element0v3.text, element0v4.text == "" ? "" : element0v4.text, element0v5.text == "" ? "" : element0v5.text, element0v6.text == "" ? "" : element0v6.text, element1v3.text == "" ? "" : element1v3.text, element1v4.text == "" ? "" : element1v4.text, element1v5.text == "" ? "" : element1v5.text, element1v6.text == "" ? "" : element1v6.text, element2v3.text == "" ? "" : element2v3.text, element2v4.text == "" ? "" : element2v4.text, element2v5.text == "" ? "" : element2v5.text, element2v6.text == "" ? "" : element2v6.text, buttons1.checked,
                                    buttons2.checked, buttons3.checked, buttons4.checked, buttons5.checked,
                                    buttons6.checked, buttons7.checked, buttons8.checked, buttons9.checked,
                                    buttons10.checked, buttons11.checked, buttons12.checked, buttons13.checked,
                                    buttons14.checked, buttons15.checked, buttons16.checked)
                        if (returnVal == false)
                        {
                            buttonf1.checked = false;
                            return
                        }

                        buttonf1.checked = true
                        buttonf2.checked = false
                        buttonf3.checked = false

                        numplan = 0
                        mw.getPlan(numplan)
                    }
                }
                checkable: true
                font.pixelSize: 18
                checked: true
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttonf2
                x: 128
                y: -7
                width: parent.width * 73 / 360
                height: 23
                text: qsTr("方案二")
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                palette {
                    buttonText: "white"
                }
                Connections {
                    target: buttonf2
                    onClicked: {

                        returnVal = mw.cachePlan(
                                    numplan, button2.checked, button3.checked, button4.checked,
                                    button5.checked, button6.checked, button7.checked, element0v3.text
                                    == "" ? "" : element0v3.text, element0v4.text == "" ? "" : element0v4.text, element0v5.text == "" ? "" : element0v5.text, element0v6.text == "" ? "" : element0v6.text, element1v3.text == "" ? "" : element1v3.text, element1v4.text == "" ? "" : element1v4.text, element1v5.text == "" ? "" : element1v5.text, element1v6.text == "" ? "" : element1v6.text, element2v3.text == "" ? "" : element2v3.text, element2v4.text == "" ? "" : element2v4.text, element2v5.text == "" ? "" : element2v5.text, element2v6.text == "" ? "" : element2v6.text, buttons1.checked,
                                    buttons2.checked, buttons3.checked, buttons4.checked, buttons5.checked,
                                    buttons6.checked, buttons7.checked, buttons8.checked, buttons9.checked,
                                    buttons10.checked, buttons11.checked, buttons12.checked, buttons13.checked,
                                    buttons14.checked, buttons15.checked, buttons16.checked)

                        if (returnVal == false)
                        {
                            buttonf2.checked = false;
                            return
                        }

                        buttonf1.checked = false
                        buttonf2.checked = true
                        buttonf3.checked = false

                        numplan = 1
                        mw.getPlan(numplan)
                    }
                }
                checkable: true
                font.pixelSize: 18
                checked: false
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttonf3
                x: 255
                y: -7
                width: parent.width * 73 / 360
                height: 23
                text: qsTr("方案三")
                anchors.right: parent.right
                anchors.rightMargin: 32
                palette {
                    buttonText: "white"
                }
                Connections {
                    target: buttonf3
                    onClicked: {
                        returnVal = mw.cachePlan(
                                    numplan, button2.checked, button3.checked, button4.checked,
                                    button5.checked, button6.checked, button7.checked, element0v3.text
                                    == "" ? "" : element0v3.text, element0v4.text == "" ? "" : element0v4.text, element0v5.text == "" ? "" : element0v5.text, element0v6.text == "" ? "" : element0v6.text, element1v3.text == "" ? "" : element1v3.text, element1v4.text == "" ? "" : element1v4.text, element1v5.text == "" ? "" : element1v5.text, element1v6.text == "" ? "" : element1v6.text, element2v3.text == "" ? "" : element2v3.text, element2v4.text == "" ? "" : element2v4.text, element2v5.text == "" ? "" : element2v5.text, element2v6.text == "" ? "" : element2v6.text, buttons1.checked,
                                    buttons2.checked, buttons3.checked, buttons4.checked, buttons5.checked,
                                    buttons6.checked, buttons7.checked, buttons8.checked, buttons9.checked,
                                    buttons10.checked, buttons11.checked, buttons12.checked, buttons13.checked,
                                    buttons14.checked, buttons15.checked, buttons16.checked)
                        if (returnVal == false)
                        {
                            buttonf3.checked = false;
                            return
                        }

                        buttonf1.checked = false
                        buttonf2.checked = false
                        buttonf3.checked = true

                        numplan = 2
                        mw.getPlan(numplan)
                    }
                }
                checkable: true
                font.pixelSize: 18
                checked: false
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }
        }

        Rectangle {
            id: groupBox3
            x: 0
            width: parent.width
            height: 330
            anchors.top: groupBox1.bottom
            anchors.topMargin: (pf4.height - 616) / 4
            Text {
                x: 33
                y: 3
                text: qsTr("时间计划")
                font.pixelSize: 18
            }

            Button {
                id: button0
                x: 123 * pf4.width / 360
                y: 5
                width: 60 * pf4.width / 360
                height: 23
                text: qsTr("复 制")
                palette {
                    buttonText: "white"
                }
                font.pixelSize: 16
                Connections {
                    onClicked: {
                        b2 = button2.checked
                        b3 = button3.checked
                        b4 = button4.checked
                        b5 = button5.checked
                        b6 = button6.checked
                        b7 = button7.checked

                        t03 = element0v3.text
                        t04 = element0v4.text
                        t05 = element0v5.text
                        t06 = element0v6.text
                        t13 = element1v3.text
                        t14 = element1v4.text
                        t15 = element1v5.text
                        t16 = element1v6.text
                        t23 = element2v3.text
                        t24 = element2v4.text
                        t25 = element2v5.text
                        t26 = element2v6.text
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff46a385" : "#ffa2d6d7"
                }
            }

            Button {
                id: button1
                x: 198 * pf4.width / 360
                y: 5
                width: 60 * pf4.width / 360
                height: 23
                text: qsTr("粘 贴")
                palette {
                    buttonText: "white"
                }
                font.pixelSize: 16
                Connections {
                    onClicked: {
                        button2.checked = b2
                        button3.checked = b3
                        button4.checked = b4
                        button5.checked = b5
                        button6.checked = b6
                        button7.checked = b7

                        element0v3.text = t03
                        element0v4.text = t04
                        element0v5.text = t05
                        element0v6.text = t06
                        element1v3.text = t13
                        element1v4.text = t14
                        element1v5.text = t15
                        element1v6.text = t16
                        element2v3.text = t23
                        element2v4.text = t24
                        element2v5.text = t25
                        element2v6.text = t26
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff46a385" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttonc
                x: 273 * pf4.width / 360
                y: 5
                width: 60 * pf4.width / 360
                height: 23
                text: qsTr("清 除")
                palette {
                    buttonText: "white"
                }
                font.pixelSize: 16
                Connections {
                    onClicked: {
                        button2.checked = false
                        button3.checked = true
                        button4.checked = false
                        button5.checked = true
                        button6.checked = false
                        button7.checked = true

                        element0v3.text = ""
                        element0v4.text = ""
                        element0v5.text = ""
                        element0v6.text = ""
                        element1v3.text = ""
                        element1v4.text = ""
                        element1v5.text = ""
                        element1v6.text = ""
                        element2v3.text = ""
                        element2v4.text = ""
                        element2v5.text = ""
                        element2v6.text = ""
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff46a385" : "#ffa2d6d7"
                }
            }

            Text {
                id: element9
                x: 38
                y: 144
                text: qsTr("时间段一:")
                font.pixelSize: 16
            }

            TextField {
                id: element0v3
                x: 122 * pf4.width / 360
                y: 138
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                visible: true
                placeholderText: qsTr("启动时间")
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element0v3
                    onReleased: {
                        currentTextField = "element0v3"
                        timeselecter.settime(element0v3.text)
                        pf4.state = "State1"
                    }
                }
            }

            Text {
                id: element10
                x: 38
                y: 214
                width: 83.925
                height: 23
                text: qsTr("时间段二:")
                font.pixelSize: 16
            }

            TextField {
                id: element1v3
                x: 122 * pf4.width / 360
                y: 209
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "启动时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element1v3
                    onReleased: {
                        currentTextField = "element1v3"
                        timeselecter.settime(element1v3.text)
                        pf4.state = "State1"
                    }
                }
            }

            Text {
                id: element11
                x: 38
                y: 285
                text: qsTr("时间段三:")
                font.pixelSize: 16
            }

            TextField {
                id: element2v3
                x: 122 * pf4.width / 360
                y: 281
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "启动时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element2v3
                    onReleased: {
                        currentTextField = "element2v3"
                        timeselecter.settime(element2v3.text)
                        pf4.state = "State1"
                    }
                }
            }

            TextField {
                id: element0v4
                x: 233 * pf4.width / 360
                y: 138
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "结束时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element0v4
                    onReleased: {
                        currentTextField = "element0v4"
                        timeselecter.settime(element0v4.text)
                        pf4.state = "State1"
                    }
                }
            }

            TextField {
                id: element1v4
                x: 233 * pf4.width / 360
                y: 209
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "结束时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element1v4
                    onReleased: {
                        currentTextField = "element1v4"
                        timeselecter.settime(element1v4.text)
                        pf4.state = "State1"
                    }
                }
            }

            TextField {
                id: element2v4
                x: 233 * pf4.width / 360
                y: 281
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "结束时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element2v4
                    onReleased: {
                        currentTextField = "element2v4"
                        timeselecter.settime(element2v4.text)
                        pf4.state = "State1"
                    }
                }
            }

            Text {
                id: element12
                x: 210 * pf4.width / 360
                y: 145
                text: qsTr("至")
                font.pixelSize: 16
            }

            Text {
                id: element13
                x: 210 * pf4.width / 360
                y: 214
                width: 16
                height: 25
                text: qsTr("至")
                font.pixelSize: 16
            }

            Text {
                id: element14
                x: 210 * pf4.width / 360
                y: 286
                text: qsTr("至")
                font.pixelSize: 16
            }

            TextField {
                id: element0v5
                x: 122 * pf4.width / 360
                y: 170
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "喷洒时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element0v5
                    onReleased: {
                        currentTextField = "element0v5"
                        minsselecter.settime(element0v5.text, element0v5.x,
                                             element0v5.y - 15)
                        pf4.state = "State2"
                    }
                }
            }

            TextField {
                id: element1v5
                x: 122 * pf4.width / 360
                y: 241
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "喷洒时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element1v5
                    onReleased: {
                        currentTextField = "element1v5"
                        minsselecter.settime(element1v5.text, element1v5.x,
                                             element1v5.y - 15)
                        pf4.state = "State2"
                    }
                }
            }

            TextField {
                id: element2v5
                x: 122 * pf4.width / 360
                y: 311
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "喷洒时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element2v5
                    onReleased: {
                        currentTextField = "element2v5"
                        minsselecter.settime(element2v5.text, element2v5.x,
                                             element2v5.y - 15)
                        pf4.state = "State2"
                    }
                }
            }

            TextField {
                id: element0v6
                x: 233 * pf4.width / 360
                y: 170
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "间隔时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element0v6
                    onReleased: {
                        currentTextField = "element0v6"
                        timeselecter.settime(element0v6.text)
                        pf4.state = "State1"
                    }
                }
            }

            TextField {
                id: element1v6
                x: 233 * pf4.width / 360
                y: 241
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "间隔时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element1v6
                    onReleased: {
                        currentTextField = "element1v6"
                        timeselecter.settime(element1v6.text)
                        pf4.state = "State1"
                    }
                }
            }

            TextField {
                id: element2v6
                x: 233 * pf4.width / 360
                y: 311
                width: 80 * pf4.width / 360
                height: 26
                font.letterSpacing: 1
                placeholderText: "间隔时间"
                font.pixelSize: 15
                readOnly: true
                Connections {
                    target: element2v6
                    onReleased: {
                        currentTextField = "element2v6"
                        timeselecter.settime(element2v6.text)
                        pf4.state = "State1"
                    }
                }
            }

            Text {
                id: element5
                x: 38
                y: 103
                text: qsTr("喷洒模式:")
                font.pixelSize: 16
            }

            Button {
                id: button6
                x: 122 * pf4.width / 360
                y: 101
                width: 80 * pf4.width / 360
                height: 23
                text: qsTr("定时")
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                Connections {
                    target: button6
                    onCheckedChanged: {
                        button7.checked = !button6.checked
                        if (button6.checked) {
                            element12.visible = false
                            element13.visible = false
                            element14.visible = false
                            element0v4.visible = false
                            element0v6.visible = false
                            element1v4.visible = false
                            element1v6.visible = false
                            element2v4.visible = false
                            element2v6.visible = false
                        }
                    }
                }
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: button7
                x: 234 * pf4.width / 360
                y: 101
                width: 80 * pf4.width / 360
                height: 23
                text: qsTr("循环")
                palette {
                    buttonText: "white"
                }
                checked: true
                font.pixelSize: 16

                Connections {
                    target: button7
                    onCheckedChanged: {
                        button6.checked = !button7.checked
                        if (button7.checked) {
                            element12.visible = true
                            element13.visible = true
                            element14.visible = true
                            element0v4.visible = true
                            element0v6.visible = true
                            element1v4.visible = true
                            element1v6.visible = true
                            element2v4.visible = true
                            element2v6.visible = true
                        }
                    }
                }
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Text {
                id: element3
                x: 38
                y: 43
                text: qsTr("喷雾模式:")
                font.pixelSize: 16
            }

            Text {
                id: element4
                x: 38
                y: 73
                text: qsTr("工作模式:")
                font.pixelSize: 16
            }

            Button {
                id: button2
                x: 122 * pf4.width / 360
                y: 42
                width: 80 * pf4.width / 360
                height: 23
                text: qsTr("变风速 ")
                palette {
                    buttonText: "white"
                }
                Connections {
                    target: button2
                    onCheckedChanged: {
                        button3.checked = !button2.checked
                    }
                }
                checkable: true
                font.pixelSize: 16
                checked: true
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: button3
                x: 234 * pf4.width / 360
                y: 42
                width: 80 * pf4.width / 360
                height: 23
                text: qsTr("定风速")
                palette {
                    buttonText: "white"
                }
                Connections {
                    target: button3
                    onCheckedChanged: {
                        button2.checked = !button3.checked
                    }
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: button4
                x: 122 * pf4.width / 360
                y: 72
                width: 80 * pf4.width / 360
                height: 23
                text: qsTr("工作日")
                palette {
                    buttonText: "white"
                }
                Connections {
                    target: button4
                    onCheckedChanged: {
                        button5.checked = !button4.checked
                    }
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: button5
                x: 234 * pf4.width / 360
                y: 72
                width: 80 * pf4.width / 360
                height: 23
                text: qsTr("每天")
                palette {
                    buttonText: "white"
                }
                Connections {
                    target: button5
                    onCheckedChanged: {
                        button4.checked = !button5.checked
                    }
                }
                font.pixelSize: 16
                checked: true
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }
        }

        Rectangle {
            id: groupBox
            x: 0
            width: pf4.width
            height: 95
            color: "#00ffffff"
            anchors.top: groupBox3.bottom
            anchors.topMargin: (pf4.height - 616) / 4 + 9

            Label {
                x: 33
                y: 0
                text: qsTr("喷头")
                font.pixelSize: 18
            }
            Button {
                id: buttons1
                x: 22 * pf4.width / 360
                y: 29
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("01")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons2
                x: 62 * pf4.width / 360
                y: 29
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("02")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons3
                x: 103 * pf4.width / 360
                y: 29
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("03")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons4
                x: 142 * pf4.width / 360
                y: 29
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("04")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons5
                x: 182 * pf4.width / 360
                y: 29
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("05")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons6
                x: 222 * pf4.width / 360
                y: 29
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("06")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons7
                x: 262 * pf4.width / 360
                y: 29
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("07")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons8
                x: 302 * pf4.width / 360
                y: 29
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("08")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons9
                x: 22 * pf4.width / 360
                y: 66
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("09")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons10
                x: 62 * pf4.width / 360
                y: 66
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("10")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons11
                x: 103 * pf4.width / 360
                y: 66
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("11")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons12
                x: 142 * pf4.width / 360
                y: 66
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("12")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons13
                x: 182 * pf4.width / 360
                y: 66
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("13")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons14
                x: 222 * pf4.width / 360
                y: 66
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("14")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons15
                x: 262 * pf4.width / 360
                y: 66
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("15")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttons16
                x: 302 * pf4.width / 360
                y: 66
                width: 35 * pf4.width / 360
                height: 30
                text: qsTr("16")
                font.bold: true
                palette {
                    buttonText: "white"
                }
                checkable: true
                font.pixelSize: 16
                background: Rectangle {
                    color: parent.checked ? "#ff255f8e" : "#ffa2d6d7"
                }
            }
        }

        Button {
            id: button10
            x: 240
            y: 480
            width: 80 * pf4.width / 360
            height: 35
            text: qsTr("云端获取")
            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -42
            palette {
                buttonText: "white"
            }
            Connections {
                target: button10
                onClicked: {
                    mw.getIotPlan()
                }
            }
            font.pixelSize: 16
            background: Rectangle {
                color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
            }
        }

        Button {
            id: button11
            x: 138
            y: 480
            width: 80 * pf4.width / 360
            height: 35
            text: qsTr("云端提交")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -42
            palette {
                buttonText: "white"
            }
            Connections {
                target: button11
                onClicked: {
                    //mw.sumitIotPlan()
                    mw.sumitToServer(
                                numplan, button2.checked, button3.checked, button4.checked,
                                button5.checked, button6.checked, button7.checked, element0v3.text
                                == "" ? "" : element0v3.text, element0v4.text == "" ? "" : element0v4.text, element0v5.text == "" ? "" : element0v5.text, element0v6.text == "" ? "" : element0v6.text, element1v3.text == "" ? "" : element1v3.text, element1v4.text == "" ? "" : element1v4.text, element1v5.text == "" ? "" : element1v5.text, element1v6.text == "" ? "" : element1v6.text, element2v3.text == "" ? "" : element2v3.text, element2v4.text == "" ? "" : element2v4.text, element2v5.text == "" ? "" : element2v5.text, element2v6.text == "" ? "" : element2v6.text, buttons1.checked,
                                buttons2.checked, buttons3.checked, buttons4.checked, buttons5.checked,
                                buttons6.checked, buttons7.checked, buttons8.checked, buttons9.checked,
                                buttons10.checked, buttons11.checked, buttons12.checked, buttons13.checked,
                                buttons14.checked, buttons15.checked, buttons16.checked)
                }
            }
            font.pixelSize: 16
            background: Rectangle {
                color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
            }
        }


        /*
        Button {
            id: button9
            x: 185 * pf4.width / 360
            y: 480
            width: 80 * pf4.width / 360
            height: 35
            text: qsTr("获取")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -42
            palette {
                buttonText: "white"
            }
            Connections {
                target: button9
                onClicked: {
                    mw.getHostPlan()
                }
            }
            font.pixelSize: 16
            background: Rectangle {
                color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
            }
        }
        */
        Button {
            id: button8
            y: 480
            width: 80 * pf4.width / 360
            height: 35
            text: qsTr("发至主机")
            anchors.left: parent.left
            anchors.leftMargin: 39
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -42
            palette {
                buttonText: "white"
            }
            font.pixelSize: 16
            Connections {
                target: button8
                onClicked: {
                    mw.sumitPlan(
                                numplan, button2.checked, button3.checked, button4.checked,
                                button5.checked, button6.checked, button7.checked, element0v3.text
                                == "" ? "" : element0v3.text, element0v4.text == "" ? "" : element0v4.text, element0v5.text == "" ? "" : element0v5.text, element0v6.text == "" ? "" : element0v6.text, element1v3.text == "" ? "" : element1v3.text, element1v4.text == "" ? "" : element1v4.text, element1v5.text == "" ? "" : element1v5.text, element1v6.text == "" ? "" : element1v6.text, element2v3.text == "" ? "" : element2v3.text, element2v4.text == "" ? "" : element2v4.text, element2v5.text == "" ? "" : element2v5.text, element2v6.text == "" ? "" : element2v6.text, buttons1.checked,
                                buttons2.checked, buttons3.checked, buttons4.checked, buttons5.checked,
                                buttons6.checked, buttons7.checked, buttons8.checked, buttons9.checked,
                                buttons10.checked, buttons11.checked, buttons12.checked, buttons13.checked,
                                buttons14.checked, buttons15.checked, buttons16.checked)
                }
            }
            background: Rectangle {
                color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
            }
        }
    }

    TimeSelect {
        id: timeselecter
        x: 0
        y: -200
        visible: true
    }

    MinsSelect {
        id: minsselecter
        x: 0
        y: -200
        visible: true
    }

    states: [
        State {
            name: "State1"
            PropertyChanges {
                target: timeselecter
                x: timeselecter.ix + 191 / 2
                y: timeselecter.iy + 103 / 2 + 36
            }
        },
        State {
            name: "State2"
            PropertyChanges {
                target: minsselecter
                x: minsselecter.ix + 111 / 2
                y: minsselecter.iy + 103 / 2 + 36
            }
        }
    ]
}






/*##^## Designer {
    D{i:0;height:616;width:360}D{i:117;anchors_x:39}
}
 ##^##*/
