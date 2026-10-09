import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0

Page {
    id: pf5
    width: appwin.width
    height: appwin.height
    property alias pf5: pf5
    property alias spr16: spr16
    property alias spr15: spr15
    property alias spr14: spr14
    property alias spr13: spr13
    property alias spr12: spr12
    property alias spr11: spr11
    property alias spr10: spr10
    property alias spr9: spr9
    property alias spr8: spr8
    property alias spr7: spr7
    property alias spr6: spr6
    property alias spr5: spr5
    property alias spr4: spr4
    property alias spr3: spr3
    property alias spr2: spr2
    property alias spr1: spr1
    property bool work: true

    header: Label {
        text: qsTr("演示管理")
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
        onDeviceInfoArrive: {
            state = "State" + (NumberOfSpray - 1)
            for (var i = 0; i < sprayIds.length; i++) {
                pf5["spr" + (i + 1)].sprayId = sprayIds[i]
            }
        }
    }

    Button {
        id: button
        x: 8 * pf5.width / 360
        y: 466
        width: 150 * pf5.width / 360
        height: 40
        text: qsTr("重  启")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60
        font.bold: true
        font.pixelSize: 20
        palette {
            buttonText: "white"
        }
        Connections {
            onClicked: {
                mw.resetHost()
            }
        }
        background: Rectangle {
            color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
        }
    }

    Button {
        id: button4
        x: 201 * pf5.width / 360
        y: 466
        width: 150 * pf5.width / 360
        height: 40
        text: qsTr("停  机")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60
        font.bold: true
        font.pixelSize: 20
        palette {
            buttonText: "white"
        }
        Connections {
            onClicked: {
                work = !work
                button4.text = work ? "停  机" : "复  机"
                mw.stopWork(!work)
            }
        }
        background: Rectangle {
            color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
        }
    }

    Button {
        id: button5
        x: 8
        y: 409
        width: 150 * pf5.width / 360
        height: 40
        text: qsTr("全部喷雾")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 116
        font.bold: true
        font.pixelSize: 20
        palette {
            buttonText: "white"
        }
        Connections {
            onClicked: {
                mw.sprayAll(pf4.buttonf1.checked ? 1 : pf4.buttonf2.checked ? 4 : pf4.buttonf3.checked ? 7 : 4)
            }
        }
        background: Rectangle {
            color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
        }
    }

    Button {
        id: button8
        x: 201 * pf5.width / 360
        y: 410
        width: 150 * pf5.width / 360
        height: 40
        text: qsTr("清除喷雾")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 116
        font.bold: true
        font.pixelSize: 20
        palette {
            buttonText: "white"
        }
        Connections {
            onClicked: {
                mw.clearSpray()
            }
        }
        background: Rectangle {
            color: parent.pressed ? "#ff255f8e" : "#ffa2d6d7"
        }
    }

    Rectangle {
        id: rectangle36
        y: (pf5.height - 616) / 2 - 84
        width: 360
        height: 432
        color: "#00ffffff"
        scale: (appwin.width / appwin.height)
               < (360 / 616) ? appwin.width / 360 : appwin.height / 616 //胖?

        anchors.horizontalCenter: parent.horizontalCenter

        Image {
            id: image1
            x: 30
            y: 230
            width: 104
            height: 100
            source: "img/a.png"
            fillMode: Image.PreserveAspectFit

            Rectangle {
                id: rectangle3
                x: 58
                y: -101
                width: 7
                height: 15
                color: "#000000"
            }

            Rectangle {
                id: rectangle
                x: 60
                y: -95
                width: 3
                height: 100
                color: "#000000"
            }

            MouseArea {
                id: mouseArea
                x: 4
                y: 0
                width: 100
                height: 100
                Connections {
                    target: mouseArea
                    onReleased: mw.debugCurrentDevice()
                }

                BusyIndicator {
                    id: busyIndicator
                    x: 18
                    y: 34
                    width: 40
                    height: 40
                    running: false
                }
            }
        }

        Rectangle {
            id: rectangle2
            x: 88
            y: 132
            width: 200
            height: 10
            color: "#00000000"

            Rectangle {
                id: pow12
                x: 7
                y: 3
                width: 187
                height: 3
                color: "#000000"
            }

            Rectangle {
                id: pow13
                x: 7
                y: 3
                width: 187
                height: 3
                color: "#000000"
            }

            Rectangle {
                id: pow14
                x: 7
                y: 3
                width: 187
                height: 3
                color: "#000000"
            }

            Rectangle {
                id: pow15
                x: 7
                y: 3
                width: 187
                height: 3
                color: "#000000"
            }

            Rectangle {
                id: pow16
                x: 7
                y: 3
                width: 187
                height: 3
                color: "#000000"
            }

            Rectangle {
                id: pow17
                x: 7
                y: 3
                width: 187
                height: 3
                color: "#000000"
            }
        }

        Rectangle {
            id: rectangle35
            x: -121
            y: -144
            width: 120
            height: 120
            color: "#00000000"

            ImgSpray {
                id: spr16
                sprayId: 16
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr15
                sprayId: 15
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr14
                sprayId: 14
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr13
                sprayId: 13
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr12
                sprayId: 12
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr11
                sprayId: 11
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr10
                sprayId: 10
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr9
                sprayId: 9
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr8
                sprayId: 8
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr7
                sprayId: 7
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr6
                sprayId: 6
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr5
                sprayId: 5
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr4
                sprayId: 4
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr3
                sprayId: 3
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr2
                sprayId: 2
                x: 10
                y: 0
            }

            ImgSpray {
                id: spr1
                sprayId: 1
                x: 351
                y: 301
            }
        }
    }
    states: [
        State {
            name: "State1"

            PropertyChanges {
                target: spr16
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr15
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr13
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr12
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr11
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr10
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr9
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr8
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr7
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr6
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr5
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr4
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr3
                x: -90
                y: 10
            }

            PropertyChanges {
                target: spr2
                x: 350
                y: 302
            }

            PropertyChanges {
                target: spr1
                x: 250
                y: 302
            }
        },
        State {
            name: "State2"
            PropertyChanges {
                target: spr16
                x: -558
                y: -346
            }

            PropertyChanges {
                target: rectangle35
                scale: 100 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 252
                y: 345
            }

            PropertyChanges {
                target: spr2
                x: 354
                y: 345
            }

            PropertyChanges {
                target: spr3
                x: 457
                y: 345
            }

            PropertyChanges {
                target: spr4
                x: -558
                y: -504
            }

            PropertyChanges {
                target: spr5
                x: -465
                y: -504
            }

            PropertyChanges {
                target: spr6
                x: -374
                y: -504
            }

            PropertyChanges {
                target: spr7
                x: -149
                y: -342
            }

            PropertyChanges {
                target: spr8
                x: -16
                y: -342
            }

            PropertyChanges {
                target: spr9
                x: -273
                y: -504
            }

            PropertyChanges {
                target: spr10
                x: 131
                y: -344
            }

            PropertyChanges {
                target: spr11
                x: 239
                y: -346
            }

            PropertyChanges {
                target: spr12
                x: 346
                y: -346
            }

            PropertyChanges {
                target: spr13
                x: -273
                y: -344
            }

            PropertyChanges {
                target: spr14
                x: -374
                y: -346
            }

            PropertyChanges {
                target: spr15
                x: -465
                y: -346
            }

            PropertyChanges {
                target: pow16
                width: 217
                height: 3
            }
        },
        State {
            name: "State3"
            PropertyChanges {
                target: spr16
                x: -71
                y: 9
            }

            PropertyChanges {
                target: rectangle35
                x: -121
                y: -144
                width: 120
                height: 126
                scale: 100 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 300
                y: 344
            }

            PropertyChanges {
                target: spr2
                x: 438
                y: 344
            }

            PropertyChanges {
                target: spr3
                x: 300
                y: 515
            }

            PropertyChanges {
                target: spr4
                x: 438
                y: 515
            }

            PropertyChanges {
                target: spr5
                x: 31
                y: 8
            }

            PropertyChanges {
                target: spr6
                x: -71
                y: 10
            }

            PropertyChanges {
                target: spr7
                x: -71
                y: 9
            }

            PropertyChanges {
                target: spr8
                x: -71
                y: 9
            }

            PropertyChanges {
                target: spr9
                x: -71
                y: 10
            }

            PropertyChanges {
                target: spr10
                x: -71
                y: 8
            }

            PropertyChanges {
                target: spr11
                x: -71
                y: 10
            }

            PropertyChanges {
                target: spr12
                x: -71
                y: 9
            }

            PropertyChanges {
                target: spr13
                x: -71
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -71
                y: 8
            }

            PropertyChanges {
                target: spr15
                x: -71
                y: 8
            }

            PropertyChanges {
                target: pow16
                width: 203
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 35
                y: 145
                width: 175
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 35
                y: 3
                width: 3
                height: 145
            }

            PropertyChanges {
                target: rectangle36
                width: 360
                height: 460
            }
        },
        State {
            name: "State4"
            PropertyChanges {
                target: spr16
                x: -95
                y: 9
            }

            PropertyChanges {
                target: rectangle35
                x: -200
                y: -236
                scale: 100 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 371
                y: 455
            }

            PropertyChanges {
                target: spr2
                x: 459
                y: 455
            }

            PropertyChanges {
                target: spr3
                x: 547
                y: 455
            }

            PropertyChanges {
                target: spr4
                x: 371
                y: 627
            }

            PropertyChanges {
                target: spr5
                x: 546
                y: 627
            }

            PropertyChanges {
                target: spr6
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr7
                x: -95
                y: 10
            }

            PropertyChanges {
                target: spr8
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr9
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr10
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr11
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr12
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr13
                x: -95
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr15
                x: -95
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 216
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 28
                y: 145
                width: 195
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 145
            }
        },
        State {
            name: "State5"
            PropertyChanges {
                target: spr16
                x: -95
                y: 9
            }

            PropertyChanges {
                target: rectangle35
                x: -132
                y: -234
                scale: 100 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 283
                y: 454
            }

            PropertyChanges {
                target: spr2
                x: 375
                y: 455
            }

            PropertyChanges {
                target: spr3
                x: 467
                y: 455
            }

            PropertyChanges {
                target: spr4
                x: 283
                y: 624
            }

            PropertyChanges {
                target: spr5
                x: 375
                y: 623
            }

            PropertyChanges {
                target: spr6
                x: 467
                y: 623
            }

            PropertyChanges {
                target: spr7
                x: -95
                y: 10
            }

            PropertyChanges {
                target: spr8
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr9
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr10
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr11
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr12
                x: -95
                y: 10
            }

            PropertyChanges {
                target: spr13
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr14
                x: -95
                y: 9
            }

            PropertyChanges {
                target: spr15
                x: -95
                y: 9
            }

            PropertyChanges {
                target: pow16
                width: 216
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 28
                y: 145
                width: 195
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 145
            }
        },
        State {
            name: "State6"
            PropertyChanges {
                target: spr16
                x: -123
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -165
                y: -224
                scale: 80 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 388
                y: 531
            }

            PropertyChanges {
                target: spr2
                x: 475
                y: 531
            }

            PropertyChanges {
                target: spr3
                x: 562
                y: 531
            }

            PropertyChanges {
                target: spr4
                x: 649
                y: 531
            }

            PropertyChanges {
                target: spr5
                x: 388
                y: 744
            }

            PropertyChanges {
                target: spr6
                x: 517
                y: 744
            }

            PropertyChanges {
                target: spr7
                x: 649
                y: 744
            }

            PropertyChanges {
                target: spr8
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr9
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr10
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr11
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr12
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr13
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr15
                x: -123
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 227
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 28
                y: 145
                width: 206
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 145
            }
        },
        State {
            name: "State7"
            PropertyChanges {
                target: spr16
                x: -123
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -158
                y: -216
                scale: 80 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 383
                y: 519
            }

            PropertyChanges {
                target: spr2
                x: 468
                y: 519
            }

            PropertyChanges {
                target: spr3
                x: 555
                y: 519
            }

            PropertyChanges {
                target: spr4
                x: 641
                y: 519
            }

            PropertyChanges {
                target: spr5
                x: 383
                y: 732
            }

            PropertyChanges {
                target: spr6
                x: 468
                y: 732
            }

            PropertyChanges {
                target: spr7
                x: 555
                y: 732
            }

            PropertyChanges {
                target: spr8
                x: 641
                y: 732
            }

            PropertyChanges {
                target: spr9
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr10
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr11
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr12
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr13
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr15
                x: -123
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 227
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 28
                y: 145
                width: 206
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 145
            }
        },
        State {
            name: "State8"
            PropertyChanges {
                target: spr16
                x: -123
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -61
                y: -207
                scale: 70 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 271
                y: 566
            }

            PropertyChanges {
                target: spr2
                x: 411
                y: 566
            }

            PropertyChanges {
                target: spr3
                x: 551
                y: 566
            }

            PropertyChanges {
                target: spr4
                x: 265
                y: 733
            }

            PropertyChanges {
                target: spr5
                x: 411
                y: 733
            }

            PropertyChanges {
                target: spr6
                x: 551
                y: 733
            }

            PropertyChanges {
                target: spr7
                x: 265
                y: 895
            }

            PropertyChanges {
                target: spr8
                x: 411
                y: 895
            }

            PropertyChanges {
                target: spr9
                x: 551
                y: 895
            }

            PropertyChanges {
                target: spr10
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr11
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr12
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr13
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr15
                x: -123
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 222
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 27
                y: 100
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 195
            }

            PropertyChanges {
                target: pow13
                x: 27
                y: 195
                width: 202
                height: 3
                color: "#000000"
            }
        },
        State {
            name: "State9"
            PropertyChanges {
                target: spr16
                x: -123
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -168
                y: -222
                scale: 70 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 450
                y: 592
            }

            PropertyChanges {
                target: spr2
                x: 545
                y: 592
            }

            PropertyChanges {
                target: spr3
                x: 640
                y: 592
            }

            PropertyChanges {
                target: spr4
                x: 734
                y: 592
            }

            PropertyChanges {
                target: spr5
                x: 450
                y: 759
            }

            PropertyChanges {
                target: spr6
                x: 545
                y: 759
            }

            PropertyChanges {
                target: spr7
                x: 640
                y: 759
            }

            PropertyChanges {
                target: spr8
                x: 734
                y: 759
            }

            PropertyChanges {
                target: spr9
                x: 450
                y: 922
            }

            PropertyChanges {
                target: spr10
                x: 734
                y: 922
            }

            PropertyChanges {
                target: spr11
                x: -121
                y: 10
            }

            PropertyChanges {
                target: spr12
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr13
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr15
                x: -123
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 222
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 27
                y: 100
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 195
            }

            PropertyChanges {
                target: pow13
                x: 27
                y: 195
                width: 202
                height: 3
            }
        },
        State {
            name: "State10"
            PropertyChanges {
                target: spr16
                x: -123
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -146
                y: -226
                scale: 70 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 409
                y: 600
            }

            PropertyChanges {
                target: spr2
                x: 505
                y: 600
            }

            PropertyChanges {
                target: spr3
                x: 601
                y: 600
            }

            PropertyChanges {
                target: spr4
                x: 698
                y: 600
            }

            PropertyChanges {
                target: spr5
                x: 409
                y: 766
            }

            PropertyChanges {
                target: spr6
                x: 505
                y: 766
            }

            PropertyChanges {
                target: spr7
                x: 601
                y: 766
            }

            PropertyChanges {
                target: spr8
                x: 698
                y: 766
            }

            PropertyChanges {
                target: spr9
                x: 409
                y: 928
            }

            PropertyChanges {
                target: spr10
                x: 552
                y: 928
            }

            PropertyChanges {
                target: spr11
                x: 698
                y: 928
            }

            PropertyChanges {
                target: spr12
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr13
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr15
                x: -123
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 222
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 27
                y: 100
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 195
            }

            PropertyChanges {
                target: pow13
                x: 27
                y: 195
                width: 202
                height: 3
            }
        },
        State {
            name: "State11"
            PropertyChanges {
                target: spr16
                x: -123
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -123
                y: -236
                scale: 70 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 368
                y: 616
            }

            PropertyChanges {
                target: spr2
                x: 464
                y: 616
            }

            PropertyChanges {
                target: spr3
                x: 561
                y: 616
            }

            PropertyChanges {
                target: spr4
                x: 658
                y: 616
            }

            PropertyChanges {
                target: spr5
                x: 368
                y: 783
            }

            PropertyChanges {
                target: spr6
                x: 464
                y: 783
            }

            PropertyChanges {
                target: spr7
                x: 561
                y: 783
            }

            PropertyChanges {
                target: spr8
                x: 658
                y: 783
            }

            PropertyChanges {
                target: spr9
                x: 368
                y: 945
            }

            PropertyChanges {
                target: spr10
                x: 464
                y: 945
            }

            PropertyChanges {
                target: spr11
                x: 561
                y: 945
            }

            PropertyChanges {
                target: spr12
                x: 658
                y: 945
            }

            PropertyChanges {
                target: spr13
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr14
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr15
                x: -123
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 222
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 27
                y: 100
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 195
            }

            PropertyChanges {
                target: pow13
                x: 27
                y: 195
                width: 202
                height: 3
            }
        },
        State {
            name: "State12"
            PropertyChanges {
                target: spr16
                x: -123
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -146
                y: -224
                scale: 70 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: -17
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 414
                y: 595
            }

            PropertyChanges {
                target: spr2
                x: 510
                y: 595
            }

            PropertyChanges {
                target: spr3
                x: 607
                y: 595
            }

            PropertyChanges {
                target: spr4
                x: 703
                y: 595
            }

            PropertyChanges {
                target: spr5
                x: 414
                y: 721
            }

            PropertyChanges {
                target: spr6
                x: 510
                y: 721
            }

            PropertyChanges {
                target: spr7
                x: 607
                y: 721
            }

            PropertyChanges {
                target: spr8
                x: 703
                y: 721
            }

            PropertyChanges {
                target: spr9
                x: 414
                y: 844
            }

            PropertyChanges {
                target: spr10
                x: 510
                y: 844
            }

            PropertyChanges {
                target: spr11
                x: 607
                y: 844
            }

            PropertyChanges {
                target: spr12
                x: 703
                y: 844
            }

            PropertyChanges {
                target: spr13
                x: 559
                y: 966
                width: 120
            }

            PropertyChanges {
                target: spr14
                x: -123
                y: 10
            }

            PropertyChanges {
                target: spr15
                x: -123
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 222
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 27
                y: 76
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 219
            }

            PropertyChanges {
                target: pow13
                x: 27
                y: 148
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow12
                x: 27
                y: 219
                width: 121
                height: 3
            }
        },
        State {
            name: "State13"
            PropertyChanges {
                target: spr16
                x: "-123"
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -136
                y: -200
                scale: 70 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: "-17"
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 397
                y: 555
            }

            PropertyChanges {
                target: spr2
                x: 492
                y: 555
            }

            PropertyChanges {
                target: spr3
                x: 588
                y: 555
            }

            PropertyChanges {
                target: spr4
                x: 682
                y: 555
            }

            PropertyChanges {
                target: spr5
                x: 399
                y: 680
            }

            PropertyChanges {
                target: spr6
                x: 492
                y: 680
            }

            PropertyChanges {
                target: spr7
                x: 588
                y: 680
            }

            PropertyChanges {
                target: spr8
                x: 682
                y: 680
            }

            PropertyChanges {
                target: spr9
                x: 398
                y: 803
            }

            PropertyChanges {
                target: spr10
                x: 492
                y: 803
            }

            PropertyChanges {
                target: spr11
                x: 588
                y: 803
            }

            PropertyChanges {
                target: spr12
                x: 682
                y: 803
                width: 120
            }

            PropertyChanges {
                target: spr13
                x: 397
                y: 925
            }

            PropertyChanges {
                target: spr14
                x: 682
                y: 925
            }

            PropertyChanges {
                target: spr15
                x: -121
                y: 10
            }

            PropertyChanges {
                target: pow16
                width: 222
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 27
                y: 76
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 219
            }

            PropertyChanges {
                target: pow13
                x: 27
                y: 148
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow12
                x: 27
                y: 219
                width: 202
                height: 3
            }
        },
        State {
            name: "State14"
            PropertyChanges {
                target: spr16
                x: "-123"
                y: 10
            }

            PropertyChanges {
                target: rectangle35
                x: -140
                y: -202
                scale: 70 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: "-17"
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 393
                y: 557
            }

            PropertyChanges {
                target: spr2
                x: 493
                y: 557
            }

            PropertyChanges {
                target: spr3
                x: 591
                y: 557
            }

            PropertyChanges {
                target: spr4
                x: 690
                y: 557
            }

            PropertyChanges {
                target: spr5
                x: 393
                y: 683
            }

            PropertyChanges {
                target: spr6
                x: 493
                y: 683
            }

            PropertyChanges {
                target: spr7
                x: 591
                y: 683
            }

            PropertyChanges {
                target: spr8
                x: 690
                y: 683
            }

            PropertyChanges {
                target: spr9
                x: 393
                y: 812
            }

            PropertyChanges {
                target: spr10
                x: 493
                y: 812
            }

            PropertyChanges {
                target: spr11
                x: 591
                y: 812
            }

            PropertyChanges {
                target: spr12
                x: 690
                y: 812
            }

            PropertyChanges {
                target: spr13
                x: 393
                y: 938
            }

            PropertyChanges {
                target: spr14
                x: 543
                y: 938
            }

            PropertyChanges {
                target: spr15
                x: 690
                y: 938
            }

            PropertyChanges {
                target: pow16
                width: 222
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 27
                y: 76
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 224
            }

            PropertyChanges {
                target: pow13
                x: 27
                y: 151
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow12
                x: 27
                y: 224
                width: 202
                height: 3
            }
        },
        State {
            name: "State15"
            PropertyChanges {
                target: spr16
                x: 635
                y: 1054
            }

            PropertyChanges {
                target: rectangle35
                x: -110
                y: -274
                scale: 70 / 120
            }

            PropertyChanges {
                target: image1
                x: 6
                y: 230
            }

            PropertyChanges {
                target: pow17
                x: "-17"
                y: 3
                width: 211
                height: 3
            }

            PropertyChanges {
                target: spr1
                x: 347
                y: 682
            }

            PropertyChanges {
                target: spr2
                x: 443
                y: 682
            }

            PropertyChanges {
                target: spr3
                x: 539
                y: 682
            }

            PropertyChanges {
                target: spr4
                x: 635
                y: 682
            }

            PropertyChanges {
                target: spr5
                x: 347
                y: 806
            }

            PropertyChanges {
                target: spr6
                x: 443
                y: 806
            }

            PropertyChanges {
                target: spr7
                x: 539
                y: 806
            }

            PropertyChanges {
                target: spr8
                x: 635
                y: 806
            }

            PropertyChanges {
                target: spr9
                x: 347
                y: 930
            }

            PropertyChanges {
                target: spr10
                x: 443
                y: 930
            }

            PropertyChanges {
                target: spr11
                x: 539
                y: 930
            }

            PropertyChanges {
                target: spr12
                x: 635
                y: 930
            }

            PropertyChanges {
                target: spr13
                x: 347
                y: 1054
            }

            PropertyChanges {
                target: spr14
                x: 443
                y: 1054
            }

            PropertyChanges {
                target: spr15
                x: 539
                y: 1054
            }

            PropertyChanges {
                target: pow16
                width: 222
                height: 3
            }

            PropertyChanges {
                target: pow15
                x: 27
                y: 76
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow14
                x: 27
                y: 3
                width: 3
                height: 219
            }

            PropertyChanges {
                target: pow13
                x: 27
                y: 148
                width: 202
                height: 3
            }

            PropertyChanges {
                target: pow12
                x: 27
                y: 220
                width: 202
                height: 3
            }
        }
    ]
}




/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
