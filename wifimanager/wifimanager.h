#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QList>
#include <QString>
#include <QDebug>
#include <math.h>

#include "jni.h"

struct WifiInfo{
    int level;
    QString SSID;
    QString BSSID;
    QString keytype;
};

class WifiManager{

public:
     WifiManager();
    ~WifiManager();

     QAndroidJniObject JniObject;    //wifi process

     void ShowSoftKeyboard(int isShow = 1);
     QString isWifiEnable();
     void openWifi();
     void closeWifi();
     void scanWifi();
     QString getwifi(QString id,QString passwd);
     int getWifiListCount();

     void opencamera();

     QString getWifiSSID(int index);
     QString getWifiBSSID(int index);
     QString getwifiip();
     int getWifiLevel(int index);
     QString getKeyType(int index);

     QString getConntectedWifiSSID();    //获取当前连接的wifi信息
     QString getConnectedWifiAddress();

     void connectToWifi(int id,QString passwd);    //连接到wifi
     void connectToWifiWithoutPasswd(int id);

     int getMaxVolumnStream();    //多媒体音量控制
     int getCurrentVolumnStream();
     void setVolumnStream(int a);

     double getDentisy();    //获取屏幕像素密度

     void refreshWifiList();    //user process

     int  wifiCount();
     int  wifiLevel(int i);
     QString wifiSSID(int i);
     QString wifiBSSID(int i);
     QString wifiKeyType(int i);

     QString getgpslocation();

     void enableGps();
     void showToast(QString msg);
signals:

public slots:
private:
    QList<WifiInfo> wifiList;
    QString jpath_="";
};

#endif // WIFIMANAGER_H
