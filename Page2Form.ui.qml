import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Page {
    id: pf2
    width: appwin.width
    height: appwin.height
    property alias pf2: pf2

    header: Label {
        color: "#FFFFFF"
        text: qsTr("主机配置")
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        padding: 10
        background: Image {
            id: label0bg
            anchors.fill: parent
            source: "img/hdb.png"
        }
    }

    Connections {
        target: mw
        onHostScanSuccess: {
            //主机扫描获得二维码后
            element0v.text = sn
            element1v.text = ssid
            element2v.text = pwd
            mw.debug("onHostScanSuccess")
        }

        onHostScanStop: {
            busyIndicator.running = false
        }

        onUpdatestate: {
            element3v.text = connectState
            if (gpsref == "on")
                element5v.text = gpslocation
            if (connectWifi)
                busyIndicator.running = false
        }
        onAddressArrive: {
            textField1.text = address
        }
        onDeviceInfoArrive: {
            element0v.text = sn
            element1v.text = ssid
            element2v.text = pwd
            element5v.text = MapCoordinates
            textField1.text = Address
            textInput3.text = WorkWifiSsid
            textInput2.text = WorkWifiPassword
            element0v1.text = NumberOfSpray + "个"
        }
    }

    Pane {
        //用于去除TextField的焦点
        anchors.fill: parent
        focusPolicy: Qt.ClickFocus
    }

    Rectangle {
        id: rectangle
        y: 0
        width: appwin.width
        height: appwin.height

        Rectangle {
            id: groupBox0
            width: pf2.width
            height: 70

            Image {
                id: title1
                width: pf2.width
                height: 24
                source: "img/p2title.png"
            }

            Text {
                x: 8
                color: "#ffffff"
                text: qsTr("主机信息")
                anchors.verticalCenter: title1.verticalCenter
                font.letterSpacing: 1.9
                font.bold: true
                font.pixelSize: 18
            }

            Text {
                id: element0
                x: 10
                y: 26
                text: qsTr("序列号:")
                font.pixelSize: 16
            }

            Text {
                id: element0v
                x: 80
                y: 26
                width: 180
                text: qsTr("未扫描")
                font.pixelSize: 16
            }

            Text {
                id: element1
                x: 10
                y: 46
                text: qsTr("热点名称:")
                font.pixelSize: 16
            }

            Text {
                id: element1v
                x: 80
                y: 46
                text: qsTr("未扫描")
                font.pixelSize: 16
            }

            Text {
                id: element2
                x: 10
                y: 66
                text: qsTr("热点密码:")
                font.pixelSize: 16
            }

            Text {
                id: element2v
                x: 80
                y: 66
                text: qsTr("未扫描")
                font.pixelSize: 16
            }

            Image {
                id: button2
                x: 275
                y: 46
                width: 52
                height: 26
                anchors.right: parent.right
                anchors.rightMargin: 33
                source: "img/p2button.png"
                Text {
                    text: qsTr("扫描")
                    anchors.centerIn: parent
                    font.pixelSize: 16
                    color: "#ffffff"
                }
                MouseArea {
                    id: button2m
                    anchors.fill: parent
                }
                Connections {
                    target: button2m
                    onClicked: {
                        startScanQR("host")
                    }
                    onPressedChanged: {
                        button2.opacity = (button2.opacity === 0.3) ? 1.0 : 0.3
                    }
                }
            }
        }

        Rectangle {
            id: groupBox1
            width: pf2.width
            height: 90
            anchors.top: groupBox0.bottom
            anchors.topMargin: (pf2.height - 616) / 5

            Image {
                id: title2
                width: pf2.width
                height: 24
                source: "img/p2title.png"
            }

            Text {
                x: 8
                color: "#ffffff"
                text: qsTr("连接主机Wifi热点")
                anchors.verticalCenter: title2.verticalCenter
                font.letterSpacing: 1.9
                font.bold: true
                font.pixelSize: 18
            }

            Text {
                id: element3
                x: 10
                y: 33
                text: qsTr("主机热点:")
                font.pixelSize: 16
            }

            Text {
                id: element3v
                x: 80
                y: 33
                text: qsTr("未连接")
                font.pixelSize: 16
            }

            Image {
                id: button3
                x: 275
                y: 33
                width: 52
                height: 26
                anchors.right: parent.right
                anchors.rightMargin: 33
                source: "img/p2button.png"
                Text {
                    text: qsTr("连接")
                    anchors.centerIn: parent
                    font.pixelSize: 16
                    color: "#ffffff"
                }
                MouseArea {
                    id: button3m
                    anchors.fill: parent
                }
                Connections {
                    target: button3m
                    onClicked: {
                        mw.openWifi()
                        mw.connectHostWifi()
                        busyIndicator.running = true
                    }
                    onPressedChanged: {
                        button3.opacity = (button3.opacity === 0.3) ? 1.0 : 0.3
                    }
                }
            }

            BusyIndicator {
                id: busyIndicator
                x: 281
                y: 25
                width: 40
                height: 40
                anchors.right: parent.right
                anchors.rightMargin: 39
                font.pixelSize: 8
                running: false
            }

            Button {
                id: button7
                x: 223 //* pf5.width / 360
                y: 71
                width: 52 //* pf5.width / 360
                height: 26
                text: qsTr("断开")
                anchors.right: parent.right
                anchors.rightMargin: 33
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        mw.disconnectHostWifi()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }
        }

        Rectangle {
            id: groupBox2
            width: pf2.width
            height: 170
            anchors.top: groupBox1.bottom
            anchors.topMargin: (pf2.height - 616) / 5

            Image {
                id: title3
                x: 0
                y: 0
                width: pf2.width
                height: 24
                source: "img/p2title.png"
            }

            Text {
                x: 8
                color: "#ffffff"
                text: qsTr("配置主机")
                anchors.verticalCenter: title3.verticalCenter
                font.letterSpacing: 1.9
                font.bold: true
                font.pixelSize: 18
            }

            Text {
                id: element5v
                x: 80
                y: 33
                width: 251
                height: 24
                text: qsTr("未配置")
                font.pixelSize: 16
                wrapMode: Text.Wrap
            }

            Text {
                id: element7
                x: 10
                y: 153
                text: qsTr("Wifi密码:")
                font.pixelSize: 16
            }

            TextField {
                id: textInput2
                x: 81
                y: 153
                width: pf2.width / 2
                height: 26
                placeholderText: qsTr("请填写工作环境Wifi密码")
                echoMode: TextField.Password
                font.pixelSize: 14
            }

            Text {
                id: element8
                x: 10
                y: 123
                text: qsTr("现场Wifi:")
                font.pixelSize: 16
            }

            TextField {
                id: textInput3
                x: 80
                y: 120
                width: pf2.width / 2
                height: 26
                placeholderText: qsTr("请填写工作环境Wifi")
                font.pixelSize: 14
            }

            TextField {
                id: textField1
                x: 80
                y: 58
                width: pf2.width - 80 - 33
                height: 52
                placeholderText: qsTr("请填写安装地址及位置信息")
                font.pixelSize: 16
                wrapMode: Text.Wrap
                Connections {
                    target: textField1
                    onReleased: {

                        //mw.getAddress()
                    }
                }
            }

            Text {
                id: element5
                x: 10
                y: 33
                text: qsTr("GPS坐标:")
                font.pixelSize: 16
            }

            Text {
                id: element6
                x: 10
                y: 53
                text: qsTr("安装位置:")
                font.pixelSize: 16
            }

            //            Image {
            //                id: button5
            //                x: 275
            //                y: 30
            //                width: 52
            //                height: 26
            //                anchors.right: parent.right
            //                anchors.rightMargin: 33
            //                source: "img/p2button.png"
            //                Text {
            //                    text: qsTr("提交")
            //                    font.pixelSize: 16
            //                    anchors.centerIn: parent
            //                    color: "#ffffff"
            //                }
            //                MouseArea {
            //                    id: button5m
            //                    anchors.fill: parent
            //                }
            //                Connections {
            //                    target: button5m
            //                    onClicked: {
            //                        mw.saveHostInfo(element5v.text, textField1.text,
            //                                        textInput3.text, textInput2.text)

            //                        mw.gpsDescription()
            //                        mw.postAddressInfo()
            //                    }
            //                    onPressedChanged: {
            //                        button5.opacity = (button5.opacity === 0.3) ? 1.0 : 0.3
            //                    }
            //                }
            //            }
            Image {
                id: button6
                x: 275
                y: 120
                width: 52
                height: 26
                anchors.right: parent.right
                anchors.rightMargin: 33
                source: "img/p2button.png"
                Text {
                    text: qsTr("配网")
                    font.pixelSize: 16
                    anchors.centerIn: parent
                    color: "#ffffff"
                }
                MouseArea {
                    id: button4m
                    anchors.fill: parent
                }
                Connections {
                    target: button4m
                    onClicked: {
                        mw.saveHostInfo(element5v.text, textField1.text,
                                        textInput3.text, textInput2.text)
                        mw.configWorkWifi(textInput3.text, textInput2.text)
                    }
                    onPressedChanged: {
                        button6.opacity = (button6.opacity === 0.3) ? 1.0 : 0.3
                    }
                }
            }
        }

        Rectangle {
            id: groupBox3
            width: pf2.width
            height: 80
            anchors.top: groupBox2.bottom
            anchors.topMargin: (pf2.height - 616) / 5

            Image {
                id: title4
                x: 0
                y: 2
                width: pf2.width
                height: 24
                source: "img/p2title.png"
            }

            Text {
                x: 8
                color: "#ffffff"
                text: qsTr("喷头信息")
                anchors.verticalCenter: title4.verticalCenter
                font.letterSpacing: 1.9
                font.bold: true
                font.pixelSize: 18
            }

            Text {
                id: element4
                x: 10
                y: 33
                text: qsTr("喷头数量:")
                font.pixelSize: 16
            }

            Text {
                id: element0v1
                x: 80
                y: 33
                width: 61
                height: 24
                text: qsTr("0个")
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 16
            }


            /*
            Text {
                id: element10
                x: 10
                y: 53
                text: qsTr("喷头状态:")
                font.pixelSize: 16
            }

            Text {
                id: element2v1
                x: 80
                y: 53
                width: 140
                height: 24
                text: element0v1.text == "0个" ? "--" : qsTr("正常运行")
                font.pixelSize: 16
            }
            */


            /*
            Image {
                id: button8
                x: 139
                y: 31
                width: 82
                height: 26
                anchors.right: parent.right
                anchors.rightMargin: 139
                source: "img/p2button.png"
                Text {
                    text: element0v1.text == "0个" ? qsTr("添加") : qsTr("查看")
                    anchors.verticalCenterOffset: -1
                    anchors.horizontalCenterOffset: -1
                    font.pixelSize: 16
                    anchors.centerIn: parent
                    color: "#ffffff"
                }
                MouseArea {
                    id: button6m
                    width: 46
                    anchors.rightMargin: 2
                    anchors.bottomMargin: 2
                    anchors.leftMargin: -2
                    anchors.topMargin: -2
                    anchors.fill: parent
                }
                Connections {
                    target: button6m
                    onClicked: {
                        swipeView.currentIndex = 2
                    }
                    onPressedChanged: {
                        button8.opacity = (button8.opacity === 0.3) ? 1.0 : 0.3
                    }
                }
            }   */
            Button {
                id: button8
                x: 140 //* pf5.width / 360
                y: 32
                width: 87 //* pf5.width / 360
                height: 26
                text: qsTr("加喷头")
                leftPadding: 3
                display: AbstractButton.TextBesideIcon
                transformOrigin: Item.Center //("提交")
                anchors.right: parent.right
                anchors.rightMargin: 135
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        swipeView.currentIndex = 2
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: button4
                x: 244 //* pf5.width / 360
                y: 32
                width: 86 //* pf5.width / 360
                height: 26
                text: qsTr("配喷头")
                leftPadding: 3
                display: AbstractButton.TextBesideIcon
                transformOrigin: Item.Center //("提交")
                anchors.right: parent.right
                anchors.rightMargin: 30
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        mw.sumitAllSpray()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttonReadSprikler
                x: 140 //* pf5.width / 360
                y: 72
                width: 87 //* pf5.width / 360
                height: 26
                text: qsTr("读取喷头")
                anchors.rightMargin: 135
                anchors.right: parent.right
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        mw.readSpriklers()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: button9
                x: 244 //* pf5.width / 360
                y: 72
                width: 86 //* pf5.width / 360
                height: 26
                text: qsTr("一键加水")
                anchors.right: parent.right
                anchors.rightMargin: 30
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        mw.addWater()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttonDownloadOneDev
                x: 140 //* pf5.width / 360
                y: 110
                width: 87 //* pf5.width / 360
                height: 26
                text: qsTr("云端下载")
                anchors.rightMargin: 135
                anchors.right: parent.right
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        mw.dlCurrentDevInfo()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: buttonPostDev
                x: 240 //* pf5.width / 360
                y: 110
                width: 86 //* pf5.width / 360
                height: 26
                text: qsTr("提交云端")
                anchors.right: parent.right
                anchors.rightMargin: 30
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        mw.postDeviceInfo()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }
        }

        Rectangle {
            id: groupBox4
            x: 0
            width: pf2.width
            height: 10
            anchors.topMargin: 70
            anchors.top: groupBox3.bottom

            Image {
                id: title5
                x: 0
                y: 3
                width: pf2.width
                height: 24
                source: "img/p2title.png"
            }

            Text {
                x: 8
                color: "#ffffff"
                text: qsTr("主机喷雾记录")
                anchors.verticalCenter: title5.verticalCenter
                font.letterSpacing: 1.9
                font.bold: true
                font.pixelSize: 18
            }

            Button {
                id: button11
                x: (appwin.width - 52 * 3) / 4
                y: 50
                width: 52
                height: 26
                text: qsTr("读取")
//                anchors.leftMargin: appwin.width * 0.1
//                anchors.left: parent.left
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        mw.sendOrderToHost()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: button12
                x: (appwin.width - 52 * 3) / 2 + 52
                y: 50
                width: 52
                height: 26
                text: qsTr("查看")
//                anchors.right: parent.right
//                anchors.rightMargin: appwin.width * 0.1
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        viewWindow.visible = true
                        showFilesName()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }

            Button {
                id: button13
                x: (appwin.width - 52 * 3) / 4 * 3 + 104
                y: 50
                width: 52
                height: 26
                text: qsTr("上传")
//                anchors.right: parent.right
//                anchors.rightMargin: appwin.width * 0.1
                font.pixelSize: 16
                palette {
                    buttonText: "white"
                }
                Connections {
                    onClicked: {
                        mw.uploadSprayRecord()
                    }
                }
                background: Rectangle {
                    color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
                }
            }
        }
    }
}

/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
