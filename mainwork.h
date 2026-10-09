#ifndef MAINWORK_H
#define MAINWORK_H

#include <QObject>
#include <base/base.h>
#include <QJsonObject>
#include <wifimanager/wifimanager.h>
#include <network/udpwork.h>
#include <fjson/fjson.h>
#include <QTimer>
#include <gps/gps.h>
#include <network/tcpwork.h>
#include <QCryptographicHash>
#include <QtMqtt/QtMqtt>
#include <QFile>

#include "iotmessage.h"
#include "iotSDK/iotsdk.h"
#include "deviceman.h"

//#define MOVE_DATA_OPER

class NetDebug
{
public:
    static void post(QByteArray& data);
    static void post(QString msg);
    static void post(QJsonObject& data);
    static void post(QJsonArray& data);

    static void setDebugAddr(QString ip);

private:
    static bool enable;
};

class mainwork : public QObject
{
    Q_OBJECT

signals:
    void hostScanSuccess(QString sn, QString ssid, QString pwd);
    void hostScanStop();
    void updatestate(QString connectState,QString gpslocation,bool connectWifi);
    void addressArrive(QString address);
    void refreshDevicesList(QJsonArray devices);
    void uiSetPriority(int prio);
    void deviceInfoArrive(QString sn,
                          QString ssid,
                          QString pwd,
                          QString MapCoordinates,
                          QString Address,
                          QString WorkWifiSsid,
                          QString WorkWifiPassword,
                          QString NumberOfSpray,
                          QList<int> sprayIds);

    void sprayScanSuccess(QJsonArray listdata);
    void showPlan(int num,
            bool pb2, bool pb3,
            bool pb4, bool pb5,
            bool pb6, bool pb7,

            QString pt03, QString pt04,
            QString pt05, QString pt06,
            QString pt13, QString pt14,
            QString pt15, QString pt16,
            QString pt23, QString pt24,
            QString pt25, QString pt26,

            bool s00, bool s01, bool s02, bool s03, bool s04, bool s05, bool s06, bool s07,
            bool s10, bool s11, bool s12, bool s13, bool s14, bool s15, bool s16, bool s17
            );
    void setAccount(QString name, QString pwd, bool isUserChanged);

    void hostResponse(QString Response, bool Visible);
    void showMessage(QString msg, bool Visible);
    void firstShowMessage(QString msg, bool Visible);
    void sprayWork(int index);
    void sprayTimeUp(int index);
    void userDataArrive(QString userName, QString passWord, QString loginTime);

public:
    mainwork();
    WifiManager WifiController;
    QString APState;
    bool WifiRightConnected = false;

    QJsonObject CurrentPlan;
    int CurrentPlanIndex = 0;
    QString CurrentPlanName = "方案一";
    QJsonArray Plans;
    QJsonArray CachePlans;
    QString conflictMsg = "";
    bool isTimingAvalid[PLAN_COUNT][TIMING_COUNT] = {{false}};
	
    QJsonObject ScanSpray;
    int CurrentSprayindex;
    QJsonObject CurrentSpray;
    QJsonArray  SprayHeads;
    QJsonObject SpraysData;
    QString SpraysString;

    int CurrentDeviceIndex;
    QJsonObject CurrentDevice;
    QJsonArray  Devices;    //记录所以设备信息

    QJsonObject DownloadDevices; //从服务器下载的设备列表

    //QJsonObject DevicesData;
    //QString DevicesString;
    QString DevicesTableViewDataString;
    udpwork udpw;
    QTimer *timer;
    QTimer *timerRefresh;
    QTimer *timerSendOneOder;
    QTimer *timerWritedTimeout;
    QTimer *timerMessageClose;
    QTimer *timerConnectTimeout;
    QString netState = "没有连接";
    QString workState = "";
    gps gpsd;
    tcpwork MapGet;

    QString rootPath = "/storage/self/primary/FYAIRO/FySpray";
    QString dataPath = "/userdata";
    QString jsonPath = "/jsondata";
    QString UserName ;
    QString PassWord ;
    QString LoginTime;
    QByteArray UserData;

    QString netId;
    tcpwork Connecter;
    bool QueueSendComplate = true;

    QJsonArray Oders;
    QTimer *timerClosePump;
    QByteArray Receivemsg;
    int numOfPakge = 0;
    int Duration10 = 0;

    Q_INVOKABLE void init();

    void initDebug();

    Q_INVOKABLE int getScreenWidth();
    Q_INVOKABLE int getScreenHeight();

    int connectHostWifiTimes = 0;
    Q_INVOKABLE QString getqrcode(QString qr, QString cvm4);    //扫描获得二维码后

    // Wifi Operation:
    Q_INVOKABLE void openWifi();
    Q_INVOKABLE void connectHostWifi();
    Q_INVOKABLE void disconnectTcp();
    Q_INVOKABLE void disconnectHostWifi();

    bool isConnect2Host();
    QString getHostWifiName(QString hostSN);
    QString getHostWifiPwd(QString hostSN = "");
    void onNewHost(QString sn);

    Q_INVOKABLE void saveHostInfo(QString MapCoordinates, QString Address, QString WorkWifiSsid, QString WorkWifiPassword); //主机信息设置保存
    //Q_INVOKABLE void configWorkWifi();          //配置Wifi

    Q_INVOKABLE void disconnectHost();
    Q_INVOKABLE void debug(QString msg);
    Q_INVOKABLE void compareTime(QString element_msg, bool checked);
    void update();
    Q_INVOKABLE void getAddress();

    void call_off_sending();

    void httpPageReady();
    void httpConnected();

    bool readDevicesData(QString& devicesString);
    bool writeDevicesData(QString& devicesString);

    bool saveDevices(); //将Devices保存到配置文件中
    bool loadDevices();

    void saveAllDevices();

    void saveHost();

    void LogFile(QByteArray& info);
    void LogFile(QJsonObject& info);
    void LogFile(QString info);

    Q_INVOKABLE void viewDevice(int index);
    Q_INVOKABLE void adddevices();
    void wifiRefresh();
    QString ScanMode;
    Q_INVOKABLE void scanmode(QString mode);
    Q_INVOKABLE QString getScannedSpray();

    Q_INVOKABLE void getPlan(int index);
    Q_INVOKABLE bool cachePlan(int numplan,
                               bool isVarSpeed, bool isConstSpead,
                               bool isWorkday, bool isEveryday,
                               bool isTiming, bool isLoop,
                               QString Starttime1, QString Endtime1,
                               QString Spraytime1, QString Intervaltime1,
                               QString Starttime2, QString Endtime2,
                               QString Spraytime2, QString Intervaltime2,
                               QString Starttime3, QString Endtime3,
                               QString Spraytime3, QString Intervaltime3,
                               bool isSparyer01, bool isSparyer02, bool isSparyer03, bool isSparyer04, bool isSparyer05, bool isSparyer06, bool isSparyer07, bool isSparyer08,
                               bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12, bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16);

    bool getPlan(int index, PlanInfo& aPlan);

    bool checkTimeConflict(int numplan, QJsonArray& allPlans);
    bool checkTimingAvalid(QJsonArray& allPlans);

    Q_INVOKABLE void setPlan(int numplan,
                  bool isVarSpeed, bool isConstSpead,
                  bool isWorkday, bool isEveryday,
                  bool isTiming, bool isLoop,
                  QString Starttime1, QString Endtime1,
                  QString Spraytime1, QString Intervaltime1,
                  QString Starttime2, QString Endtime2,
                  QString Spraytime2, QString Intervaltime2,
                  QString Starttime3, QString Endtime3,
                  QString Spraytime3, QString Intervaltime3,
                  bool isSparyer01, bool isSparyer02, bool isSparyer03, bool isSparyer04, bool isSparyer05, bool isSparyer06, bool isSparyer07, bool isSparyer08,
                  bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12, bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16);
				  
    void HostConnected();
    void HostMessage();
    void SendOneOder();

    bool SendPlan(ControlOders Cmd, QJsonObject Data);
    bool SendOder(ControlOders Cmd, QJsonObject Data, int deciSecond = 10);	// deciSecond 单位: 0.1 sec, 只对直连主机WiFi有效

    int DurationMin = 0;

    QJsonObject getSprayMode(int ID, int Duration, int Delay, int Mode, int Length,
                             int Speed1= -1, int OnTime1= -1, int OffTime1= -1,
                             int Speed2= -1, int OnTime2= -1, int OffTime2= -1,
                             int Speed3= -1, int OnTime3= -1, int OffTime3= -1,
                             int Speed4= -1, int OnTime4= -1, int OffTime4= -1,
                             int Speed5= -1, int OnTime5= -1, int OffTime5= -1);
    QJsonObject getSprayModeDefault(int ID, int mode);

    QJsonObject getDailyTime(int ID, int Mode, int Length,
                             int Hour1= -1, int Min1= -1,
                             int Hour2= -1, int Min2= -1,
                             int Hour3= -1, int Min3= -1,
                             int Hour4= -1, int Min4= -1,
                             int Hour5= -1, int Min5= -1,
                             int Hour6= -1, int Min6= -1,
                             int Hour7= -1, int Min7= -1,
                             int Hour8= -1, int Min8= -1,
                             int Hour9= -1, int Min9= -1);

    QJsonObject getGroup(bool ID1  = false, bool ID2  = false, bool ID3  = false, bool ID4  = false, bool ID5  = false, bool ID6  = false, bool ID7  = false, bool ID8  = false,
                         bool ID9  = false, bool ID10 = false, bool ID11 = false, bool ID12 = false, bool ID13 = false, bool ID14 = false, bool ID15 = false, bool ID16 = false);

    QJsonObject getSingleDayTask(int ID, int Num,
                                 int TimeID1   = -1, int GroupID1   = -1, int ModeID1   = -1,
                                 int TimeID2   = -1, int GroupID2   = -1, int ModeID2   = -1,
                                 int TimeID3   = -1, int GroupID3   = -1, int ModeID3   = -1,
                                 int TimeID4   = -1, int GroupID4   = -1, int ModeID4   = -1,
                                 int TimeID5   = -1, int GroupID5   = -1, int ModeID5   = -1,
                                 int TimeID6   = -1, int GroupID6   = -1, int ModeID6   = -1,
                                 int TimeID7   = -1, int GroupID7   = -1, int ModeID7   = -1,
                                 int TimeID8   = -1, int GroupID8   = -1, int ModeID8   = -1,
                                 int TimeID9   = -1, int GroupID9   = -1, int ModeID9   = -1,
                                 int TimeID10  = -1, int GroupID10  = -1, int ModeID10  = -1);

    Q_INVOKABLE void spray(int ModeID, int GroupID);

    QString Topic;
    QByteArray System = "/$system";
    QByteArray productSN = "6i72kff5gcsql2wo";    //"d5wkpeecywzaaayr";
    QByteArray deviceSN;

    void HostDisconnected();
    QString HostMessageStr = "";

    void search(QString keywords, QStringList& devList); //从主机列表中搜索含有指定关键字的主机
    Q_INVOKABLE QStringList searchDev(QString keyWords); //界面传入关键字调用搜索功能
    Q_INVOKABLE bool searchSelectDev(QString sn);       //将搜索结果返回给界面

    QJsonObject getDevice(QString sn);
    bool setDevice(QJsonObject& dev);

    void setCurrentDevice(int index);
    void setCurrentDevice(QString sn);

    bool postDeviceInfoToServer(QString sn, QString& errorMsg);
    bool getDeviceInfoFromServer(QString sn, QJsonObject& devInfo, QString& errorMsg);  //从服务器获取主机和它的喷头信息
    bool updateLocalDevice(QString sn, QJsonObject& devInfo, bool autoSave = false);    //从服务器获取主机信息 转换成本地存储格式

    bool postSprayPlanToServer(QString sn, QString& errorMsg);
    bool getSprayPlanFromServer(QString sn, QJsonArray& plans, QString& errorMsg);          //消毒方案提交到服务器
    bool updateLocalSprayPlan(QString sn, QJsonArray& plans, bool autoSave = false);    //plan本地存储

    bool getMainCtrllerFromServer(QString sn, QJsonObject& mainCtrller, QString& errorMsg); //从服务器获取主机信息, 消毒方案
    bool updateLocalMainCtrller(QString sn, QJsonObject& mainCtrller, bool autoSave = false);    //主机信息 本地存储

    bool updateMainCtrllerFromServer(QString sn, QString& errorMsg, bool autoSave = false);        //从服务器获取主机信息 and 本地存储
    bool updateSprayPlanFromServer(QString sn, QString& errorMsg, bool autoSave = false);        //从服务器获取主机 Plans and 本地存储

    //for move data (old device data to the new Server):
    void moveData();
    int getSprayNum(QString sn);
    //to Our Server:    ///////////////////////////////////////////
    Q_INVOKABLE void downloadDevList();         //获取该用户下的所有主机列表
    //Q_INVOKABLE QJsonObject gpsDescription();   //GPS坐标提交到服务器
    //Q_INVOKABLE void postAddressInfo();         //安装位置提交到服务器
    Q_INVOKABLE void getIotPlan();              //从服务器获取主机、喷头信息
    Q_INVOKABLE void sumitIotPlan();            //主机、喷头信息 + 喷雾方案 提交到服务器
    Q_INVOKABLE bool postDeviceInfo();          //主机、喷头信息 提交到服务器
    Q_INVOKABLE bool dlCurrentDevInfo();        //从服务器获取当前主机信息, 消毒方案当前

    Q_INVOKABLE void readSpriklers();           //从主机获取当前主机喷头信息

    Q_INVOKABLE bool postSprayPlan();          //消毒方案提交到服务器
    Q_INVOKABLE bool getSprayPlan();          //消毒方案提交到服务器

    Q_INVOKABLE void sumitToServer(int numplan,     //配置消毒方案
                   bool isVarSpeed, bool isConstSpead,
                   bool isWorkday, bool isEveryday,
                   bool isTiming, bool isLoop,
                   QString Starttime1, QString Endtime1,
                   QString Spraytime1, QString Intervaltime1,
                   QString Starttime2, QString Endtime2,
                   QString Spraytime2, QString Intervaltime2,
                   QString Starttime3, QString Endtime3,
                   QString Spraytime3, QString Intervaltime3,
                   bool isSparyer01, bool isSparyer02, bool isSparyer03, bool isSparyer04, bool isSparyer05, bool isSparyer06, bool isSparyer07, bool isSparyer08,
                   bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12, bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16);

    ///////////////////////// to our Server  /////////////////////////

    //to Mainboard:         /////////////////////////////////////////    
    Q_INVOKABLE void configWorkWifi(QString WorkWifiSsid, QString WorkWifiPassword);          //配置Wifi
    Q_INVOKABLE void sumitSpray(QString Position, QString SprayID = "");    //配置单个喷头
    Q_INVOKABLE void sumitAllSpray();           //配置全部喷头
    Q_INVOKABLE void sumitPlan(int numplan,     //配置消毒方案
                   bool isVarSpeed, bool isConstSpead,
                   bool isWorkday, bool isEveryday,
                   bool isTiming, bool isLoop,
                   QString Starttime1, QString Endtime1,
                   QString Spraytime1, QString Intervaltime1,
                   QString Starttime2, QString Endtime2,
                   QString Spraytime2, QString Intervaltime2,
                   QString Starttime3, QString Endtime3,
                   QString Spraytime3, QString Intervaltime3,
                   bool isSparyer01, bool isSparyer02, bool isSparyer03, bool isSparyer04, bool isSparyer05, bool isSparyer06, bool isSparyer07, bool isSparyer08,
                   bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12, bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16);
    Q_INVOKABLE void getHostPlan();             //获取消毒方案

    Q_INVOKABLE void addWater();                        //一键加水
    Q_INVOKABLE void trySpray(int SprayID, int ModeId); //试喷

    //设备演示操作
    Q_INVOKABLE void sprayAll(int ModeID);  //全部喷雾
    Q_INVOKABLE void resetHost();   //重启
    Q_INVOKABLE void clearSpray();  //清除喷雾
    Q_INVOKABLE void stopWork(bool nowork); //停  机

    ///////////////////////// to Mainboard end  /////////////////////////

    Q_INVOKABLE void clearSprinklers();     //清空喷头

    bool setSprayID(QString spraySN, int ID);
    void onGotSpray(QString spraySN, int ID);
    int getSprayCountAndMaxID(int& maxID);

    bool parseSpraySNFromResp(QJsonObject& data);

    bool requestMainCtrlInfo(QString sn, MainCtrlInfo& info, QString& errorMsg);
    void enterWriteIotPlan(QString sn, MainCtrlInfo& info, QString& errorMsg); //currentStatus:true 表示停机, false表示复机（正常计划喷雾）
    void leaveWriteIotPlan(QString sn, MainCtrlInfo& info);    // lastStatus:true 表示停机, false表示复机（正常计划喷雾）

    void SendTimeout();
    void MessageClose();
    void ConnectTimeout();

    void RefreshDevicesList();
    void ViewSprayHeads();
    Q_INVOKABLE void deleteSpray(int index);    //本地删除喷头
    Q_INVOKABLE void debugCurrentDevice();

    bool isDeviceExist(QString sn);
    bool DeviceIdExits(QString DevicesId);
    bool SprayHyidExits(QString SprayHYId);
    void ClosePump();

    Q_INVOKABLE void uiDebug(QString msg);

    QTimer *timerSprayRowByRow;
    int pointerRowByRow = 0;
    int pointerOneByOne = 0;
    int pointerRowByRowId = 0;
    int pointerOneByOneId = 0;
    QTimer *timerSprayOneByOne;
    QTimer *timerSprayStop;

    Q_INVOKABLE void sprayRowByRow();
    Q_INVOKABLE void sprayOneByOne();
    Q_INVOKABLE void sprayByTime();

    int ResponseNum = 0;
    bool SprayIdExits(int SprayId);

    QTimer *timerClearSpray;
    QString ClearSprayState = "开始";

    iotMessage iotMsg;
    QString getIotMessage(QString url);
    void sprayStop();
    int getDuration0();

    qint64 startTick = 0;
    qint64 lastTime = 0;
    qint64 getProcessTime();

    int sprayTime = 12;
    Q_INVOKABLE QString getANDROID_VERSION_NAME();
    void emitHostResponse(QString Response, bool Visble = true, int secs = 3);
    void emitShowMessage(QString msg, bool visable = true, int secs = 10);
    void emitFirstShowMessage(QString msg, bool visable = true, int secs = 10);
    void updateDevices(QJsonObject downloadJson);
    void updatePlan();

    int loginFailedTimes = 0;
    Q_INVOKABLE bool login(QString username, QString password);
    Q_INVOKABLE QString getUserName();
    Q_INVOKABLE QString getPassWord();
    Q_INVOKABLE void uiShowMsg(QString msg);

    void SaveUserData();
    void ReadUserData();

    QMqttClient *mqtt;
    void initMqtt();
    void connectMqtt();

    iotSDK iot;
    void Sleep(int delay);
    QString province;
    QString city;
    QString district;
    void SetUIoTCoreDeviceProperty(QString propertyName, QJsonValue propertyValue);

    Q_INVOKABLE void sendOrderToHost();
    void processingSprinklersInformation(QJsonObject Rep);
    Q_INVOKABLE QStringList viewFiles();
    Q_INVOKABLE void uploadSprayRecord();

    QByteArray Spray;
    int Address;
};

#endif // MAINWORK_H
