import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
    id: pf1
    width: appwin.width
    height: appwin.height
    property alias pf1: pf1

    TableV {
        id: listv
        width: parent.width
        height: parent.height - 36
        x: (parent.width - width) / 2
        y: -4
    }

    header: Label {
        id: label0
        color: "#FFFFFF"
        text: qsTr("系统概况")
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
        //            color: "#ffffff"
        //            text: "+"
        //            font.bold: true
        //            font.pixelSize: 21
        //        }
        Label {
            id: labelAddHost
            //x: 24
            x: 18
            y: 13
            color: "#ffffff"
            text: qsTr("添加主机")
            font.pixelSize: 14
        }
        MouseArea {
            id: mouseAreaAddHost
            x: 18
            y: 13
            width: 93
            height: 30
            Connections {
                target: mouseArea
                onClicked: {
                    mw.getAddress()
                    mw.adddevices()
                    swipeView.currentIndex = 1
                }
            }
        }

        Label {
            id: labelSearch
            x: appwin.width * 0.25
            y: 13
            color: "#ffffff"
            text: qsTr("查找")
            font.pixelSize: 14
        }
        MouseArea {
            id: mouseAreaSearch
            x: appwin.width * 0.25
            y: 13
            width: 35
            height: 35
            Connections {
                target: mouseAreaSearch
                onClicked: {
                    //mw.uiShowMsg("Search")
                    searchWindow.visible = true
                }
            }
        }



        Label {
            id: labelDLDev
            x: appwin.width - 120
            y: 13
            color: "#ffffff"
            text: qsTr("下载主机")
            font.pixelSize: 14
        }
        MouseArea {
            id: mouseAreaDLDev
            x: appwin.width - 120
            y: 10
            width: 93
            height: 30
            Connections {
                target: mouseAreaDLDev
                onClicked: {
                    mw.downloadDevList()
                }
            }
        }
    }

    Connections {
        target: mw
        onRefreshDevicesList: {
            //table_model = JSON.parse(devices).data
            listv.table_model.horHeader = ["a", "b"]
            listv.table_model.initData = devices
        }

        onUiSetPriority: {
            if (prio > 2)
            {
                labelDLDev.enabled = true;
                labelDLDev.visable = true
                mouseAreaDLDev.enabled = true
                mouseAreaDLDev.visible = true
            }
            else
            {
                mouseAreaDLDev.enabled = false
                mouseAreaDLDev.visible = false
                labelDLDev.visible = false;
                labelDLDev.enabled = false;
            }

        }
    }
}




/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
