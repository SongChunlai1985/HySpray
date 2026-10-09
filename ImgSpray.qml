import QtQuick 2.12
import EasyModel 1.0

Item {
    width: parent.width
    height: parent.height
    property alias wk: wk
    property int sprayId: 1
    Image {
        id: spr
        x: 0
        y: 0
        width: 100
        height: 100
        Rectangle {
            id: rectangle1
            x: 50
            y: -13
            width: 3
            height: 14
            color: "#000000"
        }

        Rectangle {
            id: rectangle4
            x: 48
            y: -28
            width: 7
            height: 15
            color: "#000000"
        }
        source: "img/b.png"
        fillMode: Image.PreserveAspectFit

        Image {
            id: wk
            x: 20
            y: -1600
            width: 10
            height: 10
            source: "img/w.png"
        }

        Text {
            id: element
            x: 44
            y: 51
            color: "#1b9bff"
            text: sprayId
            font.bold: true
            font.pixelSize: 20
        }
    }

    MouseArea{
        anchors.fill: spr
        onReleased: {
            work("on")
            mw.trySpray(sprayId,10)
        }
    }

    Timer{
        id:tmr
        repeat: false
        interval: 12000
        onTriggered: {
            work("off");
            busyIndicator.running = false
        }
    }

    function work(On){
        if(On === "on"){
            tmr.start()
            wk.y = 0

        }
        if(On ==="off"){
            wk.y = -1600
            tmr.stop()
        }
    }

    Connections{
        target: mw
        onSprayWork: {
            if(index == sprayId) {
                work("on")
                busyIndicator.running = true
            }
        }

        onSprayTimeUp: {
            if(index == sprayId) {
                work("off")
            }
        }
    }
}












































































/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
