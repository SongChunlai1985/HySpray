import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: pf0
    width: appwin.width
    height: appwin.height
    property alias element1: element1

    Connections {
        target: mw
        onUserDataArrive: {
            textInput3.text = userName
            textInput4.text = passWord
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
    }

    Image {
        id: bg
        height: background.height
        anchors.horizontalCenter: parent.horizontalCenter
        width: background.height
        source: "img/page0.png"
    }

    Text {
        id: element
        y: -73
        width: 27
        height: 30
        color: "#ffffff"
        text: qsTr("<")
        font.family: "Tahoma"
        font.pixelSize: 20
    }

    MouseArea {
        id: mouseArea
        x: 1
        y: -83
        width: 50
        height: 50
        Connections {
            onClicked: {
                pf0.state = ""
            }
        }
    }

    Image {
        id: buttonbg
        x: 35
        y: parent.height * 398 / 616
        width: parent.width - 70
        height: 56
        source: "img/p1dl.png"
        Text {
            id: buttont
            color: "#ffffff"
            text: qsTr("登    录")
            font.bold: true
            font.pixelSize: 19
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        MouseArea {
            id: button
            anchors.fill: buttonbg
            Connections {
                onClicked: pf0.state = "State1"
                onPressedChanged: {
                    buttonbg.opacity = (buttonbg.opacity === 0.3) ? 1.0 : 0.3
                }
            }
        }
    }

    Image {
        id: button1bg
        x: 35
        y: parent.height * 482 / 616
        width: parent.width - 70
        height: 56
        visible: false
        source: "img/p1dl.png"
        Text {
            id: buttont1
            color: "#ffffff"
            text: qsTr("登    录")
            font.bold: true
            font.pixelSize: 19
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        MouseArea {
            id: button1
            visible: true
            anchors.fill: button1bg
            Connections {
                onClicked: {
                    if (mw.login(textInput3.text, textInput4.text)) {
                        pf0.y = -2000
                        swipeView.enabled = true
                        swipeView.visible = true
                    }
                }
                onPressedChanged: {
                    button1bg.opacity = (button1bg.opacity === 0.3) ? 1.0 : 0.3
                }
            }
        }
    }

    TextField {
        id: textInput3
        x: 35 + label.width
        y: parent.height * 277 / 616 - 3
        width: parent.width - 70 - 26 - label.width - 10
        height: 26
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: 35 + 13
        visible: false
        font.pixelSize: 14
        placeholderText: qsTr("请填写账号")
        background: Rectangle {
            color: "#51ffffff"
        }
    }

    TextField {
        id: textInput4
        x: button2.width - width + button2.x
        y: parent.height * 310 / 616 - 3
        width: parent.width - 70 - 26 - label.width - 10
        height: 26
        color: "#ffffff"
        anchors.right: parent.right
        anchors.rightMargin: 35 + 13
        visible: false
        font.pixelSize: 14
        echoMode: TextField.Password
        placeholderText: qsTr("请填写密码")
        background: Rectangle {
            color: "#51ffffff"
        }
    }

    TextArea {
        id: textMessage
        x: 30
        y: 220
        width: parent.width - 60
        height: 40 * 2
        color: "#ffff9000"
        font.pixelSize: 18
        font.bold: true
        readOnly: true
        background: Rectangle {
            color: blue
        }
        visible: false
        Connections {
            target: mw
            onFirstShowMessage: {
                textMessage.text = msg
                textMessage.visible = Visible
            }
        }
    }

    Image {
        id: button2bg
        x: 35
        y: parent.height * 456 / 616
        width: parent.width - 70
        height: 56

        source: "img/p1zc.png"
        Text {
            id: buttont2
            color: "#ffffff"
            text: qsTr("注    册")
            font.bold: true
            font.pixelSize: 19
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        MouseArea {
            id: button2
            anchors.fill: button2bg
            anchors.horizontalCenter: parent.horizontalCenter
            Connections {
                onClicked: {
                    appwin.register()
                }
                onPressedChanged: {
                    button2bg.opacity = (button2bg.opacity === 0.3) ? 1.0 : 0.3
                }
            }
        }
    }

    Label {
        id: label
        x: 35 + 13
        y: parent.height * 277 / 616
        color: "#ffffff"
        text: qsTr("账    号")
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.bold: true
        font.pixelSize: 16
        visible: false
    }

    Label {
        id: label1
        x: 35 + 13
        y: parent.height * 310 / 616
        color: "#ffffff"
        text: qsTr("密    码")
        verticalAlignment: Text.AlignVCenter
        font.bold: true
        font.pixelSize: 16
        visible: false
    }

    Text {
        id: element1
        x: 169
        y: 582
        width: 156
        height: 19
        color: "#00bcf2"
        text: qsTr("   ")
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        horizontalAlignment: Text.AlignRight
        font.pixelSize: 16
    }

    states: [
        State {
            name: "State1"

            PropertyChanges {
                target: label1
                visible: true
            }

            PropertyChanges {
                target: label
                visible: true
            }

            PropertyChanges {
                target: textInput3
                visible: true
            }

            PropertyChanges {
                target: textInput4
                visible: true
            }

            PropertyChanges {
                target: buttonbg
                visible: false
            }

            PropertyChanges {
                target: button2bg
                visible: false
            }

            PropertyChanges {
                target: button1bg
                visible: true
            }

            //            PropertyChanges {
            //                target: element
            //                x: 12
            //                y: 10
            //                anchors.leftMargin: 12
            //            }
            PropertyChanges {
                target: mouseArea
                x: 0
                y: 0
                anchors.leftMargin: -7
                anchors.rightMargin: -11
            }
        }
    ]
}




/*##^## Designer {
    D{i:0;height:616;width:360}D{i:3;anchors_x:27}
}
 ##^##*/
