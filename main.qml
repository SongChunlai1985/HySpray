import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtSensors 5.12
//import QtQuick.Dialogs 1.3
//import Qt.labs.platform 1.1

ApplicationWindow {

    id: appwin
    visible: true

    width: 360
    height: 616

    property int screen_width:  533
    property int screen_height: 829

    property double wd: 1.0
    property double ht: 1.0

    property alias appwin: appwin
    property alias swipeView: swipeView

    property alias btn_gb: btn_gb
    //property alias tabBar: tabBar

    property string gpsref: "on"
    property string scanmode: ""
    property alias pf4: pf4

    title: qsTr("智能空间顶端雾化系统")

    OrientationSensor{
        id:orientation1
        active: true
        onReadingChanged: {
            //console.log("方向:"+reading.orientation.toString()+" w:"+screen_width+" h:"+screen_height +" w1:"+ width +" h1:"+height)
            //mw.debug("方向:"+reading.orientation.toString()+" w:"+screen_width+" h:"+screen_height +" w1:"+ width +" h1:"+height +" w:" + appwin.height  +" h:" + appwin.width)
        }
    }

    Component.onCompleted: {
        screen_width = mw.getScreenWidth()
        screen_height = mw.getScreenHeight()
        //console.log(" w:" + screen_width + " h:" + screen_height + " w1:" + appwin.width + " h1:" + appwin.height + " wd:" + wd + " ht:" + ht)

        mw.init()
        pf0.element1.text = mw.getANDROID_VERSION_NAME()

        //mw.getPlan(0)
        swipeView.enabled = false
        swipeView.visible = false
        qRCodeScanner.stop()


    }

    SwipeView {
        id: swipeView
        anchors.fill: parent

        Page1Form {
        }

        Page2Form {
            id:pf2
        }

        Page3Form {
        }

        Page4Form {
            id:pf4
        }

        Page5Form {
        }

        Page6Form {
            id:pf6
        }
    }

    Image {
        x: 0
        y: parent.height - 44
        width: parent.width
        height: 44
        id: foottabbar
        source: "img/footbar.png"

        Image {
            id: footbariItem1
            x: parent.width / 6 * 0
            y: 0
            width: parent.width / 6
            height: 44
            visible: swipeView.currentIndex === 0
            source: "img/footbar_selected.png"

        }
        MouseArea{
            anchors.fill: footbariItem1
            onClicked: swipeView.currentIndex = 0
        }

        Image {
            id: footbariItem2
            x: parent.width / 6 * 1
            y: 0
            width: parent.width / 6
            height: 44
            visible:  swipeView.currentIndex === 1
            source: "img/footbar_selected.png"

        }
        MouseArea{
            anchors.fill: footbariItem2
            onClicked: swipeView.currentIndex = 1
        }

        Image {
            id: footbariItem3
            x: parent.width / 6 * 2
            y: 0
            width: parent.width / 6
            height: 44
            visible: swipeView.currentIndex === 2
            source: "img/footbar_selected.png"

        }
        MouseArea{
            anchors.fill: footbariItem3
            onClicked: swipeView.currentIndex = 2
        }

        Image {
            id: footbariItem4
            x: parent.width / 6 * 3
            y: 0
            width: parent.width / 6
            height: 44
            visible: swipeView.currentIndex === 3
            source: "img/footbar_selected.png"

        }
        MouseArea{
            anchors.fill: footbariItem4
            onClicked: swipeView.currentIndex = 3
        }

        Image {
            id: footbariItem5
            x: parent.width / 6 * 4
            y: 0
            width: parent.width / 6
            height: 44
            visible: swipeView.currentIndex === 4
            source: "img/footbar_selected.png"

        }
        MouseArea{
            anchors.fill: footbariItem5
            onClicked: swipeView.currentIndex = 4
        }


        Image {
            id: footbariItem6
            x: parent.width / 6 * 5
            y: 0
            width: parent.width / 6
            height: 44
            visible: swipeView.currentIndex === 5
            source: "img/footbar_selected.png"
        }

        MouseArea{
            anchors.fill: footbariItem6
            onClicked:{
                pf6.webView.url = pf6.ourURL
                mw.uiDebug(pf6.webView.url)

                swipeView.currentIndex = 5
                pf6.webView.visible = true
            }
        }

        Label{
            text: qsTr("系统")
            font.pixelSize: 20
            color: swipeView.currentIndex === 0 ? "#93bdd2" : "#FFFFFF"
            anchors.centerIn: footbariItem1
        }

        Label{
            text: qsTr("主机")
            font.pixelSize: 20
            color: swipeView.currentIndex === 1 ? "#93bdd2" : "#FFFFFF"
            anchors.centerIn: footbariItem2
        }

        Label{
            text: qsTr("喷头")
            font.pixelSize: 20
            color: swipeView.currentIndex === 2 ? "#93bdd2" : "#FFFFFF"
            anchors.centerIn: footbariItem3
        }

        Label{
            text: qsTr("方案")
            font.pixelSize: 20
            color: swipeView.currentIndex === 3 ? "#93bdd2" : "#FFFFFF"
            anchors.centerIn: footbariItem4
        }

        Label{
            text: qsTr("演示")
            font.pixelSize: 20
            color: swipeView.currentIndex === 4 ? "#93bdd2" : "#FFFFFF"
            anchors.centerIn: footbariItem5
        }

        Label{
            text: qsTr("我的")
            font.pixelSize: 20
            color: swipeView.currentIndex === 5 ? "#93bdd2" : "#FFFFFF"
            anchors.centerIn: footbariItem6
        }
    }

    Image {
        id: fy3
        x: parent.width - 50
        y: 0
        width: 40
        height: 40
        source: "img/smlogo2.png"
    }

    GroupBox {
        id: groupBox
        anchors.centerIn: parent
        width: 305
        height: 200
        font.pixelSize: 18
        title: qsTr("安装位置")
        background: Rectangle {
            color: "#f2dfdfdf"
        }
        visible: false
        TextField {
            id: textField1
            x: 0
            y: 19
            width: 281
            height: 88
            text: ""
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 18
            font.bold: true
            wrapMode: Text.Wrap
            placeholderText: qsTr("请填写安装地址及位置信息")
            maximumLength: 20
        }

        Button {
            id: button0
            x: 30
            y: 124
            width: 88
            height: 30
            text: qsTr("取消")
            palette {
                buttonText: "white"
            }
            font.pixelSize: 18
            onClicked: {
                scanmode = ""
                groupBox.visible = false;
            }
            background: Rectangle {
                color: parent.pressed ? "#ffa5b6cf" : "#ff1d4987"
            }
        }

        Button {
            id: button1
            x: 163
            y: 124
            width: 88
            height: 30
            text: qsTr("确定")
            palette {
                buttonText: "white"
            }
            font.pixelSize: 18
            onClicked: {
                scanmode = ""
                groupBox.visible = false;
                mw.sumitSpray(textField1.text)
            }
            background: Rectangle {
                color: parent.pressed ? "#ffa5b6cf" : "#ff1d4987"
            }
        }

        Label {
            id: label1
            x: 0
            y: -10
            text: qsTr("喷头:")
            font.pixelSize: 18
        }

        Label {
            id: label2
            x: 46
            y: -10
            width: 235
            height: 23
            font.pixelSize: 18
        }
    }

    Timer {
        id: tmr_vout
        interval: 300
        repeat: false
        onTriggered: {
            btn_gb.visible = false
            tmr_vout.stop()
            if(scanmode == "spray"){
                label2.text = mw.getScannedSpray()
                groupBox.visible = true;
            }
            scanmode = ""
        }
    }

    Timer {
        id: tmr_vout_b
        interval: 200
        repeat: false
        onTriggered: {
            btn_gb.visible = true
            mouseArea_closeQrScan.visible = true
        }
    }

    TextField {
        id: textField
        x: 30
        y: 301
        width: parent.width - 60
        height: 40
        color: "#ffff9000"
        font.pixelSize: 18
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        visible: false
        Connections {
            target: mw
            onHostResponse: {
                textField.text = Response
                textField.visible = Visible
            }
        }
    }

    TextArea {
        id: textMessage
        x: 20
        y: 300
        width: parent.width - 40
        height: 40 * 3
        color: "#ffff9000"
        font.pixelSize: 18
        font.bold: true
        readOnly: true
        background: Rectangle {
            color: blue
        }
        visible: false
        Connections {
            target: mw
            onShowMessage: {
                textMessage.text = msg
                textMessage.visible = Visible
            }
        }
    }

    //    MessageDialog {
    //        id: dlDevListMsgbox
    //        buttons: MessageDialog.Yes | MessageDialog.No
    //        modality: Qt.ApplicationModal
    //        text: "下载设备列表"
    //        informativeText: "是否下载设备列表？"
    //        yesClicked: {
    //            mw.downloadDevList()
    //        }

    //    }

    function startScanQR (scanMode){
        mw.scanmode(scanMode)
        tmr_vout.stop()
        tmr_vout_b.start()
        gpsref = "on"
        mw.openWifi()
        qRCodeScanner.start()
        qRCodeScanner.y = 0
        appwin.footer = null
        scanmode = scanMode
        btn_gb.visible = true
        mouseArea_closeQrScan.enabled = true
    }

    function stopScanQR (){
        tmr_vout.start()
        tmr_vout_b.stop()
        qRCodeScanner.stop()
        qRCodeScanner.y = -1000
        scanmode = ""
        groupBox.visible = false
        btn_gb.visible = false
        mouseArea_closeQrScan.enabled = false
    }

    function register(){
        var component = Qt.createComponent("register.qml")
        var window = component.createObject(appwin)
        window.z = 4
    }

    ScanQRCodeView{
        id: qRCodeScanner
        width: parent.width
        height: parent.height
        y: -5000
    }

    Image {
        id: btn_gb
        x: 20
        y: 20
        width: 30
        height: 30
        visible: false
        source: "img/gb.png"
    }

    Image {
        id: btn_gbp
        anchors.fill: btn_gb
        visible: false
        source: "img/gbp.png"
    }

    MouseArea {
        id: mouseArea_closeQrScan
        anchors.fill: btn_gb
        visible: false
        onClicked: {
            stopScanQR()
        }
        onPressedChanged: {
            btn_gb.visible = btn_gb.visible ? false : true
            btn_gbp.visible = btn_gbp.visible ? false : true
        }
    }

    Page0Form{
        id: pf0
        state: "State1"
    }

    ////////////////////////////////////////////////////////////////
    Popup  {
        id: searchWindow
        x:5
        y:45
        width: parent.width -x - 5
        height: parent.height - y - 45
        visible: false

        TextField  {
            id: searchTextField
            x: 20
            y: 15
            width: parent.width - 40
            height: 40
            font.pixelSize: 14
            focus: true
            //color: focus?"red":"black"
            horizontalAlignment: Text.AlignHLeft
            placeholderText: qsTr("请输入序列号或地址")
            text: ''

            onTextChanged:{
                var devList = mw.searchDev(text)
                updateDevList(devList)
            }
        }

        Rectangle {
            id: devListRect
            x:20
            y:80
            width: parent.width-x
            height: parent.height-y - 30

            ListModel {
                id:hostModel

                //                一个 ListElement 对象就代表一条数据
                //                ListElement {
                //                    host: "HY001"
                //                    //addr: "上海市"
                //                }

            }   //ListModel

            ListView {
                id: devListView
                //x:10
                //y:10
                anchors.fill: parent

                //                ScrollBar.vertical: ScrollBar {
                //                    id: scrollBar
                //                }
                model: hostModel
                delegate:Rectangle {
                    color: devListView.currentIndex == index? "#ffa2d6d7" : "white"
                    x:0
                    y:0
                    width: parent.width - x -10
                    height: 40

                    anchors.margins: 4
                    Column {
                        spacing: 2
                        width: parent.width
                        Label {
                            width: parent.width
                            text:host
                            wrapMode: "WrapAnywhere"
                            //verticalAlignment: Text.AlignHCenter
                        }
                        //Label {text:"<b>位置:</b>"+addr}

                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: devListView.currentIndex=index

                        onDoubleClicked: {
                            devListView.currentIndex=index

                            var content = hostModel.get(index)

                            searchWindow.visible = false
                            var isSet = mw.searchSelectDev(content.host)

                            hostModel.clear()
                            searchTextField.text = ""

                            if (isSet)
                            {
                                swipeView.currentIndex = 1
                            }
                        }
                    }
                }

            }   //ListView
        }   //Rectangle
    }   // Popup

    function updateDevList(devList)
    {
        hostModel.clear()

        for (var i = 0; i < devList.length; i++)
        {
            //hostModel.append({"host":devList[i], "addr":devList[i]});
            hostModel.append({"host":devList[i]});
        }
    }

    ////////////////////////////////////////////////////////////////

    Popup  {
        id: viewWindow
        x: 10
        y: 125
        width: parent.width - 20
        height: parent.height - 250
        visible: false
        font.pixelSize: 16

        Rectangle {
            id: listJsonTable
            x: 10
            y: 10
            width: parent.width - 20
            height: parent.height - 20
            color: "#ffa2d6d7"

            ListModel {
                id: jsonModel
            }

            ListView {
                id: jsonList
                anchors.fill: parent
                model: jsonModel

                delegate:Rectangle {
                    color: "#ffa2d6d7"
                    width: parent.width
                    height: 30

                    Column {
                        spacing: 5
                        width: parent.width

                        Label {
                            width: parent.width
                            text: js
                            wrapMode: "WrapAnywhere"
                        }
                    }
                }
            }
        }
    }

    function showFilesName()
    {
        jsonModel.clear()
        var jsonM = mw.viewFiles()

        for(var i = 0; i < jsonM.length; ++i)
        {
            jsonModel.append({"js":jsonM[i]})
        }
    }
}

/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
