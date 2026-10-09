import QtQuick 2.12
import EasyModel 1.0

Item {
    id: tableVroot

    width: parent.width
    height: parent.height

    property alias view: view
    property alias table_model: table_model

    EasyTableModel{
        id: table_model
    }

    TableView {
        model: table_model
        id: view
        anchors.fill: parent
        clip: true
        delegate:
            Component {
            Rectangle {
                id: dlgt
                implicitWidth: tableVroot.width/2
                implicitHeight: 110

                color:  "#ffffff"
                //border.width: 0

                Image {
                    id: box
                    anchors.fill: parent
                    source: "img/p1box.png"
                }

                Column{
                    Rectangle{
                        width: tableVroot.width/2
                        height: 28
                        color: "#00000000"
                        Text {
                            x: 4
                            y: 7
                            width: tableVroot.width/2 - 10
                            height: 35
                            text: "SN:" + table_model.initData[model.row][model.column].HYID
                            font.pixelSize: 18
                            color: "#ffffff"
                            wrapMode: Text.WrapAnywhere
                        }
                    }

                    Text {
                        x: 4
                        y: 0
                        property string addr: "地址:" + table_model.initData[model.row][model.column].Address
                        text: addr.substring(0,22)
                        width: tableVroot.width/2 - 10
                        height: 35
                        font.pixelSize: 16
                        wrapMode: Text.WrapAnywhere
                    }

                    Text {
                        x: 4
                        text: "状态:" + table_model.initData[model.row][model.column].State
                        font.pixelSize: 16
                    }

                    Text {
                        x: 4
                        text: "更新:" + table_model.initData[model.row][model.column].UpdateTime
                        font.pixelSize: 16
                    }
                    /*
                    Text {
                        text: "参数: 行=" +  model.row + ",列=" + model.column + ",索引=" + index
                        font.pixelSize: 12
                    }
*/
                }

                MouseArea{
                    anchors.fill: dlgt
                    onClicked: {
                        mw.debug("ListV.qml: " + index)
                        mw.viewDevice(table_model.initData[model.row][model.column].index)
                        gpsref = "off"
                        swipeView.currentIndex = 1
                    }
                    onPressedChanged: {
                        dlgt.opacity = (dlgt.opacity === 0.3) ? 1.0 : 0.3
                    }
                }
            }
        }
    }
}




/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
