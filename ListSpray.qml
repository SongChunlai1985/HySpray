import QtQuick 2.12

Item {
    width: 360
    height: 616
    Component {
        id: sprayDelegate
        Rectangle {
            id: dlgt
            height: 180
            width: 720
            color:  "#ffffff"
            Column{
                spacing: 5
                Rectangle{
                    width: 720
                    height: 28
                    color: "#88999999"
                    Text {
                        text: "序列号:" + view.model[index].HYID
                        font.pixelSize: 18
                        color: "#ffffff"
                    }
                }

                Text {
                    text: "安装位置:" + view.model[index].Position
                    font.pixelSize: 16

                }

                Text {
                    text: "工作状态:" + view.model[index].State
                    font.pixelSize: 16
                }

                Text {
                    text: "喷雾方案:" + view.model[index].Plan
                    font.pixelSize: 16
                }


                Text {
                    text: "喷头编号:" + view.model[index].SprayID
                    font.pixelSize: 16
                }

                Text {
                    text: "更新时间:" + view.model[index].UpdateTime
                    font.pixelSize: 16
                }
            }

            MouseArea{
                anchors.fill: dlgt
                onClicked: {
                    mw.debug("ListSpray.qml: " + index)
                    tabBar.setCurrentIndex(3)
                    mw.getPlan(0);
                }
                onPressedChanged: {
                    dlgt.color = dlgt.color == "#999999" ? "#ffffff" : "#999999"
                }
            }
        }

    }
    property string devices: '{}'

    ListView {
        id: view
        anchors.fill: parent
        model: JSON.parse(devices).data                                                            //C++ Json转model
        delegate: sprayDelegate
    }
}















/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
