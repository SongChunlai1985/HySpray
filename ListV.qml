import QtQuick 2.12

Item {
    width: 360
    height: 616
    Component {
        id: devDelegate
        Rectangle {
            id: dlgt
            height: 120
            width: 720
            color:  "#ffffff"
            Column{
                spacing: 5
                Rectangle{
                    width: 720
                    height: 28
                    color: "#88999999"
                    Text {
                        text: "SN:" + view.model[index].HYID
                        font.pixelSize: 18
                        color: "#ffffff"
                    }
                }

                Text {
                    text: "地址:" + view.model[index].Address
                    font.pixelSize: 16

                }

                Text {
                    text: "状态:" + view.model[index].State
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
                    mw.debug("ListV.qml: " + index)
                    mw.viewDevice(index)
                    gpsref = "off"

                    tabBar.setCurrentIndex(1)
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
        delegate: devDelegate
    }
}











/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
