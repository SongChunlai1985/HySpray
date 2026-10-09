#include "wifimanager.h"

WifiManager::WifiManager(){}

WifiManager::~WifiManager(){}

void WifiManager::refreshWifiList(){   //user functionds
    wifiList.clear();
    scanWifi();
    int count = getWifiListCount();
    for(int a = 0; a<count; a++){
        WifiInfo info;
        info.SSID=getWifiSSID(a);
        info.BSSID=getWifiBSSID(a);
        info.level=getWifiLevel(a);
        info.keytype=getKeyType(a);
        wifiList.append(info);
    }
}

int  WifiManager::wifiCount(){
    return wifiList.count();
}

int  WifiManager::wifiLevel(int i){
    return wifiList.at(i).level;
}

QString WifiManager::wifiSSID(int i){
    return wifiList.at(i).SSID;
}

QString WifiManager::wifiBSSID(int i){
    return wifiList.at(i).BSSID;
}

QString WifiManager::wifiKeyType(int i){
    return wifiList.at(i).keytype;
}

QString WifiManager::isWifiEnable(){         //wifi cability  =========com/fyairo/hyspray==========
    jint state = QAndroidJniObject::callStaticMethod<jint>("com/fyairo/hyspray/ExtendsQtWithJava", "networkState");
    return state == 1 ? "true" : "false";
}

void WifiManager::openWifi(){
    QAndroidJniObject::callStaticMethod<void>("com/fyairo/hyspray/ExtendsQtWithJava", "openWifi");
}

void WifiManager::enableGps(){
    QAndroidJniObject::callStaticMethod<void>("com/fyairo/hyspray/ExtendsQtWithJava", "enableGps");
}

void WifiManager::closeWifi(){
    QAndroidJniObject::callStaticMethod<void>("com/fyairo/hyspray/ExtendsQtWithJava", "closeWifi");
}

void WifiManager::scanWifi(){
    QAndroidJniObject::callStaticMethod<void>("com/fyairo/hyspray/ExtendsQtWithJava", "scanWifi");
}

QString WifiManager::getwifi(QString id, QString passwd){
    QAndroidJniObject sid = QAndroidJniObject::fromString(id);
    QAndroidJniObject str = QAndroidJniObject::fromString(passwd);
    QAndroidJniObject strrt = QAndroidJniObject::callStaticObjectMethod("com/fyairo/hyspray/ExtendsQtWithJava", "getwifi",
                                                                        "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
                                                                        sid.object<jstring>(), str.object<jstring>());
    return strrt.toString();
}

int WifiManager::getWifiListCount(){
    jint count = QAndroidJniObject::callStaticMethod<jint>("com/fyairo/hyspray/ExtendsQtWithJava", "getWifiCount");
    return count;
}

void WifiManager::opencamera()
{
    QAndroidJniObject::callStaticMethod<void>("com/fyairo/hyspray/ExtendsQtWithJava", "openCamera");
}

QString WifiManager::getWifiSSID(int index){
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("com/fyairo/hyspray/ExtendsQtWithJava",
                                                                    "getWifiSSID", "(I)Ljava/lang/String;", index);
    return str.toString();
}

QString WifiManager::getWifiBSSID(int index){
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("com/fyairo/hyspray/ExtendsQtWithJava",
                                                                    "getWifiBSSID", "(I)Ljava/lang/String;", index);
    return str.toString();
}

QString WifiManager::getwifiip(){
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("com/fyairo/hyspray/ExtendsQtWithJava",
                                                                    "getwifiip", "()Ljava/lang/String;");
    return str.toString();
}

QString WifiManager::getgpslocation(){
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("com/fyairo/hyspray/ExtendsQtWithJava",
                                                                    "getgpslocation", "()Ljava/lang/String;");
    return str.toString();
}

int WifiManager::getWifiLevel(int index){
    int a = QAndroidJniObject::callStaticMethod<int>("com/fyairo/hyspray/ExtendsQtWithJava", "getWifiLevel", "(I)I", index);
    qDebug()<<"wifi level: "<<a;
    if(abs(a) > 80){
        return 1;
    }
    else if(abs(a) > 50 && abs(a) <= 80){
        return 2;
    }
    else if(abs(a) <= 50){
        return 3;
    }
    return 3;
}

QString WifiManager::getKeyType(int index){
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("com/fyairo/hyspray/ExtendsQtWithJava",
                                                                    "getWifiKeyType", "(I)Ljava/lang/String;", index);
    return str.toString();
}

QString WifiManager::getConntectedWifiSSID(){                          //获取当前连接的wifi信息
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("com/fyairo/hyspray/ExtendsQtWithJava",
                                                                    "getCurrentWifiSSID", "()Ljava/lang/String;");
    return str.toString();
}

QString WifiManager::getConnectedWifiAddress(){
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("com/fyairo/hyspray/ExtendsQtWithJava",
                                                                    "getHostIPAddress", "()Ljava/lang/String;");
    return str.toString();
}

void WifiManager::connectToWifi(int id,QString passwd){    //连接到wifi
    QAndroidJniObject str=QAndroidJniObject::fromString(passwd);
    jint a = QAndroidJniObject::callStaticMethod<jint>("com/fyairo/hyspray/ExtendsQtWithJava",
                                                     "connectToWifi", "(ILjava/lang/String;)I",
                                                     id, str.object<jstring>());
    qDebug()<<"connect to wifi :"<<a;
}

void WifiManager::showToast(QString msg){
    QAndroidJniObject str = QAndroidJniObject::fromString(msg);
    QAndroidJniObject::callStaticMethod<void>("com/fyairo/hyspray/ExtendsQtWithJava",
                                              "showToast", "(Ljava/lang/String;)V", str.object<jstring>());
}

void WifiManager::connectToWifiWithoutPasswd(int id){
    jint a = QAndroidJniObject::callStaticMethod<jint>("com/fyairo/hyspray/ExtendsQtWithJava",
                                                     "connectToWifiWithoutPasswd", "(I)I", id);
    qDebug()<<"connect to wifi :"<<a;
}

void WifiManager::ShowSoftKeyboard(int isShow)
{
    JniObject.callStaticMethod<void>("com/fyairo/hyspray/ExtendsQtWithJava", "showKeyboard", "(I)V", isShow);
}

int WifiManager::getMaxVolumnStream(){  //多媒体音量控制
    return QAndroidJniObject::callStaticMethod<int>("com/fyairo/hyspray/ExtendsQtWithJava", "getMaxVolumnStream", "()I");
}

int WifiManager::getCurrentVolumnStream(){
    return QAndroidJniObject::callStaticMethod<int>("com/fyairo/hyspray/ExtendsQtWithJava", "getCurrentVolumnStream", "()I");
}

void WifiManager::setVolumnStream(int a){
    QAndroidJniObject::callStaticMethod<void>("com/fyairo/hyspray/ExtendsQtWithJava", "setVolumnStream", "(I)V", a);
}

double WifiManager::getDentisy(){   //获取屏幕像素密度
    return QAndroidJniObject::callStaticMethod<double>("com/fyairo/hyspray/ExtendsQtWithJava", "getDentisy", "()D");
}
