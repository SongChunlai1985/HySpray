import QtQuick 2.12
import QtQuick.Controls 2.12
Rectangle {
    id: rectangle
    width: frame.implicitWidth
    height: frame.implicitHeight

    property int ix: 122
    property int iy: 138

    function formatText(count, modelData) {
        var data = count === 12 ? modelData + 1 : modelData;
        return data.toString().length < 2 ? "0" + data : data;
    }

    function formatO(count){
        return count.toString().length < 2 ? "0" + count : count;
    }

    function settime( str){
        hoursTumbler.currentIndex = Number(str.substring(0, 2))
        minutesTumbler.currentIndex = Number(str.substring(3, 5))
        secTumbler.currentIndex = Number(str.substring(6, 8))
        //console.log("时间是","'",str.substring(0, 2),"' '",str.substring(3, 5),"' '",str.substring(6, 8),"'")

        if(currentTextField == "element0v3"){
            ix = element0v3.x
            iy = element0v3.y - 15
        }

        if(currentTextField == "element0v4"){
            ix = element0v4.x + element0v4.width - frame.width
            iy = element0v4.y - 15
        }

        if(currentTextField == "element0v5"){
            ix = element0v5.x
            iy = element0v5.y - 15
        }

        if(currentTextField == "element0v6"){
            ix = element0v6.x + element0v6.width - frame.width
            iy = element0v6.y - 15
        }


        if(currentTextField == "element1v3"){
            ix = element1v3.x
            iy = element1v3.y - 15
        }

        if(currentTextField == "element1v4"){
            ix = element1v4.x + element1v4.width - frame.width
            iy = element1v4.y - 15
        }

        if(currentTextField == "element1v5"){
            ix = element1v5.x
            iy = element1v5.y - 15
        }

        if(currentTextField == "element1v6"){
            ix = element1v6.x + element1v6.width - frame.width
            iy = element1v6.y - 15
        }


        if(currentTextField == "element2v3"){
            ix = element2v3.x
            iy = element2v3.y - 15
        }

        if(currentTextField == "element2v4"){
            ix = element2v4.x + element2v4.width - frame.width
            iy = element2v4.y - 15
        }

        if(currentTextField == "element2v5"){
            ix = element2v5.x
            iy = element2v5.y - 15
        }

        if(currentTextField == "element2v6"){
            ix = element2v6.x + element2v6.width - frame.width
            iy = element2v6.y - 15
        }

    }

    FontMetrics {
        id: fontMetrics
    }

    Component {
        id: delegateComponent
        Label {
            text: formatText(Tumbler.tumbler.count, modelData)
            opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 25
            font.bold: true
        }
    }

    Frame {
        id: frame
        width: 191
        height: 103
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        padding: 0

        Rectangle {
            width: 191
            border.color: "#9d9d9d"
            border.width: 1
            anchors.fill: parent
        }

        Rectangle {
            id: rectangle1
            x: 1
            width: 189
            height: 23
            anchors.verticalCenter: parent.verticalCenter
            color: "#f5f5f5"
            Label{
                x: 150
                text: "\u5206"
                font.bold: true
                font.pixelSize: 20
                anchors.verticalCenterOffset: -2
                anchors.verticalCenter: parent.verticalCenter
            }
            Label{
                x: 70
                text: "\u65f6"
                font.bold: true
                font.pixelSize: 20
                anchors.verticalCenterOffset: -2
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            id: row

            Tumbler {
                id: hoursTumbler
                width: 80
                height: 103
                font.pointSize: 20
                font.bold: true
                model: 24
                delegate: delegateComponent
            }

            Tumbler {
                id: minutesTumbler
                width: 80
                height: 103
                font.pointSize: 20
                font.bold: true
                model: 60
                delegate: delegateComponent
            }

            Tumbler {
                id: secTumbler
                width: 63
                height: 103
                font.bold: true
                model: 60
                delegate: delegateComponent
                visible: false
            }

        }
    }
    Button {
        id: button
        x: 90
        width: 191
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
        onClicked:
        {
            pf4.state = ""

            if(currentTextField == "element0v3"){
                element0v3.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element0v4"){
                element0v4.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex) /* + ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element0v5"){
                element0v5.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element0v6"){
                element0v6.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }


            if(currentTextField == "element1v3"){
                element1v3.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element1v4"){
                element1v4.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element1v5"){
                element1v5.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element1v6"){
                element1v6.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }


            if(currentTextField == "element2v3"){
                element2v3.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element2v4"){
                element2v4.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element2v5"){
                element2v5.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            if(currentTextField == "element2v6"){
                element2v6.text = ""+
                        formatO(hoursTumbler.currentIndex) + ":" +
                        formatO(minutesTumbler.currentIndex)  /*+ ":" +
                        formatO(secTumbler.currentIndex)*/
            }

            mw.compareTime(element0v3.text + "," +
                                      element0v4.text + "," +
                                      element0v5.text + "," +
                                      element0v6.text + "," +
                                      element1v3.text + "," +
                                      element1v4.text + "," +
                                      element1v5.text + "," +
                                      element1v6.text + "," +
                                      element2v3.text + "," +
                                      element2v4.text + "," +
                                      element2v5.text + "," +
                                      element2v6.text + "," +
                                       currentTextField.toString(), button6.checked )
        }
        background: Rectangle {
            color: parent.pressed ? "#ffa5b6cf" : "#ff255f8e"
        }
    }

}















/*##^## Designer {
    D{i:0;height:616;width:360}D{i:13;anchors_y:414}
}
 ##^##*/
