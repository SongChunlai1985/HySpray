#ifndef IOTMESSAGE_H
#define IOTMESSAGE_H

#include <QObject>
#include <network/tcpwork.h>
#include <network/httpwork.h>
#include <QNetworkCookie>

enum USER_PRIORITY_
{
    USER_PRIORITY_NONE = 0,
    USER_PRIORITY_GUEST = 1,

    USER_PRIORITY_USER  = 3,
    USER_PRIORITY_SUPERUSER,
    USER_PRIORITY_SUPERADMIN,

    USER_PRIORITY_MAX
};

enum ControlOders{
    UpgradeSoftwareVersionNumber  = 1,
    UpgradeRequest                = 2,
    UpgradeData                   = 3,
    UpgradeResult                 = 4,
    GetTheSoftwareVersionNumber   = 10,
    GetTheHardwareVersionNumber   = 11,
    PingSprinkler                 = 12,
    ControlNozzle                 = 13,
    SetSprayHeadID                = 14,
    GetSprayHeadID                = 15,
    SetSerialNumberSN             = 16,
    GetSerialNumberSN             = 17,
    WriteSprayMode                = 20,
    ReadSprayMode                 = 21,
    WriteDailyTime                = 22,
    ReadDailySchedule             = 23,
    WriteSprayHeadGrouping        = 24,
    ReadThePrintHeadGroupingTable = 25,
    WriteSingleDayTask            = 26,
    ReadSingle_dayTaskTable       = 27,
    WriteWeekTask                 = 28,
    ReadTheWeeklyTaskList         = 29,
    ExecuteSprayModeImmediately   = 30,
    SynchronizeHostTime           = 31,
    GetHostTime                   = 32,
    QueryHostStatus               = 33,
    StopTaskExecution             = 34,
    ControlWaterPump              = 35,
    ConfigNetWork                 = 36,
    TrySingelSpray                = 37,
    SystemReset                   = 38,
    ClearSpray                    = 39,
    GetSprayRecord                = 46
};

class iotMessage : public QObject
{
    Q_OBJECT
public:
    iotMessage();
    QString getMessage(QString url);
    //QString login(QString url, QString username, QString password);

    int pickData(QJsonObject& content, QJsonObject& data);

    bool checkServer(); // check server is available ?
    bool checkToken();  // is Token expired ?
    bool login(QString username, QString password, QString& errorMsg);
    bool isLogin();
    int getAccountPriority();

    bool deviceList(QJsonObject& devList, QString& errorMsg, bool isIncludeChild = true, int organizationID = -1);
    bool deviceRegister(QString sn, QString& errorMsg);
    bool deviceUpdate(QString sn, QJsonObject property, QString& errorMsg);
    bool deviceInfo(QString sn, QJsonObject& property, QString& errorMsg);
    bool deviceStatus(QString sn, QJsonObject& property, QString& errorMsg);
    bool deviceStatusUpdate(QString sn, QJsonObject& property, QString& errorMsg);

    bool sprinklerRegister(QString deviceSN, QString sprinklerSN, int sprinklerID, QString& errorMsg);
    bool sprinklerUnregister(QString deviceSN, QString sprinklerSN, int sprinklerID, QString& errorMsg);
    bool sprinklerUpdate(QString deviceSN, int sprinklerID, QJsonObject property, QString& errorMsg);
    bool sprinklerInfo(QString deviceSN, int sprinklerID, QJsonObject& property, QString& errorMsg);
    bool sprinklerStatus(QString deviceSN, int sprinklerID, QJsonObject& property, QString& errorMsg);

    bool planUpdate(QString sn, QJsonObject plan, QString& errorMsg);
    bool planInfo(QString sn, int planNumber, QJsonObject& plan, QString& errorMsg);

    bool setHostStatus(QString sn, bool isOn, QString& errorMsg);
    bool getSprinklerSN(QString devSN, int sprinklerID, QString& sprinklerSN, QString& errorMsg, int waitSecs = 10);

    bool sendPlan(QString devSN, ControlOders Cmd, QJsonObject Data, QString& errorMsg);

    //for move data:
    bool cbInfo(QString sn, QJsonObject& property, QString& errorMsg);
    //////////////////////

    QByteArray action(QString dev, ControlOders Cmd, QJsonObject Data);     //2022-03 通过私服发送指令到设备（RS485）
    QByteArray getActionResp(QString dev, int seq);     //2022-03 get response for "通过私服发送指令到设备（RS485）"

    QByteArray post(QString api, QJsonObject data);

    QByteArray sendReq(QString url, QByteArray content);
protected:
//    QByteArray sendReq(QString url, QByteArray content);

    void Sleep(int delay);

private:
    bool hasLogin = false;
    QDateTime loginTime;

    tcpwork IotGet;
    QByteArray loginJsonByteArray;
    QUrl Url;

    QNetworkCookie cookie;

    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest Request;

    QString token;
    int acount_priority = USER_PRIORITY_NONE;

    QString httpPageReady();
    void httpConnected();
    void httplogin(QString username, QString password);
    void httpConnected2();
    QString httpPageReady2();
};

#endif // IOTMESSAGE_H
