import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12
//import QZXing 2.3

Rectangle {
    id: scanqrcodeview
    function start(){
        camera.start()
    }

    function stop(){
        camera.stop()
    }

    function qrcode(tag){
        mw.debug("++" + tag)
        if(mw.getqrcode(tag,tag) === "ok"){
            playMusic.play()
            tmr_vout_b.stop()
            tmr_vout.start()
            mouseArea_closeQrScan.visible = false
            camera.stop()
            qRCodeScanner.y = -1000
            //appwin.footer = tabBar
            qrInput.text = qrInput.text.substring(0,28)
        }
    }

    //设置音频
    MediaPlayer {
        id: playMusic
        source: "sound/di.mp3"
    }
//    MouseArea {
//        id: playArea
//        anchors.fill: parent
//        //启动音频
//        onPressed:  {
//            playMusic.play()
//        }
//    }

    Connections {
        target: Camerafilter
        onQrcode: {
            qrcode(qr)
        }
    }

    //    border.color: "black"
    //    border.width: 1
    property int detectedTags: 0
    property string lastTag: ""
    property  int  nWidth: Math.min(width,height)

    ColumnLayout {
        anchors.fill: parent
        //anchors.margins: Style.layoutMargin
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            /*QZXingFilter {
                id: zxingFilter
                captureRect: {                                                                     // setup bindings
                    videooutput.contentRect;
                    videooutput.sourceRect;                                                        //识别的区域，这里稍微比扫描框大一些
                    return videooutput.mapRectToSource(videooutput.mapNormalizedRectToItem(Qt.rect(
                                                                                               0.2, 0.2, 0.6, 0.6
                                                                                               )));
                }

                decoder {
                    enabledDecoders: QZXing.DecoderFormat_EAN_13 | QZXing.DecoderFormat_CODE_39 | QZXing.DecoderFormat_QR_CODE
                    onTagFound: {
                        mw.debug(tag + " | " + decoder.foundedFormat() + " | " + decoder.charSet())
                        scanqrcodeview.detectedTags++;
                        scanqrcodeview.lastTag = tag;
                        qrcode(tag)
                    }
                    tryHarder: false
                }

                onDecodingStarted:
                {
                    mw.debug("DecodingStarted")
                }

                property int framesDecoded: 0
                property real timePerFrameDecode: 0

                onDecodingFinished:
                {
                    timePerFrameDecode = (decodeTime + framesDecoded * timePerFrameDecode) / (framesDecoded + 1);
                    framesDecoded++;
                    mw.debug("onDecodingFinished " + succeeded + " " + decodeTime + " " + timePerFrameDecode + " " + framesDecoded)
                }
            }*/

            Camera {
                id: camera
                captureMode: Camera.CaptureStillImage
                focus {
                    focusMode: CameraFocus.FocusContinuous
                    focusPointMode: CameraFocus.FocusPointAuto
                }

                videoRecorder {
                    frameRate: 10
                }
            }

            VideoOutput {
                id: videooutput
                source: camera
                anchors.fill: parent
                autoOrientation: true
                orientation: 90
                fillMode: VideoOutput.PreserveAspectCrop
                filters: [ Camerafilter ]

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        /*camera.focus.customFocusPoint = Qt.point(mouse.x / width,  mouse.y / height);
                        camera.focus.focusMode = CameraFocus.FocusMacro;
                        camera.focus.focusPointMode = CameraFocus.FocusPointCustom;*/
                    }
                }

                Rectangle {
                    id: capturezone
                    width: parent.width > parent.height ? parent.height * 0.7 : parent.width * 0.7
                    height: parent.width > parent.height ? parent.height * 0.7 : parent.width * 0.7
                    anchors.centerIn: parent
                    color:"transparent"
                    border.color: "lightgreen"
                    border.width: 1
                    opacity: 0.4

                    Rectangle{
                        id:line
                        width: parent.width * 0.9
                        height: 2
                        color: "lightgreen"
                        anchors.horizontalCenter: parent.horizontalCenter
                        radius: 1

                        PropertyAnimation{
                            id:ani
                            target: line
                            properties: "y"
                            duration: 2500
                            from: line.height * 2
                            running: true
                            to: capturezone.height - line.height * 2
                            onStopped: {
                                ani.start()
                            }
                        }
                    }
                }
            }

            TextField {
                id: qrInput
                x: capturezone.x
                y: capturezone.y + 3 + capturezone.height
                width: capturezone.width
                height: 35
                horizontalAlignment: Text.AlignHCenter
                placeholderText: qsTr("手工填写二维码信息")
                font.pixelSize: 14
                color: "#FF0000FF"
                background: Rectangle {
                    color: "#55229922"
                }
                onTextChanged: {
                    if(text.length > 28)qrcode(text)
                    console.log(text)
                    mw.debug("手工填写二维码信息 "+ text)
                }
            }

        }
    }
}




















/*##^## Designer {
    D{i:0;autoSize:true;height:616;width:360}
}
 ##^##*/
