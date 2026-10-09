import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
    id: pf3
    width: appwin.width
    height: appwin.height
    property alias pf3: pf3

    TableSpray {
        id: listSpray
        x: (parent.width - width) / 2
        y: -4
        width: parent.width
        height: parent.height - 36
    }

    header: Label {
        id: label0
        color: "#FFFFFF"
        text: qsTr("喷头设置")
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        padding: 10
        background: Image {
            id: label0bg
            anchors.fill: parent
            source: "img/hdb.png"
        }

//        Text {
//            id: adddevice
//            x: 9
//            y: 12
//            color: "#ffffff"
//            text: "+"
//            font.bold: true
//            font.pixelSize: 21
//        }

        Label {
            id: label
            x: 30
            y: 13
            color: "#ffffff"
            text: qsTr("添加喷头")
            font.pixelSize: 14
        }
        MouseArea {
            id: mouseArea
            x: 30
            y: 13
            width: 93
            height: 30
            Connections {
                onClicked: {
                    startScanQR("spray")
                }
            }
        }

        Label {
            id: labelDelSprinkler
            x: appwin.width - 120
            y: 13
            color: "#ffffff"
            text: qsTr("清空喷头")
            font.pixelSize: 14
        }
        MouseArea {
            id: mouseAreaDelSprinkler
            x: appwin.width - 120
            y: 13
            width: 93
            height: 30
            Connections {
                target: mouseAreaDelSprinkler
                onClicked: {
                    mw.clearSprinklers()
                }
            }
        }
    }

    Connections {
        target: mw
        onSprayScanSuccess: {
            //listSpray.devices = listdata
            listSpray.table_model2.horHeader = ["a", "b"]
            listSpray.table_model2.initData = listdata
            mw.debug("Page3 listData:" + listdata)
        }
    }
}




/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
