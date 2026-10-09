import QtQuick 2.12
import QtQuick.Extras 1.4
import QtQuick.Controls 2.12

Rectangle {
    id: rectanglemins
    width: frame.implicitWidth
    height: frame.implicitHeight

    property int ix: 122
    property int iy: 138

    property var sprayTime: [2,3,6,10,15,25]

    function formatText(count, modelData) {
        var data = modelData;
        return data.toString().length < 2 ? "0" + data : data;
    }

    function formatO(count){
        return count.toString().length < 2 ? "0" + count : count;
    }

    function settime(str, x, y){
        ix = x
        iy = y

        var time = Number(str.substring(3, 5))
        var index = 0
        for (index = 0; index < sprayTime.length; index++)
        {
            if (time === sprayTime[index])
            {
                break;
            }
        }

        if (index >= sprayTime.length)
            index = 0

        minutesTumbler.currentIndex = index
    }

    FontMetrics {
        id: fontMetrics
    }

    Component {
        id: delegateComponent
        Label {
            text:  qsTr("%1").arg(modelData)
            opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 25
            font.bold: true
        }
    }

    Frame {
        id: frame
        width: 111
        height: 103
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        padding: 0

        Rectangle {
            width: 111
            border.color: "#9d9d9d"
            border.width: 1
            anchors.fill: parent
        }

        Rectangle {
            id: rectangle1
            x: 1
            width: 109
            height: 23
            anchors.verticalCenter: parent.verticalCenter
            color: "#f5f5f5"
            Label{
                x: 70
                text: "\u5206"
                font.bold: true
                font.pixelSize: 20
                anchors.verticalCenterOffset: -2
                anchors.verticalCenter: parent.verticalCenter
            }

        }

        Row {
            id: row

            Tumbler {
                id: minutesTumbler
                width: 80
                height: 103
                font.pointSize: 20
                font.bold: true
                model: sprayTime
                delegate: delegateComponent
            }


        }
    }
    Button {
        id: button
        x: 90
        width: 111
        height: 38
        text: qsTr("确  定")
        anchors.horizontalCenterOffset: 0
        palette {
            buttonText: "white"
        }
        anchors.horizontalCenter: frame.horizontalCenter
        anchors.top: frame.bottom
        anchors.topMargin: 0
        font.pixelSize: 22

        onClicked: {
            pf4.state = ""

            var  index = 0

            if (minutesTumbler.currentIndex != -1)
            {
                index = minutesTumbler.currentIndex
            }

            var tiemStr = "00:" + formatO(sprayTime[index])

            if(currentTextField == "element0v5"){
                element0v5.text = tiemStr
            }

            if(currentTextField == "element1v5"){
                element1v5.text = ""+tiemStr
            }

            if(currentTextField == "element2v5"){
                element2v5.text = ""+tiemStr
            }
        }

        background: Rectangle {
            color: parent.pressed ? "#ffa5b6cf" : "#ff255f8e"
        }
    }

}





















/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
