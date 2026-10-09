import QtQuick 2.12
import EasyModel 1.0
import QtQuick.Controls 2.12

Item {
    id:tableSprayroot

    width: parent.width
    height: parent.height

    property alias view: view
    property alias table_model2: table_model2

    EasyTableModel{
        id: table_model2
    }

    TableView {
        model: table_model2
        id: view
        anchors.fill: parent
        clip: true

        delegate: Component {
            Rectangle {
                id: dlgt
                implicitWidth: tableSprayroot.width/2
                implicitHeight: 165
                color:  "#ffffff"

                Image {
                    id: box
                    anchors.fill: parent
                    source: "img/p1box.png"
                }

                Column{
                    spacing: 1
                    y: 10
                    Text {
                        width: tableSprayroot.width/2 - 6
                        height: 30
                        x: 4
                        y: 14
                        text: "SN:" + table_model2.initData[model.row][model.column].HYID
                        font.pixelSize: 18
                        color: "#ffffff"
                        wrapMode: Text.WrapAnywhere
                    }

                    Text {
                        x: 4
                        text: "安装位置:" + table_model2.initData[model.row][model.column].Position
                        font.pixelSize: 16
                    }

                    Text {
                        x: 4
                        text: "工作状态:" +table_model2.initData[model.row][model.column].State
                        font.pixelSize: 16
                    }

                    Text {
                        x: 4
                        text: "喷雾方案:" + table_model2.initData[model.row][model.column].Plan
                        font.pixelSize: 16
                    }

                    Text {
                        x: 4
                        text: "喷头编号:" + table_model2.initData[model.row][model.column].SprayID
                        font.pixelSize: 16
                    }

                    Text {
                        x: 4
                        text: "更新:" + table_model2.initData[model.row][model.column].UpdateTime
                        font.pixelSize: 16
                    }
                }

                MouseArea{
                    anchors.fill: dlgt
                    onClicked: {
                        mw.debug("TableSpray.qml: " + table_model2.initData[model.row][model.column].index)
                        swipeView.currentIndex = 3
                        mw.getPlan(0);
                    }
                    onPressedChanged: {
                        dlgt.opacity = (dlgt.opacity === 0.3) ? 1.0 : 0.3
                    }
                }

                Image {
                    id: trySpray
                    x: parent.width - width - 10
                    y: 134
                    width: 40
                    height: 24
                    source: "img/p2button.png"
                    Text {
                        text: qsTr("试喷")
                        anchors.centerIn: parent
                        font.pixelSize: 16
                        color: "#ffffff"
                    }
                    MouseArea {
                        id: trySpraym
                        anchors.fill: parent
                    }
                    Connections {
                        target: trySpraym
                        onClicked: {
                            if(table_model2.initData[model.row][model.column].HYID !== "")            //判断是否是补充块
                                mw.trySpray(table_model2.initData[model.row][model.column].SprayID, 10)
                        }
                        onPressedChanged: {
                            trySpray.opacity = (trySpray.opacity === 0.3) ? 1.0 : 0.3
                        }
                    }
                }

                Image {
                    id: deleteSpray
                    x: 10
                    y: 134
                    width: 40
                    height: 24
                    source: "img/p2button.png"
                    Text {
                        text: qsTr("删除")
                        anchors.centerIn: parent
                        font.pixelSize: 16
                        color: "#ffffff"
                    }
                    MouseArea {
                        id: deleteSpraym
                        anchors.fill: parent
                    }
                    Connections {
                        target: deleteSpraym
                        onClicked: {
                            //mw.deleteSpray(table_model2.initData[model.row][model.column].index)
                            if(table_model2.initData[model.row][model.column].HYID !== "")            //判断是否是补充块
                                mw.deleteSpray(table_model2.initData[model.row][model.column].index)
                        }
                        onPressedChanged: {
                            deleteSpray.opacity = (deleteSpray.opacity === 0.3) ? 1.0 : 0.3
                        }
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
