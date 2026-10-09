import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtWebView 1.1

Page {
    id: pf6
    width: appwin.width
    height: appwin.height
    property alias webView: webView
    property alias pf6: pf6

    property string userName: ""
    property string password: ""
    property bool userChanged: false
    property bool firstLoad: true
    property string ourURL: "https://hysds.hyairo.vip/"

    header: Label {
        id: label1
        color: "#FFFFFF"
        text: "用户信息"
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        padding: 10
        background: Image {
            id: label0bg
            anchors.fill: parent
            source: "img/hdb.png"
        }

        Text {
            id: adddevice
            x: 9
            y: 9
            color: "#ffffff"
            text: "<"
            font.family: "Tahoma"
            font.bold: true
            font.pixelSize: 21
        }

        MouseArea {
            id: mouseArea
            x: 10
            y: 9
            width: 50
            height: 30
            anchors.right: parent.right
            anchors.rightMargin: 300
            Connections {
                onClicked: {
                    swipeView.currentIndex = 4
                }
            }
        }

        Label {
            id: label
            x: 255
            y: 13
            color: "#ffffff"
            text: qsTr("注销登录")
            anchors.right: parent.right
            anchors.rightMargin: 70
            font.bold: true
            font.pixelSize: 14
        }

        MouseArea {
            x: 231
            y: 9
            width: 75
            height: 30
            anchors.right: parent.right
            anchors.rightMargin: 54
            Connections {
                onClicked: {
                    pf0.y = 0
                    webView.visible = false
                }
            }
        }
    }

    Connections {
            target: mw
            onSetAccount: {
                userName = name
                password = pwd

                ourURL = "https://hysds.hyairo.vip/login/user/account?userName=" + userName +  "&password=" + password + "&autoLogin=true"

                if (isUserChanged)
                {
                    mw.uiDebug("user changed")
                    userChanged = true

                    webView.url = "https://hysds.hyairo.vip/api/logout"
                }
                else
                {
                    webView.url = ourURL
                }
                //webView.url = "https://hysds.hyairo.vip/login/user/account?userName=" + userName +  "&password=" + password + "&autoLogin=true"
                //webView.url = "https://hysds.hyairo.vip/api/logout"

                webView.visible = false
                mw.uiDebug(webView.url)
            }

    }

    WebView {
        id: webView
        x: 0
        y: 0
        width: pf6.width
        height: pf6.height - 84
        //url: "https://hysds.hyairo.vip/login/user/account?userName=" + userName +  "&password=" + password + "&autoLogin=true"
        url: ourURL
        visible: false
    }
}




/*##^## Designer {
    D{i:0;height:616;width:360}
}
 ##^##*/
