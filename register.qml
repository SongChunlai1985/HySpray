import QtQuick 2.12

Item {
    width: appwin.width
    height: appwin.height
    Rectangle {
        id: bg
        width: parent.width
        height: parent.height
    }
    MouseArea {
        anchors.fill:  parent
        onClicked: {
            console.log("register.qml")
            parent.y = -1600
        }
    }
    SequentialAnimation{
        id: sequential
        running: true                                                  //加载完就执行
        NumberAnimation{
            target: bg
            properties: "x"
            from: 360
            to: 0
            duration: 100
            easing.type: Easing.InCubic                                //改变缓和曲线类型
        }
    }
}





















/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
