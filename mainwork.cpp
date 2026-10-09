#include "mainwork.h"
#include "msgsocket.h"
#include "fyspraydef.h"
#include "iotmessage.h"

#include <malloc.h>

#define SUBMIT_WITH_RESPONSE  (false)   //SUBMIT_WITH_RESPONSE  根据主机返回的数据处理
#define FAIL_TIMES_TO_PASS  (2)
#define MAX_SEARCH_ITEM_COUNT   (50)    //搜索结果返回最大数量

enum MIX_DEFINES_
{
    SPEED_MODE_CONST = 1,
    SPEED_MODE_VAR = 2,

    TIMING_MODE_TIMER = 0,
    TIMING_MODE_LOOP = 1,

    MIX_DEFINES_MAX = 0xFFFF
};

bool NetDebug::enable = false;

void NetDebug::post(QByteArray &data)
{
    if (enable)
    {
        MsgSocket* pMsgSender = MsgSocket::getInstance();
        pMsgSender->sendMsg((byte*)data.data(), data.size());
    }
}

void NetDebug::post(QString msg)
{
    QByteArray data = msg.toUtf8();
    post(data);
}

void NetDebug::post(QJsonObject &data)
{
    QByteArray ba = JsonObject2ByteArray(data);
    post(ba);
}

void NetDebug::post(QJsonArray &data)
{
    QJsonObject obj;
    obj.insert("Array:", data);
    post(obj);
}

void NetDebug::setDebugAddr(QString ip)
{
    if (!ip.isEmpty())
    {
        enable = true;
        MsgSocket::getInstance()->setDebugAddr(ip.toStdString().c_str());
    }
}

//生产一个喷雾方案的JSon数据
int genPlanInfo(QJsonObject& plan, int numplan,
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
                bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12, bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16)
{
    plan = QJsonObject();

    plan.insert("isVarSpeed", isVarSpeed);
    plan.insert("isConstSpead", isConstSpead);
    plan.insert("isWorkday", isWorkday);
    plan.insert("isEveryday", isEveryday);
    plan.insert("isTiming", isTiming);
    plan.insert("isLoop", isLoop);

    plan.insert("Starttime1", Starttime1);
    plan.insert("Endtime1", Endtime1);
    plan.insert("Spraytime1", Spraytime1);
    plan.insert("Intervaltime1", Intervaltime1);

    plan.insert("Starttime2", Starttime2);
    plan.insert("Endtime2", Endtime2);
    plan.insert("Spraytime2", Spraytime2);
    plan.insert("Intervaltime2", Intervaltime2);

    plan.insert("Starttime3", Starttime3);
    plan.insert("Endtime3", Endtime3);
    plan.insert("Spraytime3", Spraytime3);
    plan.insert("Intervaltime3", Intervaltime3);

    QJsonArray PlanSpray;

    PlanSpray.insert(0, isSparyer01);
    PlanSpray.insert(1, isSparyer02);
    PlanSpray.insert(2, isSparyer03);
    PlanSpray.insert(3, isSparyer04);
    PlanSpray.insert(4, isSparyer05);
    PlanSpray.insert(5, isSparyer06);
    PlanSpray.insert(6, isSparyer07);
    PlanSpray.insert(7, isSparyer08);

    PlanSpray.insert(8, isSparyer09);
    PlanSpray.insert(9, isSparyer10);
    PlanSpray.insert(10, isSparyer11);
    PlanSpray.insert(11, isSparyer12);
    PlanSpray.insert(12, isSparyer13);
    PlanSpray.insert(13, isSparyer14);
    PlanSpray.insert(14, isSparyer15);
    PlanSpray.insert(15, isSparyer16);

    plan.insert("PlanSpray", PlanSpray);
    plan.insert("Plan" + QString::number(numplan), numplan + 1);

    return 0;
}

mainwork::mainwork()
{
    if(!QDir(rootPath).exists())
        QDir().mkpath(rootPath);
    if(!QDir(rootPath+dataPath).exists())
        QDir().mkpath(rootPath+dataPath);
    qDebug()<<"版本信息"<<ANDROID_VERSION_CODE<<ANDROID_VERSION_NAME;
}

QString mainwork::getANDROID_VERSION_NAME()
{
    QString ver = "V";
    ver += ANDROID_VERSION_NAME;
    ver += " ";
    ver += ANDROID_BUILD_DAY;

    return ver;
}

void mainwork::httpPageReady()
{
    QByteArray receivemsg = MapGet.tcpsender->readAll();
    QList<QByteArray> msgs = receivemsg.split(char(10));
    QByteArray addressMsg;
    QString state = "";

    udpw.dbg("\n" + receivemsg);
    udpw.dbg("receivemsg------------------------------------------------");

    int length = 0;
    Receivemsg.push_back(receivemsg);

    foreach (QByteArray msg, msgs)                                                                 //包的拼接 如果回复为多个包 本函数会触发多次
    {
        if(state == "")
        {
            QList<QByteArray> infos = msg.split(':');
            if(infos.size() == 2)
            {
                if(infos[0] == "Content-Length")
                    length = infos[1].toInt();
                if(length > Receivemsg.size())
                {
                    numOfPakge++;
                    return;
                }
            }
        }
    }

    msgs = Receivemsg.split(char(10));

    foreach (QByteArray msg, msgs)
    {
        if(msg.size() && msg[0] == '{')
        {
            state = "start";
        }
        udpw.dbg(msg);
        if(state == "start")
        {
            addressMsg.push_back(msg + char(10));
        }
    }

    MapGet.tcpsender->abort();
    MapGet.tcpsender->disconnectFromHost();
    MapGet.tcpsender->close();

    /*
    QJsonObject addrJson = ByteArray2JsonObject(addressMsg);                                       //解析高德地图返回的JSon数据
    QJsonObject regeocode = addrJson["regeocode"].toObject();
    QJsonObject addressComponent = regeocode["addressComponent"].toObject();
    QString country  = addressComponent["country"].toString();
    QString province = addressComponent["province"].toString();
    QString district = addressComponent["district"].toString();
    QString township = addressComponent["township"].toString();
    QJsonObject streetNumber = addressComponent["streetNumber"].toObject();
    QString street = streetNumber["street"].toString();
    QString number = streetNumber["number"].toString();
    */

    QJsonObject addrJson = ByteArray2JsonObject(addressMsg);                                       //解析腾讯地图返回的JSon数据
    QJsonObject result = addrJson["result"].toObject();
    QJsonObject addressComponent = result["address_component"].toObject();
    QString nation = addressComponent["nation"].toString();
    province = addressComponent["province"].toString();
    city = addressComponent["city"].toString();
    district = addressComponent["district"].toString();
    QString street = addressComponent["street"].toString();
    QString street_number = addressComponent["street_number"].toString();

    udpw.dbg(nation + province + city + district + street + street_number + " " + QString::number(numOfPakge + 1));
    emit addressArrive(/*nation + */province + city + district + street + street_number);
    Receivemsg = "";
    numOfPakge = 0;
}

void mainwork::httpConnected()  //高德地图 http://restapi.amap.com/v3/geocode/regeo?location=121.621972919,31.21087610&key=17479d86c0c6a0305024e1142351a0a4   //key是别人的
{
    /*
    QString request =
            QString("GET /v3/geocode/regeo?location=" +
                    QString::number(gpsd.nowLongitude,'f',6) + "," + QString::number(gpsd.nowLatitude,'f',6) +
                    "&key=17479d86c0c6a0305024e1142351a0a4 HTTP/1.1\r\n" +
                    "Host: restapi.amap.com\r\n"+
                    "Connection: keep-alive\r\n"+
                    "User-Agent: Mozilla/5.0\r\n" +
                    "Accept: text/html\r\n" +
                    "Accept-Encoding: deflate\r\n"+
                    "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n"
                    );
    */                                                                                                 //高德地图

    QString SK = "lEeQVUliBCxmJXemGcy9sWWYe937QVPS";
    QString nowLongitude = QString::number(gpsd.nowLongitude, 'f', 6);
    QString nowLatitude = QString::number(gpsd.nowLatitude, 'f', 6);

    QString sig = "/ws/geocoder/v1?key=D3JBZ-VQ2KO-QSLWW-SEMQ2-L4BOF-TYBXY&location=" + nowLatitude + "," + nowLongitude + SK ;

    QString sigMd5 = QCryptographicHash::hash (sig.toLatin1(), QCryptographicHash::Md5).toHex();

    QString request =
            QString("GET /ws/geocoder/v1?key=D3JBZ-VQ2KO-QSLWW-SEMQ2-L4BOF-TYBXY&location=" +
                    nowLatitude + "," + nowLongitude +
                    "&sig=" + sigMd5 + " HTTP/1.1\r\n" +
                    "Host: apis.map.qq.com\r\n"+
                    "Connection: keep-alive\r\n"+
                    "User-Agent: Mozilla/5.0\r\n" +
                    "Accept: application/json\r\n" +
                    "Accept-Encoding: identity\r\n"+
                    "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n"
                    );
    //腾讯地图https://apis.map.qq.com/ws/geocoder/v1?key=5Q5BZ-5EVWJ-SN5F3-K6QBZ-B3FAO-*****&location=28.7033487,115.8660847&sig=90da272bfa19122547298e2b0bcc0e50

    MapGet.tcpsender->write(request.toUtf8());
    udpw.dbg("\n" + request);
}

void mainwork::SaveUserData()
{
    QFile *file = new QFile(rootPath + dataPath + "/user.db");
    file->open(QIODevice::WriteOnly);
    UserData = (UserName + "," + PassWord + "," + LoginTime).toUtf8().toBase64();
    qDebug().noquote()<<__FUNCTION__<<UserData;
    file->write(UserData);
    file->close();
}

void mainwork::ReadUserData()
{
    QFile *file = new QFile(rootPath + dataPath + "/user.db");
    if(!file->exists()) return;
    file->open(QIODevice::ReadOnly);
    UserData = file->readAll();
    qDebug().noquote()<<__FUNCTION__<<UserData;
    QList<QByteArray> Userdata = QByteArray::fromBase64(UserData).split(',');
    if(Userdata.size()>0)   UserName = Userdata[0];
    if(Userdata.size()>1)   PassWord = Userdata[1];
    if(Userdata.size()>2)   LoginTime = Userdata[2].toInt();
    file->close();
}

bool mainwork::login(QString username, QString password)
{
    //QString msg = iotMsg.login("https://hysds.hyairo.vip/api/login/account", username, password);  //20220308 New address
    bool superAdmin = (username == "HyjAdmin" && password == "Hyj20201112");                      //超级用户
    if (superAdmin) return true;

    QString msg;
    bool pass;
    pass = iotMsg.login(username, password, msg);
    if(pass)
    {
        bool isUserChanged = false;
        if (username != UserName)
        {
            isUserChanged = true;
        }
        UserName = username;
        PassWord = password;
        LoginTime = GetCurrentTime();
        SaveUserData();

        NetDebug::post("iotMsg.getAccountPriority():" + QString::number(iotMsg.getAccountPriority()));
        msg = "登录成功，欢迎您 " + username;
        emitHostResponse(msg, true);

        emit uiSetPriority(iotMsg.getAccountPriority());

        NetDebug::post("to emit setAccount:" + UserName + "," + PassWord);
        emit setAccount(UserName, PassWord, isUserChanged);
    }
    else {
        //登录失败
        NetDebug::post("iotMsg.getAccountPriority():" + QString::number(iotMsg.getAccountPriority()));
        emit uiSetPriority(iotMsg.getAccountPriority());
        loginFailedTimes++;
        if (loginFailedTimes == FAIL_TIMES_TO_PASS)
        {
            msg = "多次登录失败!\n 再次登录失败将进入应用,\n 进行本地查看操作。";
            emitFirstShowMessage(msg, true, 5);
            return false;
        }
        else if (loginFailedTimes > FAIL_TIMES_TO_PASS)
        {
            return true;
        }
        else {
            emitFirstShowMessage("登录失败:\n" + msg, true, 5);
        }
    }

    return pass;
}

QString mainwork::getUserName()
{
    return UserName;
}

QString mainwork::getPassWord()
{
    return PassWord;
}

void mainwork::uiShowMsg(QString msg)
{
    emitHostResponse(msg, true);
}

QString mainwork::getIotMessage(QString url)
{
    qDebug()<<__FUNCTION__<<getProcessTime();
    QString iotmsg = iotMsg.getMessage(url);
    qDebug().noquote()<<__FUNCTION__<<JsonObject2String(String2JsonObject(iotmsg))<<getProcessTime();
    return iotmsg;
}

void mainwork::getAddress()
{
    //const QString addr("restapi.amap.com");                                                      //高德地图
    const QString addr("apis.map.qq.com");                                                         //腾讯地图

    MapGet.tcpsender->abort();
    MapGet.tcpsender->disconnectFromHost();
    MapGet.tcpsender->close();
    MapGet.tcpsender->connectToHost(addr, u_short(80));

    udpw.dbg("getAddress: " + MapGet.tcpsender->errorString());
}

bool mainwork::readDevicesData(QString& devicesString)
{
    QFile loadFile(rootPath + dataPath + "/Devices.json");
    if(!loadFile.open(QIODevice::ReadOnly))
    {
        devicesString = "";
        NetDebug::post("打开" + rootPath + dataPath + "/Devices.json出错");
        return false;
    }
    devicesString = loadFile.readAll();
    loadFile.close();

    return true;
}

bool mainwork::writeDevicesData(QString& devicesString)
{
    QFile loadFile(rootPath + dataPath + "/Devices.json");
    if(!loadFile.open(QIODevice::WriteOnly))
    {
        udpw.dbg("打开" + rootPath + dataPath + "/Devices.json出错");
        return false;
    }
    loadFile.write(devicesString.toUtf8());
    loadFile.close();

    return true;
}

void mainwork::RefreshDevicesList()
{
    QJsonArray DevicesTableArray;

    for(int i = 0; i < Devices.size(); i+=2)
    {
        QJsonArray DevicesRowArray;
        QJsonObject Device = Devices[i].toObject();
        Device.insert("allSpary", "");                                                              //不清空会卡顿
        Device.insert("Table", "");
        Device.insert("Plans", "");
        Device.insert("index", i);
        Device.insert("HYID", Device["HYID"].toString().replace("HYACHNSDS", ""));
        DevicesRowArray.push_back(Device);

        if(i < Devices.size() - 1)
        {
            Device = Devices[i + 1].toObject();
            Device.insert("allSpary", "");
            Device.insert("Table", "");
            Device.insert("Plans", "");
            Device.insert("index", i+1);
            Device.insert("HYID", Device["HYID"].toString().replace("HYACHNSDS", ""));
            DevicesRowArray.push_back(Device);
        }
        else
        {
            QJsonObject empty;
            empty.insert("HYID", "");
            empty.insert("Address", "");
            empty.insert("State", "");
            empty.insert("UpdateTime", "");
            empty.insert("index", 0);
            Device.insert("HYID", Device["HYID"].toString().replace("HYACHNSDS", ""));
            DevicesRowArray.push_back(empty);
        }
        DevicesTableArray.push_back(DevicesRowArray);
    }
    emit refreshDevicesList(DevicesTableArray);
}

void mainwork::init()
{
    bool ret;
    bool has_WRITE_EXTERNAL_STORAGE = true;
    bool has_ACCESS_COARSE_LOCATION = true;
    bool has_ACCESS_FINE_LOCATION = true;

    initDebug();

    //to do: 后续对权限进行检查提示
    ret = checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if (!ret)
    {
        has_WRITE_EXTERNAL_STORAGE = false;
        LogFile("no perission: WRITE_EXTERNAL_STORAGE");
    }

    ret = checkPermission("android.permission.ACCESS_COARSE_LOCATION");
    if (!ret)
    {
        has_ACCESS_COARSE_LOCATION = false;
        LogFile("no perission: ACCESS_COARSE_LOCATION");
    }
    ret = checkPermission("android.permission.ACCESS_FINE_LOCATION");
    if (!ret)
    {
        has_ACCESS_FINE_LOCATION = false;
        LogFile("no perission: ACCESS_FINE_LOCATION");
    }

    udpw.myname = "mainwork";

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &mainwork::update);
    timer->start(3000);

    timerRefresh = new QTimer(this);
    connect(timerRefresh, &QTimer::timeout, this, &mainwork::wifiRefresh);

    timerSendOneOder = new QTimer(this);
    connect(timerSendOneOder, &QTimer::timeout, this, &mainwork::SendOneOder);

    timerWritedTimeout = new QTimer(this);
    connect(timerWritedTimeout, &QTimer::timeout, this, &mainwork::SendTimeout);

    timerMessageClose = new QTimer(this);
    connect(timerMessageClose, &QTimer::timeout, this, &mainwork::MessageClose);

    timerConnectTimeout = new QTimer(this);
    connect(timerConnectTimeout, &QTimer::timeout, this, &mainwork::ConnectTimeout);

    timerClosePump = new QTimer(this);
    connect(timerClosePump, &QTimer::timeout, this, &mainwork::ClosePump);

    timerSprayRowByRow = new QTimer(this);
    connect(timerSprayRowByRow, &QTimer::timeout, this, &mainwork::sprayRowByRow);

    timerSprayOneByOne = new QTimer(this);
    connect(timerSprayOneByOne, &QTimer::timeout, this, &mainwork::sprayOneByOne);

    timerSprayStop = new QTimer(this);
    connect(timerSprayStop, &QTimer::timeout, this, &mainwork::sprayStop);

    timerClearSpray = new QTimer(this);
    connect(timerClearSpray, &QTimer::timeout, this, &mainwork::clearSpray);

    //initLog();
    loadDevices();

    connect(MapGet.tcpsender, &QTcpSocket::readyRead, this, &mainwork::httpPageReady) ;
    connect(MapGet.tcpsender, &QTcpSocket::connected, this, &mainwork::httpConnected) ;

    //    connect(Connecter.tcpsender, &QTcpSocket::readyRead, this, &mainwork::receiveHostMessages);

    connect(Connecter.tcpsender, &QTcpSocket::readyRead, this, &mainwork::HostMessage);
    connect(Connecter.tcpsender, &QTcpSocket::connected, this, &mainwork::HostConnected);
    connect(Connecter.tcpsender, &QTcpSocket::disconnected, this, &mainwork::HostDisconnected);

    ReadUserData();
    emit userDataArrive(UserName, PassWord, LoginTime);

    //initMqtt();

    iot.init("",
             productSN,
             "org-fqp0vb",
             "HLuTb8zkqOV5kcTkYkFon1Q2KQy5zAMdtPGPeaje",
             "cn-sh2",
             "58RU5cZtTCPKXuF8u5ClriLbtdDC6LYWVBMVaXdjLccrISdny0ikTSWLLTGdXJYhKT",
             "https://api-cn-sh2.iot.ucloud.cn");

    //    QString permissionInfo = "本应用还需要开启：";
    //    if (!has_WRITE_EXTERNAL_STORAGE) {}
}

void mainwork::initDebug()
{
    QFile loadFile(rootPath + dataPath + "/mix");
    if(!loadFile.open(QIODevice::ReadOnly)) return ;

    QString cfgString = loadFile.readAll();
    loadFile.close();

    if (!cfgString.isEmpty())
    {
        NetDebug::setDebugAddr(cfgString);
    }

    NetDebug::post(cfgString);
}

void mainwork::scanmode(QString mode)
{
    ScanMode = mode;
}

QString mainwork::getScannedSpray()
{
    return ScanSpray["HYID"].toString();
}

bool mainwork::DeviceIdExits(QString DevicesId)
{
    QString ExitsDevicesId;
    for(int i = 0; i < Devices.size(); i++)
    {
        ExitsDevicesId = Devices[i].toObject()["HYID"].toString();  //这行不能去掉 去掉会造成部分机型无法匹配
        if(DevicesId == ExitsDevicesId)
        {
            CurrentDeviceIndex = i;
            return true;
        }
    }
    return false;
}

QString mainwork::getqrcode(QString qr, QString cvm4)
{
    cvm4 = "";
    //    udpw.dbg(qr);
    //    udpw.dbg(QString(char(7)));
    NetDebug::post("getqrcode():" + qr);
    QStringList infoValue = qr.split(':');
    if(infoValue.size() != 2)   return "" ;
    QString left = infoValue[0];
    QString right = infoValue[1];

    if (ScanMode == "host")
    {
        if(left == "HYID" && right.left(2) == "HY" && right.mid(6, 5) == "SDSCM")
        {
            if(DeviceIdExits(right))
            {
                viewDevice(CurrentDeviceIndex);
            }
            else
            {
                //扫描主机二维码，获得 序列号、WiFi名、密码
                onNewHost(right);

                CurrentDevice.insert("UpdateTime", GetCurrentTime("yyyy.MM.dd hh:mm"));
                saveHost();
                //                CurrentDevice = QJsonObject();
                //                CurrentDevice.insert("HYID",right);
                //                CurrentDevice.insert("WIFI", getHostWifiName(right));//right.right(10));
                //                CurrentDevice.insert("PWD", getHostWifiPwd());//"fy12345678");
            }
            emit hostScanSuccess(CurrentDevice["HYID"].toString(), CurrentDevice["WIFI"].toString(), CurrentDevice["PWD"].toString());
            return "ok";
        }
    }
    else if (ScanMode == "spray")
    {
        if(infoValue[0] == "HYID" && right.left(2) == "HY" && right.mid(6, 5) == "SDSSH" )
        {
            ScanSpray.insert("HYID", infoValue[1]);
            ScanSpray.insert("State", "添加成功");
            ScanSpray.insert("UpdateTime", GetCurrentTime("yyyy.MM.dd hh:mm"));
            return "ok";
        }
    }

    return "";
}

void mainwork::update()
{
    bool connectWifi = false;
    QString currentWifiAP;

    if(WifiController.isWifiEnable() == "true")
    {
        currentWifiAP = WifiController.getConntectedWifiSSID();

        connectWifi = (currentWifiAP == CurrentDevice["WIFI"].toString() ||
                currentWifiAP == "\"" + CurrentDevice["WIFI"].toString() + "\"")
                && CurrentDevice["WIFI"].toString() != "";
        APState = connectWifi ? "已连接" : "未连接";

        if(currentWifiAP.toLower() == "<unknown ssid>" || currentWifiAP.toLower() == "\"<unknown ssid>\"")
        {
            APState = "定位服务未开启";
        }
    }
    else {
        APState ="Wifi未开启";
    }
    //QStringList location = WifiController.getgpslocation().split(',');
    WifiRightConnected = connectWifi;
    emit updatestate(APState + " , " + currentWifiAP,
                     "经" + QString::number(gpsd.nowLongitude,'f',6) + ",纬" + QString::number(gpsd.nowLatitude,'f',6),
                     connectWifi);
}

void mainwork::sumitAllSpray()
{
    NetDebug::post(SprayHeads);

    for (int i = 0; i < SprayHeads.size(); ++i)
    {
        if (i > 0)
        {
            Sleep(1000);
        }

        QJsonObject spray = SprayHeads[i].toObject();
        QString sn = spray["HYID"].toString();
        int ID = spray["SprayID"].toString().toInt();

        NetDebug::post(sn + ", id:" + QString::number(ID));
        emitHostResponse("配喷头:" + sn, true);
        setSprayID(sn, ID);
    }
}

bool mainwork::SprayHyidExits(QString SprayHYId)
{
    for (int i = 0; i < SprayHeads.size(); ++i)
    {
        if(SprayHeads[i].toObject()["HYID"] == SprayHYId)
        {
            CurrentSprayindex = i;
            return true;
        }
    }
    return false;
}

bool mainwork::SprayIdExits(int SprayId)
{
    for (int i = 0; i < SprayHeads.size(); i++)
    {
        if(SprayHeads[i].toObject()["SprayID"].toString().toInt() == SprayId)
            return true;
    }
    return false;
}

inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

void mainwork::sumitSpray(QString Position, QString SprayID)
{
    ScanSpray.insert("Position", Position);
    CurrentSpray = ScanSpray;

    QString method = SprayHyidExits(CurrentSpray["HYID"].toString()) ? "更新" : "新增";

    if(method == "更新")
    {
        CurrentSpray.insert("Plan", SprayHeads[CurrentSprayindex].toObject()["Plan"].toString());
        CurrentSpray.insert("SprayID", SprayHeads[CurrentSprayindex].toObject()["SprayID"].toString());
        SprayHeads.removeAt(CurrentSprayindex);
    }

    if(method == "新增")
    {
        QStringList SprayIDs;
        for (int i = 0; i < 239; ++i)                                                              //ID范围:1-239    0x01 - 0xEF
        {
            SprayIDs.push_back(QString::number(i));
        }

        for (int i = 0; i < SprayHeads.size(); i++)
        {
            SprayIDs[ SprayHeads[i].toObject()["SprayID"].toString().toInt() ] = "used";
        }

        int back = 0;
        for (int i = 0; i < SprayIDs.size(); i++)
        {
            i += back;
            back = 0;
            if(SprayIDs[i] == "used")
            {
                SprayIDs.removeAt(i);
                back = -1;
            }
        }

        CurrentSpray.insert("Plan", "");
        CurrentSpray.insert("SprayID", SprayIDs[1]);
        CurrentSprayindex = SprayHeads.size();
    }

    if(SprayID != "")
    {
        CurrentSpray.insert("SprayID", SprayID);
    }
    QJsonObject ControlOderData;
    ControlOderData.insert("SprayID", CurrentSpray["SprayID"].toString().toInt());
    ControlOderData.insert("SpraySN", CurrentSpray["HYID"].toString());
    SendOder(SetSprayHeadID, ControlOderData);

    SprayHeads.insert(CurrentSprayindex, CurrentSpray);

    std::sort(SprayHeads.begin(), SprayHeads.end(), [](const QJsonValue &v1, const QJsonValue &v2)
    {
        return v1.toObject()["SprayID"].toString().toInt() < v2.toObject()["SprayID"].toString().toInt();
    }); //必须使用 inline void swap(QJsonValueRef v1, QJsonValueRef v2)...

    SpraysData.insert("data", SprayHeads);
    SpraysString = JsonObject2String(SpraysData);
    if(CurrentDevice["HYID"].toString() == "") return;
    //debug(CurrentDevice["HYID"].toString());
    CurrentDevice.insert("allSpray", SpraysData);
    saveHost();                                                                                   //会修改CurrentSpray
}

void mainwork::ViewSprayHeads()
{
    QJsonArray SprayHeadsTableArray;

    for(int i = 0; i < SprayHeads.size(); i+=2)
    {
        QJsonArray SprayHeadsRowArray;
        QJsonObject Spray = SprayHeads[i].toObject();
        Spray.insert("index", i);
        Spray.insert("HYID", Spray["HYID"].toString().replace("HYACHNSDS", ""));
        SprayHeadsRowArray.push_back(Spray);

        if(i < SprayHeads.size() - 1)
        {
            Spray = SprayHeads[i + 1].toObject();
            Spray.insert("index", i+1);
            Spray.insert("HYID", Spray["HYID"].toString().replace("HYACHNSDS", ""));
            SprayHeadsRowArray.push_back(Spray);
        }
        else
        {
            QJsonObject empty;
            empty.insert("HYID", "");
            empty.insert("Position", "");
            empty.insert("State", "");
            empty.insert("Plan", "");
            empty.insert("SprayID", "");
            empty.insert("UpdateTime", "");
            empty.insert("index", 0);
            Spray.insert("HYID", Spray["HYID"].toString().replace("HYACHNSDS", ""));
            SprayHeadsRowArray.push_back(empty);
        }
        SprayHeadsTableArray.push_back(SprayHeadsRowArray);
    }
    emit sprayScanSuccess(SprayHeadsTableArray);
}

void mainwork::saveHostInfo(QString MapCoordinates, QString Address, QString WorkWifiSsid, QString WorkWifiPassword)
{
    CurrentDevice.insert("MapCoordinates", MapCoordinates);
    CurrentDevice.insert("Address", Address);
    CurrentDevice.insert("WorkWifiSsid", WorkWifiSsid);
    CurrentDevice.insert("WorkWifiPassword", WorkWifiPassword);
    CurrentDevice.insert("UpdateTime", GetCurrentTime("yyyy.MM.dd hh:mm"));
    saveHost();
}

void mainwork::saveHost()
{
    if(CurrentDevice["HYID"].toString() == "")  return;
    QString method = DeviceIdExits(CurrentDevice["HYID"].toString()) ? "更新" : "新增";
    if(method == "更新")
    {
        CurrentDevice.insert("State", "已更新");
        Devices.removeAt(CurrentDeviceIndex);
    }

    if(method == "新增")
    {
        CurrentDevice.insert("State", "配置成功");
        CurrentDeviceIndex = Devices.size();
    }

    Devices.insert(CurrentDeviceIndex, CurrentDevice);

    RefreshDevicesList();
    viewDevice(CurrentDeviceIndex);         //eventloop结束后删除对象

    saveDevices();
}

void mainwork::LogFile(QByteArray& info)
{
    static bool isFirst = true;
    QFile logFile(rootPath + dataPath + "/log.txt");

    if (isFirst)
    {
        isFirst  = false;
        if(!logFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            return;
        }
    }
    else {
        if(!logFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            return;
        }
    }

    // ？？？
    {
        QDateTime now = QDateTime::currentDateTime();
        logFile.write(now.toString("MM-dd hh:mm:ss.zzz ").toUtf8());
        logFile.write(info);
        logFile.write("\r\n");
        logFile.close();
    }
}
void mainwork::LogFile(QJsonObject &info)
{
    QByteArray ba;
    ba = JsonObject2ByteArray(info);
    LogFile(ba);
}

void mainwork::LogFile(QString info)
{
    QByteArray ba;
    ba = info.toUtf8();
    LogFile(ba);
}

bool mainwork::saveDevices()
{
    QJsonObject DevicesData;
    DevicesData.insert("data", Devices);
    DevicesData.insert("Updatetime", GetCurrentTime("yyyy.MM.dd hh:mm:ss"));

    QString DevicesString;
    DevicesString = JsonObject2String(DevicesData);

    bool ret = writeDevicesData(DevicesString);
    return ret;
}

bool mainwork::loadDevices()
{
    QString devicesString;
    readDevicesData(devicesString);
    QJsonObject DevicesData;
    DevicesData = String2JsonObject(devicesString);

    Devices = DevicesData["data"].toArray();
    Plans = Devices[0].toObject()["Plans"].toArray();
    if(Plans.empty())
    {
        Plans.insert(0, QJsonObject());
        Plans.insert(0, QJsonObject());
        Plans.insert(0, QJsonObject());
    }

    CachePlans = Plans;
    RefreshDevicesList();
    return true;
}

void mainwork::debugCurrentDevice()
{
#if 1                                                                                              //修复无法远程升级的主机
    QByteArray OderByteArray =                                                                     //12个小时失效，需要用的时候再更新新的命令
            "{"
            "\"Method\":\"update_firmware\","
            "\"Payload\":"
            "{"
            "\"Module\":\"default\","
            "\"Version\":\"1_12_0_2b\","
            "\"URL\":\"http://uiot-ota1.cn-sh2.ufileos.com/6i72kff5gcsql2wo_1_12_0_2b_Update_0112.bin?"
            "UCloudPublicKey=dhXd1zwB367CNkw1sSL2qVjybhBBjkQapw%2BDH3tqkhwvN%2F0B6EckE%2BCZ%2FFI%3D&"
            "Signature=LgIMqF4oj0PuRiHPC8SA9sjcGoo%3D&"
            "Expires=1619748457\","
            "\"MD5\":\"4fb2d98bfbe5007503e4db3c95b7f959\","
            "\"Size\":55276"
            "}"
            "}";
    Connecter.tcpsender->write(OderByteArray);
    if(Connecter.tcpsender)
    {
        qDebug() << "发送到主机";
    }
    else
    {
        qDebug() << "未发送到主机";
    }
    qDebug().noquote()<<__FUNCTION__<<JsonObject2ByteArray(String2JsonObject(OderByteArray));
#endif
    //getIotMessage("http://iot.fyairo.com/api/devices/publish?SN=HYACHNSDSCM0202009250001&Msg=hello");
    //getIotMessage("http://iot.fyairo.com/api/usage");
#if 0
    iot.DeviceSN = CurrentDevice["HYID"].toString().toLatin1();
    qDebug().noquote()<<__FUNCTION__<<JsonObject2ByteArray(String2JsonObject(iot.Action("GetUIoTCoreDeviceInfo")));     //测试iotApi
#endif
}

bool mainwork::isDeviceExist(QString sn)
{
    QString ExitsDevicesId;
    for(int i = 0; i < Devices.size(); i++)
    {
        ExitsDevicesId = Devices[i].toObject()["HYID"].toString();  //这行不能去掉 去掉会造成部分机型无法匹配
        if(sn == ExitsDevicesId) return true;
    }

    return false;
}

qint64 mainwork::getProcessTime()
{
    qint64 processTime = getCurrentMSecsSinceEpoch() - lastTime;
    lastTime = getCurrentMSecsSinceEpoch();
    return processTime;
}

void mainwork::configWorkWifi(QString WorkWifiSsid, QString WorkWifiPassword)
{
    QJsonObject ControlOderData;

    if (WorkWifiSsid.compare(CurrentDevice["WorkWifiSsid"].toString()) != 0 ||
            WorkWifiPassword.compare(CurrentDevice["WorkWifiPassword"].toString()) != 0)
    {
        CurrentDevice.insert("WorkWifiSsid", WorkWifiSsid);
        CurrentDevice.insert("WorkWifiPassword", WorkWifiPassword);
        CurrentDevice.insert("UpdateTime", GetCurrentTime("yyyy.MM.dd hh:mm"));

        NetDebug::post(QString("WorkWifiSsid:" + WorkWifiSsid + ",WorkWifiPassword:" + WorkWifiPassword));
    }

    ControlOderData.insert("RouterName", WorkWifiSsid);
    ControlOderData.insert("RouterPassword", WorkWifiPassword);

    SendOder(ConfigNetWork, ControlOderData);
}

void mainwork::ConnectTimeout()
{
    Connecter.tcpsender->abort();
    Connecter.tcpsender->disconnectFromHost();
    Connecter.tcpsender->close();
    netState = "没有连接";
    timerConnectTimeout->stop();
    qDebug()<<__FUNCTION__<<"连接主机TCP端口超时";
    emitHostResponse("连接主机TCP端口超时", true);
    Oders = QJsonArray();
}

void mainwork::Sleep(int delay)
{
    QTimer *Delay = new QTimer(this);
    Delay->start(delay);

    QEventLoop loop;
    connect(Delay, &QTimer::timeout, &loop, &QEventLoop::quit) ;
    loop.exec();
}

//根据主机返回的数据处理
bool mainwork::SendPlan(ControlOders Cmd, QJsonObject Data)
{
    bool ret = true;

    QJsonObject Oder;
    Oder.insert("Cmd", Cmd);
    Oder.insert("Data", Data);

    if(APState == "已连接")
    {
        Oders.push_back(Oder);
        if( netState == "没有连接")
        {
            Connecter.tcpsender->abort();
            Connecter.tcpsender->connectToHost("192.168.1.119", u_short(10086));
            netState = "正在连接";
            timerConnectTimeout->start(10 * 1000);
            qDebug()<<__FUNCTION__<<"开始连接主机" <<" "<<getProcessTime()<<"ms";
            emitHostResponse("开始连接主机", true);
        }

        if(netState == "待命")
        {
            timerSendOneOder->start(1 * 1000);
        }
    }
    else
    {
        QString DeviceSN = CurrentDevice["HYID"].toString();
        QString errorMsg;

        ret = iotMsg.sendPlan(DeviceSN, Cmd, Data, errorMsg);
        if (!ret)
        {
            if (!errorMsg.isEmpty())
            {
                emitHostResponse(errorMsg, true);
            }
        }
        //Sleep(100);
    }

    return ret;
}

bool mainwork::SendOder(ControlOders Cmd, QJsonObject Data, int deciSecond)
{
    bool ret = true;

    QJsonObject Oder;
    Oder.insert("Cmd", Cmd);
    Oder.insert("Data", Data);

    if(APState == "已连接")
    {
        Oders.push_back(Oder);
        if(netState == "没有连接")
        {
            Connecter.tcpsender->abort();
            Connecter.tcpsender->connectToHost("192.168.1.119", u_short(10086));
            netState = "正在连接";
            timerConnectTimeout->start(10 * 1000);
            qDebug()<<__FUNCTION__<<"开始连接主机" <<" "<<getProcessTime()<<"ms";
            emitHostResponse("开始连接主机", true);
        }

        if(netState == "待命")
        {
            timerSendOneOder->start(deciSecond * 100);
        }
    }
    else
    {
        QString DeviceSN = CurrentDevice["HYID"].toString();
        QByteArray re = iotMsg.action(DeviceSN, Cmd, Data);

        QJsonObject reJson = ByteArray2JsonObject(re);
        QJsonObject data;
        QString msg;// = reJson["Message"].toString();

        int code = iotMsg.pickData(reJson, data);
        if(code == 0)
        {
            msg = QString::number(ResponseNum++) + ": " + "云指令发送成功";
        }
        else
        {
            if (code == -1)
            {
                msg = "请检查网络";
            }
            else {
                msg = "主机不在线或异常";
            }
            ret = false;
        }
        emitHostResponse(msg, true);
        Sleep(100);
    }

    return ret;
}

void mainwork::emitHostResponse(QString Response, bool Visble, int secs)
{
    //qDebug().noquote()<<__FUNCTION__<<Response<<Visble;
    emit hostResponse(Response, Visble);
    timerMessageClose->start(secs * 1000);                                                            // 提示信息展示时间
}

void mainwork::MessageClose()
{
    emit hostResponse("消息关闭", false);
    emit showMessage("消息关闭", false);
    emit firstShowMessage("消息关闭", false);
    timerMessageClose->stop();
}

void mainwork::emitShowMessage(QString msg, bool visable, int secs)
{
    qDebug().noquote()<<__FUNCTION__<<msg<<visable;
    emit showMessage(msg, true);
    timerMessageClose->start(secs * 1000);                                                            // 提示信息展示时间
}

void mainwork::emitFirstShowMessage(QString msg, bool visable, int secs)
{
    emit firstShowMessage(msg, true);
    timerMessageClose->start(secs * 1000);                                                            // 提示信息展示时间
}

void mainwork::HostConnected()
{
    qDebug()<<__FUNCTION__<<"已连接到主机Tcp端口"<<" "<<getProcessTime()<<"ms";
    emitHostResponse("已连接到主机Tcp端口", true);

    timerConnectTimeout->stop();
    netState = "待命";
    timerSendOneOder->start(1 * 1000);
}

void mainwork::HostDisconnected()
{
    netState = "没有连接";
    timerWritedTimeout->stop();
    timerConnectTimeout->stop();
    timerSendOneOder->stop();
    qDebug()<<__FUNCTION__<<"已从主机断开";
    emitHostResponse("已从主机断开", true);
}

void mainwork::search(QString keywords, QStringList &devList)
{
    QString sn, content;
    QString dev;
    for(int i =0; i < Devices.size(); i++)
    {
        sn = Devices[i].toObject()["HYID"].toString();                         //这行不能去掉 去掉会造成部分机型无法匹配
        content = Devices[i].toObject()["Address"].toString();

        if ((sn.indexOf(keywords, 0, Qt::CaseInsensitive) >= 0) || (content.indexOf(keywords, 0, Qt::CaseInsensitive) >= 0))
        {
            dev = "SN:"+ sn + "\n";
            dev += "地址:" + content;
            devList.append(dev);

            if (devList.size() >= MAX_SEARCH_ITEM_COUNT)
                break;
        }
    }
}

QStringList mainwork::searchDev(QString keyWords)
{
    QStringList ret;
    if (!keyWords.isEmpty())
    {
        search(keyWords, ret);
    }

    return ret;
}

QString pickSN(QString content)
{
    QString sn;
    int pos;
    pos = content.indexOf("\n");

    if (pos < 0) return sn;
    content = content.left(pos);
    pos = content.indexOf("SN:");

    if (pos < 0) return sn;
    sn = content.right(content.size() - pos -3);

    return sn;
}

bool mainwork::searchSelectDev(QString sn)
{
    bool ret = false;
    if (sn.isEmpty())
    {
        return ret;
    }
    QString searchSN = pickSN(sn);
    NetDebug::post(QString::asprintf("%s(): select: ", __FUNCTION__) + searchSN);

    if (searchSN.isEmpty())
    {
        return ret;
    }
    setCurrentDevice(searchSN);

    return true;
}

QJsonObject mainwork::getDevice(QString sn)
{
    NetDebug::post(QString::asprintf("%s(): line:%d", __FUNCTION__, __LINE__));

    QJsonObject dev;
    QString devSN;
    for(int i = 0; i < Devices.size(); i++)
    {
        devSN = Devices[i].toObject()["HYID"].toString();                         //这行不能去掉 去掉会造成部分机型无法匹配
        if(sn == devSN)
        {
            NetDebug::post(QString::asprintf("%s(): line:%d, find at %d", __FUNCTION__, __LINE__, i));
            dev = Devices[i].toObject();
            break;
        }
    }

    return dev;
}

bool mainwork::setDevice(QJsonObject &dev)
{
    NetDebug::post(QString::asprintf("%s(): line:%d", __FUNCTION__, __LINE__));

    QString sn = dev["HYID"].toString();
    QString tempSN;
    QString state = "配置成功";
    int i = 0;
    bool find = false;

    for(i = 0; i < Devices.size(); i++)
    {
        tempSN = Devices[i].toObject()["HYID"].toString();
        if(sn == tempSN)
        {
            find = true;
            state = "已更新";
            Devices.removeAt(i);
            break;
        }
    }

    dev.insert("State", state);
    Devices.insert(i, dev);
    NetDebug::post(QString::asprintf("Device count: %d", Devices.size()));

    return true;
}

void mainwork::saveAllDevices()
{
#ifdef  MOVE_DATA_OPER
    NetDebug::post(QString::asprintf("to do... %s(): line:%d", __FUNCTION__, __LINE__));

    QFile loadFile(rootPath + dataPath + "/DevicesNew.json");
    if(!loadFile.open(QIODevice::WriteOnly))
    {
        NetDebug::post("打开" + rootPath + dataPath + "/DevicesNew.json出错");
        return;
    }

    QJsonObject DevicesData;
    DevicesData.insert("data", Devices);
    DevicesData.insert("Updatetime", GetCurrentTime("yyyy.MM.dd hh:mm:ss"));

    QString DevicesString;
    DevicesString = JsonObject2String(DevicesData);

    //NetDebug::post(DevicesString);

    qint64 ret = loadFile.write(DevicesString.toUtf8());
    if (ret == -1)
    {
        NetDebug::post("save error");
    }
    //NetDebug::post(QString::asprintf("save:%d", ret));
    loadFile.close();

#else
    saveDevices();
#endif

    return;
}

void mainwork::setCurrentDevice(int index)
{
    NetDebug::post(QString::asprintf("%s(): line:%d  index %d", __FUNCTION__, __LINE__, index));
    viewDevice(index);
}

void mainwork::setCurrentDevice(QString sn)
{
    NetDebug::post(QString::asprintf("%s(): line:%d", __FUNCTION__, __LINE__));

    int index = 0;
    QString devSN;
    for(int i = 0; i < Devices.size(); i++)
    {
        devSN = Devices[i].toObject()["HYID"].toString();   //这行不能去掉 去掉会造成部分机型无法匹配
        if(sn == devSN)
        {
            index = i;
            break;
        }
    }

    setCurrentDevice(index);
}

bool mainwork::postDeviceInfoToServer(QString sn, QString &errorMsg)
{
    NetDebug::post(QString::asprintf("%s(): line:%d", __FUNCTION__, __LINE__));

    QJsonObject property;
    QJsonObject item;
    QString propName;
    QString val;
    bool ret;

    QJsonObject dev = getDevice(sn);
    QJsonArray sprays =  dev["allSpray"].toObject()["data"].toArray();

    if (dev["HYID"].toString() != sn)
    {
        errorMsg = "Can't find " + sn;
        return  false;
    }

    //    bool ret = iotMsg.deviceRegister(sn, errorMsg);
    //    if (!ret)
    //    {
    //        return  false;
    //    }

    //Address
    item = QJsonObject();
    propName = "Address";
    item.insert("Name", propName);
    val = dev["Address"].toString().toUtf8();
    item.insert("Value", val);
    val = dev["UpdateTime"].toString().toUtf8();
    item.insert("LastUpdateTime", val);
    property.insert(propName, item);

    //Location
    item = QJsonObject();
    propName = "Location";
    item.insert("Name", propName);
    val = dev["MapCoordinates"].toString().toUtf8();
    item.insert("Value", val);
    val = dev["UpdateTime"].toString().toUtf8();
    item.insert("LastUpdateTime", val);
    property.insert(propName, item);

    //NumOfSprinklers
    propName = "NumOfSprinklers";
    item.insert("Name", propName);
    item.insert("Value", QString::number(sprays.size()));
    val = dev["UpdateTime"].toString().toUtf8();
    item.insert("LastUpdateTime", val);
    property.insert(propName, item);

    //TaskPlanLastUpdateTime
    propName = "TaskPlanLastUpdateTime";
    item.insert("Name", propName);
    val = dev["TaskPlanLastUpdateTime"].toString().toUtf8();
    item.insert("Value", val);
    item.insert("LastUpdateTime", val); //update time
    property.insert(propName, item);

    //Organization
    propName = "Organization";
    item.insert("Name", propName);
    val = dev["Organization"].toString().toUtf8();
    item.insert("Value", val);
    val = dev["UpdateTime"].toString().toUtf8();
    item.insert("LastUpdateTime", val); //update time
    property.insert(propName, item);

    ret = iotMsg.deviceUpdate(sn, property, errorMsg);
    if (ret)
    {
        errorMsg = "提交主机信息成功";
    }
    else {
        errorMsg = "提交主机信息失败:\n" + errorMsg;
        return false;
    }

    //sprinkler:
#define MAX_SPRINKLER_NUMBER    (16)
    int SprayID;
    QString sprinklerSN;
    QString pos;
    QString datetime;

    int count = 0;
    int Heads[MAX_SPRINKLER_NUMBER+1] = {0};
    foreach (QJsonValue SprayHead, sprays)
    {
        SprayID = SprayHead.toObject()["SprayID"].toString().toInt();
        sprinklerSN = SprayHead.toObject()["HYID"].toString();
        pos = SprayHead.toObject()["Position"].toString().toUtf8();
        datetime = SprayHead.toObject()["UpdateTime"].toString();

        if (SprayID < MAX_SPRINKLER_NUMBER)
        {
            Heads[SprayID] = 1;

            ret = iotMsg.sprinklerRegister(sn, sprinklerSN, SprayID, errorMsg);
            if (ret)
            {
                QJsonObject sprayProp;
                QJsonObject item;

                item.insert("Name", "Position");
                item.insert("Value", pos);
                item.insert("LastUpdateTime", datetime);
                sprayProp.insert("Position", item);

                ret = iotMsg.sprinklerUpdate(sn, SprayID, sprayProp, errorMsg);
                count++;
            }
        }
        else {
            errorMsg = sprinklerSN+ " sprinkler ID error: " + QString::number(SprayID);
            emitHostResponse(errorMsg,true);
        }
    }

    errorMsg = "提交主机信息完成(含喷头数: " + QString::number(count) + ")";
    emitHostResponse(errorMsg, true);

    return true;
}

void mainwork::downloadDevList()
#if 1
{
    QJsonObject dlDevList;
    QString errorMsg;
    int devCount = 0;
    int dlCount = 0;

    errorMsg = "正在从云端获取设备列表......";
    emitHostResponse(errorMsg, true);
    bool ret = iotMsg.deviceList(dlDevList, errorMsg);
    if (ret)
    {
        devCount = dlDevList["Count"].toInt();
        if (devCount <= 0)
        {
            errorMsg = "服务器上没有设备信息!";
        }
        else {
            QString sn;
            QJsonObject device;
            QJsonArray dlDevArray = dlDevList["Devices"].toArray();
            QJsonArray deviceArray;

            if (dlDevArray.size() != devCount)
            {
                NetDebug::post(QString::asprintf("Warning!!! Devices count %d, but Device Array count %d", devCount, dlDevArray.size()));
                devCount = dlDevArray.size();
            }

            for (int i = 0; i < devCount; ++i) {
                QJsonObject devInfo;

                device = dlDevArray[i].toObject();
                sn = device["DeviceSN"].toString();

                if (sn.isEmpty())
                    continue;

                emitHostResponse("下载 " + sn, true);
                ret = updateMainCtrllerFromServer(sn, errorMsg,false);
                if (ret)
                {
                    dlCount++;
                }
            }
            errorMsg = QString::asprintf("下载设备列表成功(主机数:%d)", dlCount);

#ifdef  MOVE_DATA_OPER
            moveData();
#endif
            saveAllDevices();
            RefreshDevicesList();
            setCurrentDevice(0);
        }
    }
    else {
        errorMsg = "下载设备列表失败:\n" + errorMsg;
    }
    emitHostResponse(errorMsg,true);
}
#else
{
    QJsonObject dlDevList;
    QString errorMsg;

    errorMsg = "正在从云端获取设备列表......";
    emitShowMessage(errorMsg, true);
    bool ret = iotMsg.deviceList(dlDevList, errorMsg);
    if (ret)
    {
        int count = dlDevList["Count"].toInt();

        if (count <= 0)
        {
            errorMsg = "服务器上没有设备信息!";
        }
        else {
            QString sn;
            QJsonObject device;
            QJsonArray dlDevArray = dlDevList["Devices"].toArray();
            QJsonArray deviceArray;

            if (dlDevArray.size() != count)
            {
                count = dlDevArray.size();
            }

            for (int i = 0; i < count; ++i) {
                QJsonObject devInfo;

                device = dlDevArray[i].toObject();
                sn = device["DeviceSN"].toString();

                if (sn.isEmpty())
                {
                    continue;
                }

                ret = getDeviceInfoFromServer(sn, devInfo, errorMsg);
                NetDebug::post(devInfo);

                if (ret)
                {
                    deviceArray.insert(i, devInfo);
                }

                updateLocalDevice(sn, devInfo);
            }

            saveDevices();
            //to do: update current device...

            //DownloadDevices["downloadDevices"] = deviceArray;

            errorMsg = "下载设备列表成功";
            //NetDebug::post(DownloadDevices);
        }
    }
    else {
        errorMsg = "下载设备列表失败:\n" + errorMsg;
    }

    emitShowMessage(errorMsg, false);
    emitHostResponse(errorMsg,true);
}
#endif

void mainwork::HostMessage()
{
    QByteArray hostMessage = Connecter.tcpsender->readAll();
    HostMessageStr = hostMessage;
    QJsonObject hostMessageJson = String2JsonObject(HostMessageStr);

    if(Oders.size())
    {
        workState = "队列执行中";
        //timerSendOneOder->start(100);                                                              // 接收到回复后等待主机写入
        timerSendOneOder->start(600);
    }
    else
    {
        Connecter.tcpsender->close();
        netState = "没有连接";
        workState = "队列完成";
        hostMessage += " " + workState;
    }
    timerWritedTimeout->stop();

    if(hostMessageJson.size())
    {
        qDebug().noquote()<<__FUNCTION__<<QString("主机发来Json信息:\r\n")
                         <<HostMessageStr<<hostMessageJson.size()<<workState;

        //NetDebug::post(hostMessageJson);

        LogFile("Recv:" + JsonObject2String(hostMessageJson));

        updateDevices(hostMessageJson);

        hostMessage = "收到主机应答"; // + QString::number(HostMessageStr.length()).toLatin1() + "字节";
    }
    else
    {
        hostMessage.replace("SetPenWuMode", "设置喷雾模式");
        hostMessage.replace("SetDayTimer", "设置日定时表");
        hostMessage.replace("SetGroupTable", "设置分组表");
        hostMessage.replace("SetDayTask", "设置当日任务");
        hostMessage.replace("SetWeekTask", "设置周任务");
        hostMessage.replace("OK", "成功");
        hostMessage.replace("CtrlAlonePenIdWnMode", "试喷");
        hostMessage.replace("ExecuteTask", "执行任务");
        hostMessage.replace("StopTask", "停止任务");
        hostMessage.replace("CleanTask", "清除喷雾");
        hostMessage.replace("SystemReset", "重新启动");
        hostMessage.replace("CtrlShuibeng", "控制水泵");
        hostMessage.replace("SET_ID", "设置编号");
        hostMessage.replace("ConfigWifi", "配置工作网络");
        qDebug().noquote()<<__FUNCTION__<< QString("主机发来信息:\r\n")
                         << hostMessage <<" "<<getProcessTime()<<"ms";
    }

    emitHostResponse(QString::number(ResponseNum++) + ": " + hostMessage, true);
}

void mainwork::SendOneOder()
{
    if(netState == "待命" && Oders.size())
    {
        QByteArray OderByteArray = shortJson(JsonObject2ByteArray(Oders.first().toObject()));

        qDebug().noquote()<<__FUNCTION__<<QString(OderByteArray)<<" buff Size:"<<OderByteArray.size() <<" "<<getProcessTime()<<"ms";

        LogFile("Send:" + QString::fromUtf8(OderByteArray));

        Connecter.tcpsender->write(OderByteArray);
        Oders.removeFirst();
        timerWritedTimeout->start(5 * 1000);    //主机回复超时时间  Wifi拥挤应加大两个超时时间
    }
    timerSendOneOder->stop();
}

void mainwork::SendTimeout()
{
    netState = "待命";
    timerSendOneOder->start(4 * 1000);  //主机回复超时 发送下一条 设置周表会比较长
}

void mainwork::viewDevice(int index)
{
    CurrentDeviceIndex = index;
    CurrentDevice = Devices[index].toObject();
    SpraysData = CurrentDevice["allSpray"].toObject();
    SprayHeads = SpraysData["data"].toArray();
    if(SprayHeads.size())   CurrentSpray = SprayHeads[0].toObject();
    SpraysString = JsonObject2String(SpraysData);
    ViewSprayHeads();

    Plans = CurrentDevice["Plans"].toArray();
    if(Plans.empty())
    {
        Plans.insert(0, QJsonObject());
        Plans.insert(0, QJsonObject());
        Plans.insert(0, QJsonObject());
    }

    CachePlans = Plans;
    QList<int> sprayIds;
    for (int i = 0; i < SprayHeads.size(); ++i)
    {
        sprayIds.push_back(SprayHeads[i].toObject()["SprayID"].toString().toInt());
    }

    emit deviceInfoArrive(
                CurrentDevice["HYID"].toString(),
            CurrentDevice["WIFI"].toString(),
            CurrentDevice["PWD"].toString(),
            CurrentDevice["MapCoordinates"].toString(),
            CurrentDevice["Address"].toString(),
            CurrentDevice["WorkWifiSsid"].toString(),
            CurrentDevice["WorkWifiPassword"].toString(),
            QString::number(SprayHeads.size()),
            sprayIds
            );

    CurrentPlanIndex = 0;
    getPlan(CurrentPlanIndex);
}

QJsonObject mainwork::getSprayMode(int ID, int Duration, int Delay, int Mode, int Length,
                                   int Speed1, int OnTime1, int OffTime1,
                                   int Speed2, int OnTime2, int OffTime2,
                                   int Speed3, int OnTime3, int OffTime3,
                                   int Speed4, int OnTime4, int OffTime4,
                                   int Speed5, int OnTime5, int OffTime5)
{
    QJsonObject SprayMode;
    SprayMode.insert("ID"      , ID);
    SprayMode.insert("Duration", Duration);
    SprayMode.insert("Delay"   , Delay);
    SprayMode.insert("Mode"    , Mode);
    SprayMode.insert("Length"  , Length);
    QJsonArray Speeds;
    QJsonObject Speed;

    Speed.insert("Speed"  , Speed1);
    Speed.insert("OnTime" , OnTime1);
    Speed.insert("OffTime", OffTime1);
    if(Speed1 != -1)
        Speeds.push_back(Speed);

    Speed.insert("Speed"  , Speed2);
    Speed.insert("OnTime" , OnTime2);
    Speed.insert("OffTime", OffTime2);
    if(Speed2 != -1)
        Speeds.push_back(Speed);

    Speed.insert("Speed"  ,Speed3);
    Speed.insert("OnTime" ,OnTime3);
    Speed.insert("OffTime",OffTime3);
    if(Speed3 != -1)
        Speeds.push_back(Speed);

    Speed.insert("Speed"  , Speed4);
    Speed.insert("OnTime" , OnTime4);
    Speed.insert("OffTime", OffTime4);
    if(Speed4 != -1)
        Speeds.push_back(Speed);

    Speed.insert("Speed"  , Speed5);
    Speed.insert("OnTime" , OnTime5);
    Speed.insert("OffTime", OffTime5);
    if(Speed5 != -1)
        Speeds.push_back(Speed);

    SprayMode.insert("Speeds", Speeds);
    return SprayMode;
}

QJsonObject mainwork::getSprayModeDefault(int ID, int mode)
{
    return getSprayMode(ID, DurationMin, 1, mode, 1,    //Duration最小能填5 这里填DurationMin 用于清除过期数据
                        10, 5, 1);
}

QJsonObject mainwork::getDailyTime(int ID,int Mode,int Length,
                                   int Hour1,int Min1,
                                   int Hour2,int Min2,
                                   int Hour3,int Min3,
                                   int Hour4,int Min4,
                                   int Hour5,int Min5,
                                   int Hour6,int Min6,
                                   int Hour7,int Min7,
                                   int Hour8,int Min8,
                                   int Hour9,int Min9)
{

    QJsonObject DailyTime;
    DailyTime.insert("ID"  , ID);
    DailyTime.insert("Mode", Mode);

    if(Mode == 0)   //不循环
    {
        QJsonArray Times;
        QJsonObject Time;

        Time.insert("Hour", Hour1);
        Time.insert("Min" , Min1);
        if(Hour1 != -1)
            Times.push_back(Time);

        Time.insert("Hour", Hour2);
        Time.insert("Min" , Min2);
        if(Hour2 != -1)
            Times.push_back(Time);

        Time.insert("Hour", Hour3);
        Time.insert("Min" , Min3);
        if(Hour3 != -1)
            Times.push_back(Time);

        Time.insert("Hour", Hour4);
        Time.insert("Min" , Min4);
        if(Hour4 != -1)
            Times.push_back(Time);

        Time.insert("Hour", Hour5);
        Time.insert("Min" , Min5);
        if(Hour5 != -1)
            Times.push_back(Time);

        Time.insert("Hour", Hour6);
        Time.insert("Min" , Min6);
        if(Hour6 != -1)
            Times.push_back(Time);

        Time.insert("Hour", Hour7);
        Time.insert("Min" , Min7);
        if(Hour7 != -1)
            Times.push_back(Time);

        Time.insert("Hour", Hour8);
        Time.insert("Min" , Min8);
        if(Hour8 != -1)
            Times.push_back(Time);

        Time.insert("Hour", Hour9);
        Time.insert("Min" , Min9);
        if(Hour9 != -1)
            Times.push_back(Time);

        DailyTime.insert("Times", Times);
        DailyTime.insert("Length", Times.size());
    }
    else if(Mode == 1)  // 循环
    {
        DailyTime.insert("Hour"  , Hour1);
        DailyTime.insert("Min"   , Min1);
        DailyTime.insert("IntMin", Hour2);
        DailyTime.insert("Length", Length);                                                         //这里是总次数
    }

    return DailyTime;
}

QJsonObject mainwork::getGroup(bool ID1,  bool ID2 ,  bool ID3 ,  bool ID4 ,  bool ID5 ,  bool ID6 ,  bool ID7 ,  bool ID8 ,
                               bool ID9,  bool ID10,  bool ID11,  bool ID12,  bool ID13,  bool ID14,  bool ID15,  bool ID16)
{
    QJsonObject Group;
    QJsonArray Sprays;
    QJsonObject Spray;

    Spray.insert("ID", 1);
    if(ID1)
        Sprays.push_back(Spray);

    Spray.insert("ID", 2);
    if(ID2)
        Sprays.push_back(Spray);

    Spray.insert("ID", 3);
    if(ID3)
        Sprays.push_back(Spray);

    Spray.insert("ID", 4);
    if(ID4)
        Sprays.push_back(Spray);

    Spray.insert("ID", 5);
    if(ID5)
        Sprays.push_back(Spray);

    Spray.insert("ID", 6);
    if(ID6)
        Sprays.push_back(Spray);

    Spray.insert("ID", 7);
    if(ID7)
        Sprays.push_back(Spray);

    Spray.insert("ID", 8);
    if(ID8)
        Sprays.push_back(Spray);

    Spray.insert("ID", 9);
    if(ID9)
        Sprays.push_back(Spray);

    Spray.insert("ID", 10);
    if(ID10)
        Sprays.push_back(Spray);

    Spray.insert("ID", 11);
    if(ID11)
        Sprays.push_back(Spray);

    Spray.insert("ID", 12);
    if(ID12)
        Sprays.push_back(Spray);

    Spray.insert("ID", 13);
    if(ID13)
        Sprays.push_back(Spray);

    Spray.insert("ID", 14);
    if(ID14)
        Sprays.push_back(Spray);

    Spray.insert("ID", 15);
    if(ID15)
        Sprays.push_back(Spray);

    Spray.insert("ID", 16);
    if(ID16)
        Sprays.push_back(Spray);

    Group.insert("SprayNum", Sprays.size());
    Group.insert("Sprays", Sprays);

    return Group;
}

QJsonObject mainwork::getSingleDayTask(int ID, int Num,
                                       int TimeID1, int GroupID1, int ModeID1,
                                       int TimeID2, int GroupID2, int ModeID2,
                                       int TimeID3, int GroupID3, int ModeID3,
                                       int TimeID4, int GroupID4, int ModeID4,
                                       int TimeID5, int GroupID5, int ModeID5,
                                       int TimeID6, int GroupID6, int ModeID6,
                                       int TimeID7, int GroupID7, int ModeID7,
                                       int TimeID8, int GroupID8, int ModeID8,
                                       int TimeID9, int GroupID9, int ModeID9,
                                       int TimeID10, int GroupID10, int ModeID10
                                       )
{
    QJsonObject SingleDayTask;
    SingleDayTask.insert("ID" , ID);
    SingleDayTask.insert("Num", Num);
    QJsonArray Groups;
    QJsonObject Group ;

    Group.insert("TimeID" , TimeID1);
    Group.insert("GroupID", GroupID1);
    Group.insert("ModeID" , ModeID1);
    if(TimeID1 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID2);
    Group.insert("GroupID", GroupID2);
    Group.insert("ModeID" , ModeID2);
    if(TimeID2 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID3);
    Group.insert("GroupID", GroupID3);
    Group.insert("ModeID" , ModeID3);
    if(TimeID3 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID4);
    Group.insert("GroupID", GroupID4);
    Group.insert("ModeID" , ModeID4);
    if(TimeID4 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID5);
    Group.insert("GroupID", GroupID5);
    Group.insert("ModeID" , ModeID5);
    if(TimeID5 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID6);
    Group.insert("GroupID", GroupID6);
    Group.insert("ModeID" , ModeID6);
    if(TimeID6 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID7);
    Group.insert("GroupID", GroupID7);
    Group.insert("ModeID" , ModeID7);
    if(TimeID7 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID8);
    Group.insert("GroupID", GroupID8);
    Group.insert("ModeID" , ModeID8);
    if(TimeID8 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID9);
    Group.insert("GroupID", GroupID9);
    Group.insert("ModeID" , ModeID9);
    if(TimeID9 != -1)
        Groups.push_back(Group);

    Group.insert("TimeID" , TimeID10);
    Group.insert("GroupID", GroupID10);
    Group.insert("ModeID" , ModeID10);
    if(TimeID10 != -1)
        Groups.push_back(Group);

    if(Groups.size())
        SingleDayTask.insert("Groups",Groups);
    return SingleDayTask;
}

void mainwork::setPlan(int numplan,
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
                       bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12, bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16)
{
    QJsonObject plan;

    genPlanInfo(plan, numplan,
                isVarSpeed, isConstSpead,
                isWorkday, isEveryday,
                isTiming, isLoop,
                Starttime1, Endtime1,
                Spraytime1, Intervaltime1,
                Starttime2, Endtime2,
                Spraytime2, Intervaltime2,
                Starttime3, Endtime3,
                Spraytime3, Intervaltime3,
                isSparyer01, isSparyer02, isSparyer03, isSparyer04, isSparyer05, isSparyer06, isSparyer07, isSparyer08,
                isSparyer09, isSparyer10, isSparyer11, isSparyer12, isSparyer13, isSparyer14, isSparyer15, isSparyer16);

    //    Plans.removeAt(numplan);
    //    Plans.insert(numplan, plan);
    CachePlans.removeAt(numplan);
    CachePlans.insert(numplan, plan);
}

void mainwork::sumitPlan(int numplan, bool isVarSpeed, bool isConstSpead, bool isWorkday, bool isEveryday, bool isTiming, bool isLoop,
                         QString Starttime1, QString Endtime1, QString Spraytime1, QString Intervaltime1,
                         QString Starttime2, QString Endtime2, QString Spraytime2, QString Intervaltime2,
                         QString Starttime3, QString Endtime3, QString Spraytime3, QString Intervaltime3,
                         bool isSparyer01, bool isSparyer02, bool isSparyer03, bool isSparyer04, bool isSparyer05, bool isSparyer06, bool isSparyer07, bool isSparyer08,
                         bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12, bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16)
{
    setPlan(numplan,
            isVarSpeed,  isConstSpead,
            isWorkday,   isEveryday,
            isTiming,    isLoop,
            Starttime1,  Endtime1,
            Spraytime1,  Intervaltime1,
            Starttime2,  Endtime2,
            Spraytime2,  Intervaltime2,
            Starttime3,  Endtime3,
            Spraytime3,  Intervaltime3,
            isSparyer01, isSparyer02, isSparyer03, isSparyer04, isSparyer05, isSparyer06, isSparyer07, isSparyer08,
            isSparyer09, isSparyer10, isSparyer11, isSparyer12, isSparyer13, isSparyer14, isSparyer15, isSparyer16);

    // to check 方案冲突:
    if (checkTimeConflict(numplan, CachePlans))
    {
        qDebug() << "方案有冲突！" << endl;
        emitShowMessage(conflictMsg);
        return;
    }

    Plans = CachePlans;

    QJsonObject Plan1 = Plans[0].toObject();                                                       //开始Plans转五个表
    QJsonObject Plan2 = Plans[1].toObject();
    QJsonObject Plan3 = Plans[2].toObject();

    QJsonArray PlanSpray1 = Plan1["PlanSpray"].toArray();
    QJsonArray PlanSpray2 = Plan2["PlanSpray"].toArray();
    QJsonArray PlanSpray3 = Plan3["PlanSpray"].toArray();

    bool isTimingAvalid[PLAN_COUNT][TIMING_COUNT] = {{false}};

    //喷雾模式
    QJsonArray SprayModeTable;

    if (true)
    {
        QDateTime sprayTime1;
        QDateTime sprayTime2;
        QDateTime sprayTime3;

        int Duration1 = 0;
        int Duration2 = 0;
        int Duration3 = 0;

        for (int i = 0; i < PLAN_COUNT; i++)
        {
            QJsonObject aPlan = Plans[i].toObject();

            isVarSpeed = aPlan["isVarSpeed"].toBool();

            sprayTime1 = QDateTime::fromString("1970-01-01 " + aPlan["Spraytime1"].toString(), "yyyy-MM-dd hh:mm");
            sprayTime2 = QDateTime::fromString("1970-01-01 " + aPlan["Spraytime2"].toString(), "yyyy-MM-dd hh:mm");
            sprayTime3 = QDateTime::fromString("1970-01-01 " + aPlan["Spraytime3"].toString(), "yyyy-MM-dd hh:mm");

            sprayTime1.setTimeSpec(Qt::UTC);
            sprayTime2.setTimeSpec(Qt::UTC);
            sprayTime3.setTimeSpec(Qt::UTC);

            if(!sprayTime1.isValid())   sprayTime1 = QDateTime::fromMSecsSinceEpoch(0).toUTC();
            if(!sprayTime2.isValid())   sprayTime2 = QDateTime::fromMSecsSinceEpoch(0).toUTC();
            if(!sprayTime3.isValid())   sprayTime3 = QDateTime::fromMSecsSinceEpoch(0).toUTC();

            Duration1 = sprayTime1.toString("mm").toInt() * 60;
            Duration2 = sprayTime2.toString("mm").toInt() * 60;
            Duration3 = sprayTime3.toString("mm").toInt() * 60;


            if(Duration1 < 0)   Duration1 = 0;
            if(Duration1 == 0)
            {
                SprayModeTable.push_back(getSprayModeDefault(i*3+1, isVarSpeed? 2:1));   //getSprayMode(1, DurationMin, 1, !aPlan["isVarSpeed"].toBool() ? 1 : 2, 1,//Duration最小能填5 这里填DurationMin 用于清除过期数据
                //10, 5, 1));/**/
            }
            else
            {
                isTimingAvalid[i][0] = true;
                SprayModeTable.push_back(getSprayMode(i*3+1, Duration1, 2, isVarSpeed ? 2 : 1, 3,  //Duration最小能填5 这里填10
                                                      30, 16, 2,
                                                      60, 18, 2,
                                                      100, 18, 2));
            }

            if(Duration2 < 0)   Duration2 = 0;
            if(Duration2 == 0)
            {
                SprayModeTable.push_back(getSprayModeDefault(i * 3 + 2, isVarSpeed ? 2 : 1));
            }
            else
            {
                isTimingAvalid[i][1] = true;
                SprayModeTable.push_back(getSprayMode(i * 3 + 2, Duration2, 2, isVarSpeed ? 2 : 1, 3,
                                                      30, 16, 2,
                                                      60, 18, 2,
                                                      100, 18, 2));
            }

            if(Duration3 < 0)   Duration3 = 0;
            if(Duration3 == 0)
            {
                SprayModeTable.push_back(getSprayModeDefault(i * 3 + 3, isVarSpeed ? 2 : 1));
            }
            else
            {
                isTimingAvalid[i][2] = true;
                SprayModeTable.push_back(getSprayMode(i * 3 + 3, Duration3, 2, isVarSpeed ? 2 : 1, 3,
                                                      30, 16, 2,
                                                      60, 18, 2,
                                                      100, 18, 2));
            }
        }

        int DurationTest = 7;
        SprayModeTable.push_back(getSprayMode(10, DurationTest, 1, 1, 1,                                 //清除喷雾
                                              100, 5, 1));                                             //1.SprayModeTable
    }

    //日定时表
    QJsonArray DailyTimeTable;

    //int Length  = -1;
    int IntMin1 = -1;
    int Hour1   = -1;
    int Min1    = -1;

    for (int i = 0; i < PLAN_COUNT; i++)
    {
        QJsonObject aPlan = Plans[i].toObject();

        QDateTime startTime    = QDateTime::fromMSecsSinceEpoch(0).toUTC();
        QDateTime endTime      = QDateTime::fromMSecsSinceEpoch(0).toUTC();
        QDateTime intervalTime = QDateTime::fromMSecsSinceEpoch(0).toUTC();

        startTime   .setTimeSpec(Qt::UTC);
        endTime     .setTimeSpec(Qt::UTC);
        intervalTime.setTimeSpec(Qt::UTC);

        bool TimingMode ;

        TimingMode = aPlan["isTiming"].toBool();
        if(TimingMode)  //定时
        {
            for (int timingNum = 0; timingNum < TIMING_COUNT; timingNum++)
            {
                if (!isTimingAvalid[i][timingNum])
                {
                    //未配置或无效的时间段，忽略
                    continue;
                }

                QString itemName = "Starttime" + QString::number(timingNum + 1);
                startTime = QDateTime::fromString("1970-01-01 " + aPlan[itemName].toString(), "yyyy-MM-dd hh:mm");
                startTime.setTimeSpec(Qt::UTC);

                if(!startTime.isValid())
                {
                    //启动时间无效，该时间段也视作无效,忽略
                    isTimingAvalid[i][timingNum] = false;
                    continue;
                }

                //startTime = QDateTime::fromMSecsSinceEpoch(0).toUTC();

                Hour1 = startTime.toString("hh").toInt();
                Min1  = startTime.toString("mm").toInt();

                DailyTimeTable.push_back(getDailyTime(i * 3 + timingNum + 1,
                                                      TIMING_MODE_TIMER, 1, Hour1, Min1));
            }
        }
        else    // 循环
        {
            for (int timingNum = 0; timingNum < TIMING_COUNT; timingNum++)
            {
                if (!isTimingAvalid[i][timingNum])
                {
                    //未配置或无效的时间段，忽略
                    continue;
                }

                QString itemNameStart = "Starttime" + QString::number(timingNum+1);
                QString itemNameEnd = "Endtime" + QString::number(timingNum+1);
                QString itemNameInterval = "Intervaltime" + QString::number(timingNum+1);

                startTime    = QDateTime::fromString("1970-01-01 " + aPlan[itemNameStart].toString(), "yyyy-MM-dd hh:mm");
                endTime      = QDateTime::fromString("1970-01-01 " + aPlan[itemNameEnd].toString(), "yyyy-MM-dd hh:mm");
                intervalTime = QDateTime::fromString("1970-01-01 " + aPlan[itemNameInterval].toString(), "yyyy-MM-dd hh:mm");

                startTime   .setTimeSpec(Qt::UTC);
                endTime     .setTimeSpec(Qt::UTC);
                intervalTime.setTimeSpec(Qt::UTC);

                if(!startTime.isValid() || !endTime.isValid()  || !intervalTime.isValid())
                {
                    //有时间无效，该时间段也视作无效,忽略
                    isTimingAvalid[i][timingNum] = false;
                    continue;
                }

                Hour1    = startTime.toString("hh").toInt();        // 时
                Min1     = startTime.toString("mm").toInt();        // 分
                IntMin1  = intervalTime.toSecsSinceEpoch() / 60;    // 间隔时间(分)

                //20210621: 方案里选择循环喷洒时，app计算的次数与实际不符。次数现为（结束时间-开始时间）除以（喷洒时间+间隔时间）。应当改为结束时间-开始时间）除以（间隔时间）。
                //int times1 = fyDiv((endTime.toSecsSinceEpoch() - startTime.toSecsSinceEpoch()), (Duration1 + IntMin1 * 60));       //计算次数

                int waitTime = 1000;
                if (startTime.toSecsSinceEpoch() > endTime.toSecsSinceEpoch())
                {
                    emitHostResponse("结束时间不能小于开始时间！", true);
                    Sleep(waitTime);
                    emitHostResponse("取消发送！", true);
                    // 退出sumitPlan函数
                    return;
                }
                if(intervalTime.toSecsSinceEpoch() > (endTime.toSecsSinceEpoch() - startTime.toSecsSinceEpoch()))
                {
                    if (startTime.toSecsSinceEpoch() == endTime.toSecsSinceEpoch())
                    {
                        emitHostResponse("结束时间不能与开始时间相同！", true);
                        Sleep(waitTime);
                        emitHostResponse("取消发送！", true);
                        return;
                    }
                    emitHostResponse("间隔时间大于循环时间段！", true);
                    Sleep(waitTime);
                    emitHostResponse("取消发送！", true);
                    return;
                }
                if(intervalTime.toSecsSinceEpoch() > 195*60)
                {
                    emitHostResponse("间隔时间不能大于195分钟！", true);
                    Sleep(waitTime);
                    emitHostResponse("取消发送！", true);
                    return;
                }

                int times = fyDiv((endTime.toSecsSinceEpoch() - startTime.toSecsSinceEpoch()), IntMin1 * 60);       //计算喷洒次数
                if (times == INT_MAX || times < 0) times = 0;        // 喷洒次数不能为负数
                if (times == 0)
                {
                    if (IntMin1 == 0)        // 间隔时间不能为 0
                    {
                        emitHostResponse("间隔时间不能小于1分钟！", true);
                        Sleep(waitTime);
                        emitHostResponse("取消发送！", true);
                        return;
                    }
                    if (startTime.toSecsSinceEpoch() == endTime.toSecsSinceEpoch())
                    {
                        emitHostResponse("结束时间不能与开始时间相同！", true);
                        Sleep(waitTime);
                        emitHostResponse("取消发送！", true);
                        return;
                    }

                    //未配置或无效的时间段，忽略
                    isTimingAvalid[i][timingNum] = false;
                    continue;
                }

                DailyTimeTable.push_back(getDailyTime(i*3+timingNum+1, TIMING_MODE_LOOP,
                                                      times, Hour1, Min1, IntMin1));
            }
        }
    }
    //2.DailyTimeTable

    //喷头分组表
    QJsonArray SprayHeadGroupingTable;
    QJsonObject SprayHeadGrouping;

    QJsonArray Groups;

    for (int i = 0; i < PLAN_COUNT; i++)
    {
        QJsonObject aPlan = Plans[i].toObject();
        QJsonArray PlanSpray = aPlan["PlanSpray"].toArray();

        Groups.push_back(
                    getGroup(PlanSpray[ 0].toBool(),PlanSpray[ 1].toBool(), PlanSpray[ 2].toBool(), PlanSpray[ 3].toBool(),
                PlanSpray[ 4].toBool(), PlanSpray[ 5].toBool(), PlanSpray[ 6].toBool(), PlanSpray[ 7].toBool(),
                PlanSpray[ 8].toBool(), PlanSpray[ 9].toBool(), PlanSpray[10].toBool(), PlanSpray[11].toBool(),
                PlanSpray[12].toBool(), PlanSpray[13].toBool(), PlanSpray[14].toBool(), PlanSpray[15].toBool())
                );
    }

    Groups.push_back(
                getGroup( SprayIdExits( 1), SprayIdExits( 2), SprayIdExits( 3), SprayIdExits( 4),
                          SprayIdExits( 5), SprayIdExits( 6), SprayIdExits( 7), SprayIdExits( 8),
                          SprayIdExits( 9), SprayIdExits(10), SprayIdExits(11), SprayIdExits(12),
                          SprayIdExits(13), SprayIdExits(14), SprayIdExits(15), SprayIdExits(16))
                );  //用于全部喷雾

    SprayHeadGrouping.insert("Groups", Groups);
    SprayHeadGrouping.insert("GroupNum", Groups.size());
    SprayHeadGroupingTable.push_back(SprayHeadGrouping);                                           //3.SprayHeadGroupingTable    √

    //单日任务表
    QJsonArray SingleDayTaskTable;

    //每日的 喷洒任务放在 序号1； 工作日的 喷洒任务放在 序号2；
    //序号2中，要包含工作日的任务 & 每日的任务
    QJsonObject DayTask1;
    QJsonObject DayTask2;

    QJsonArray Groups1;
    QJsonArray Groups2;
    QJsonObject Group ;

    for (int i = 0; i < PLAN_COUNT; i++)
    {
        QJsonObject aPlan = Plans[i].toObject();
        bool isEveryday = aPlan["isEveryday"].toBool();

        if (isEveryday)
        {
            //每天的
            for (int timingNum = 0; timingNum < TIMING_COUNT; timingNum++)
            {
                if (isTimingAvalid[i][timingNum])
                {
                    Group.insert("TimeID" , i*3+timingNum+1);
                    Group.insert("GroupID", i+1);
                    Group.insert("ModeID" , i*3+timingNum+1);

                    Groups1.push_back(Group);
                    Groups2.push_back(Group);
                }
            }
        }
        else
        {
            //工作日的
            for (int timingNum = 0; timingNum < TIMING_COUNT; timingNum++)
            {
                if (isTimingAvalid[i][timingNum])
                {
                    Group.insert("TimeID" , i * 3 + timingNum + 1);
                    Group.insert("GroupID", i + 1);
                    Group.insert("ModeID" , i * 3 + timingNum + 1);

                    Groups2.push_back(Group);
                }
            }
        }
    }

    DayTask1.insert("ID" , 1);
    DayTask1.insert("Num", Groups1.size());
    DayTask1.insert("Groups", Groups1);

    DayTask2.insert("ID" , 2);
    DayTask2.insert("Num", Groups2.size());
    DayTask2.insert("Groups", Groups2);

    //if(Groups1.size() > 0)
    {
        SingleDayTaskTable.insert(0, DayTask1);
    }

    //if(Groups2.size() > 0)
    {
        SingleDayTaskTable.insert(1, DayTask2);
    }

    //周任务表
    QJsonArray WeekTaskTable;
    QJsonObject WeekTask;

    bool haveEveryday = true;
    bool haveWorkday = true;

    if (Groups1.size() == 0)
    {
        haveEveryday = false;
    }
    if (Groups2.size() == 0)
    {
        haveWorkday = false;
    }

    WeekTask.insert("Mon",  haveWorkday ? 2 : 0);
    WeekTask.insert("Tue",  haveWorkday ? 2 : 0);
    WeekTask.insert("Wed",  haveWorkday ? 2 : 0);
    WeekTask.insert("Thu",  haveWorkday ? 2 : 0);
    WeekTask.insert("Fri",  haveWorkday ? 2 : 0);
    WeekTask.insert("Sat", haveEveryday ? 1 : 0);
    WeekTask.insert("Sun", haveEveryday ? 1 : 0);

    WeekTask.insert("Today", 0);
    WeekTaskTable.push_back(WeekTask);                                                             //5.WeekTaskTable    √

    QJsonObject Table;
    Table.insert("SprayModeTable", SprayModeTable);
    Table.insert("DailyTimeTable", DailyTimeTable);
    Table.insert("SprayHeadGroupingTable", SprayHeadGroupingTable);
    Table.insert("SingleDayTaskTable", SingleDayTaskTable);
    Table.insert("WeekTaskTable", WeekTaskTable);

    bool ret;
    int progress  = 0;
    int stageLimit = 50;
    QString errorMsg;
    QString sn = CurrentDevice["HYID"].toString();
    MainCtrlInfo mcInfo;
    int waitTime = 500;

    emitHostResponse("正在获取设备状态...", true);
    ret = requestMainCtrlInfo(sn, mcInfo, errorMsg);

    if (mcInfo.onlineStatus == HOST_ONLINE_STATUS_OFFLINE)
    {
        emitHostResponse("设备不在线！", true);
        return;
    }

    if (mcInfo.connectionMode == HOST_CONNECTION_MODE_NB)
    {
        waitTime = 4000;
    }

    NetDebug::post("gap time:" + QString::number(waitTime));

    emitHostResponse("正在准备写入消毒计划...", true);
    enterWriteIotPlan(sn, mcInfo, errorMsg);

    Sleep(waitTime);
    progress = 5;
    emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
    NetDebug::post("write plan... " + QString::number(progress) + "%");

    for(int i = 0; i < SprayModeTable.size(); i++)
    {
        // size will be  9
        NetDebug::post("SprayModeTable: " + QString::number(i));
#if SUBMIT_WITH_RESPONSE
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        NetDebug::post("write plan... " + QString::number(progress) + "%");

        ret = SendPlan(WriteSprayMode, SprayModeTable[i].toObject());                                    //20
#else
        ret = SendOder(WriteSprayMode, SprayModeTable[i].toObject());                                    //20
#endif
        if (!ret)
        {
            leaveWriteIotPlan(sn, mcInfo);

            //emitHostResponse("写入消毒计划异常 !", true);

            return;
        }

        progress += 4;
        if (progress > stageLimit)
        {
            progress = stageLimit;
        }
        NetDebug::post("write plan... " + QString::number(progress) + "%");

#if SUBMIT_WITH_RESPONSE
        //
#else
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
#endif
    }

    progress = stageLimit;
    emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);

    stageLimit = 65;
    for(int i = 0; i < DailyTimeTable.size(); i++)
    {
        // size will be  5
        NetDebug::post("DailyTimeTable: " + QString::number(i));

#if SUBMIT_WITH_RESPONSE
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        NetDebug::post("write plan... " + QString::number(progress) + "%");

        SendPlan(WriteDailyTime, DailyTimeTable[i].toObject());                                    //22
#else
        SendOder(WriteDailyTime, DailyTimeTable[i].toObject());                                    //22
#endif
        progress += 3;
        if (progress > stageLimit)
        {
            progress = stageLimit;
        }
        NetDebug::post("write plan... " + QString::number(progress) + "%");

#if SUBMIT_WITH_RESPONSE
        //
#else
        NetDebug::post("write plan... " + QString::number(progress) + "%");

        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
#endif
    }
    progress = stageLimit;
    emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);

    stageLimit = 78;
    for(int i = 0; i < SprayHeadGroupingTable.size(); i++)
    {
        NetDebug::post("SprayHeadGroupingTable: " + QString::number(i));

#if SUBMIT_WITH_RESPONSE
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        NetDebug::post("write plan... " + QString::number(progress) + "%");

        SendPlan(WriteSprayHeadGrouping, SprayHeadGroupingTable[i].toObject());                    //24
#else
        SendOder(WriteSprayHeadGrouping, SprayHeadGroupingTable[i].toObject());                    //24
#endif
        progress += 3;
        if (progress > stageLimit)
        {
            progress = stageLimit;
        }
        NetDebug::post("write plan... " + QString::number(progress) + "%");

#if SUBMIT_WITH_RESPONSE
        //
#else
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
#endif
    }
    progress = stageLimit;
    emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);

    stageLimit = 89;
    for(int i = 0; i < SingleDayTaskTable.size(); i++)
    {
        NetDebug::post("SingleDayTaskTable: " + QString::number(i));

#if SUBMIT_WITH_RESPONSE
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        NetDebug::post("write plan... " + QString::number(progress) + "%");

        SendPlan(WriteSingleDayTask, SingleDayTaskTable[i].toObject());                            //26
#else
        SendOder(WriteSingleDayTask, SingleDayTaskTable[i].toObject());                            //26
#endif
        progress += 3;
        if (progress > stageLimit)
        {
            progress = stageLimit;
        }
        NetDebug::post("write plan... " + QString::number(progress) + "%");

#if SUBMIT_WITH_RESPONSE
        //
#else
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
#endif
    }
    progress = stageLimit;
    emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);

    stageLimit = 95;
    for(int i = 0; i < WeekTaskTable.size(); i++)
    {
        NetDebug::post("WeekTaskTable: " + QString::number(i));

#if SUBMIT_WITH_RESPONSE
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        NetDebug::post("write plan... " + QString::number(progress) + "%");

        SendPlan(WriteWeekTask, WeekTaskTable[i].toObject());                                      //28
#else
        SendOder(WriteWeekTask, WeekTaskTable[i].toObject());                                      //28
#endif
        progress += 3;
        if (progress > stageLimit)
        {
            progress = stageLimit;
        }
        NetDebug::post("write plan... " + QString::number(progress) + "%");

#if SUBMIT_WITH_RESPONSE
        //
#else
        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);

        emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
        Sleep(waitTime);
#endif
    }
    progress = stageLimit;
    emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);

    stageLimit = 98;

    leaveWriteIotPlan(sn, mcInfo);
    NetDebug::post("write plan... " + QString::number(progress) + "%");

    emitHostResponse("正在写入消毒计划 " + QString::number(progress) + "%", true);
    Sleep(waitTime);

    progress = 100;
    emitHostResponse("写入消毒计划完成！", true);
    NetDebug::post("write plan... " + QString::number(progress) + "%");

    // to do : make clear ??
    QJsonArray PlanSpray;

    PlanSpray = Plans[numplan].toObject()["PlanSpray"].toArray();

    CurrentDevice.insert("Table",Table);
    CurrentDevice.insert("Plans",Plans);

    QJsonObject SpraysData = CurrentDevice["allSpray"].toObject();

    QJsonArray sprays = SpraysData["data"].toArray();

    if(CurrentPlanIndex == 0)   CurrentPlanName = "方案一";
    if(CurrentPlanIndex == 1)   CurrentPlanName = "方案二";
    if(CurrentPlanIndex == 2)   CurrentPlanName = "方案三";

    for(int i = 0; i < sprays.size(); i++)
    {
        QJsonObject spray = sprays[i].toObject();
        QString Plan = spray["Plan"].toString();
        QStringList PlanList = Plan.split(",");

        if (PlanSpray[ spray["SprayID"].toString().toInt() -1 ].toBool())                          //SprayID从1开始
        {
            bool exist = false;
            for(int j = 0; j < PlanList.size(); j++)
            {
                if(PlanList[j] == CurrentPlanName)  exist = true;
            }
            if(!exist)
            {
                if(Plan == "")
                {
                    Plan = CurrentPlanName;
                }
                else
                {
                    Plan += "," + CurrentPlanName;
                }
            }
        }
        else
        {
            for(int j = 0; j < PlanList.size(); j++)
            {
                if(PlanList[j] == CurrentPlanName)
                {
                    PlanList.removeAt(j);
                };
            }
            Plan = "";
            if(PlanList.size())
            {
                Plan = PlanList[0];
                PlanList.removeFirst();
            }
            for(int j = 0; j < PlanList.size(); j++)
            {
                Plan += "," + PlanList[j];
            }
        }

        spray.insert("Plan", Plan);
        sprays[i] = spray;
    };

    SpraysData["data"] = sprays;
    CurrentDevice["allSpray"] = SpraysData;

    CurrentDevice.insert("TaskPlanLastUpdateTime", GetCurrentTime("yyyy.MM.dd hh:mm"));

    //SetUIoTCoreDeviceProperty("last_update_plan_time", qint32(QDateTime::currentSecsSinceEpoch()));
    saveHost();

    progress = 100;
    emitHostResponse("写入消毒计划完成！", true);
    NetDebug::post("write plan... " + QString::number(progress) + "%");
}

void mainwork::updateDevices(QJsonObject downloadJson)
{
    QString TableName;
    ControlOders Cmd = ControlOders(downloadJson["Cmd"].toInt());

    bool subJson = false;
    switch(Cmd)
    {
    case GetSprayRecord:
        qDebug().noquote() << QString("主机回复的JSON信息:\r") << downloadJson;
        processingSprinklersInformation(downloadJson);
        return;
        break;

    case GetSerialNumberSN:
        if (downloadJson.find("Data") != downloadJson.end())
        {
            QJsonObject data = downloadJson["Data"].toObject();

            if (parseSpraySNFromResp(data))
            {
                return;
            }
        }
        return;

    case ReadSprayMode:
        TableName = "SprayModeTable";
        subJson = true;
        break;

    case ReadDailySchedule:
        TableName = "DailyTimeTable";
        subJson = true;
        break;

    case ReadThePrintHeadGroupingTable:
        TableName = "SprayHeadGroupingTable";
        break;

    case ReadSingle_dayTaskTable:
        TableName = "SingleDayTaskTable";
        subJson = true;
        break;

    case ReadTheWeeklyTaskList:
        if (downloadJson.find("Data") != downloadJson.end())
        {
            QJsonObject data = downloadJson["Data"].toObject();

            if (parseSpraySNFromResp(data))
            {
                return;
            }
        }
        TableName = "WeekTaskTable";
        break;

    default:
        return;
    }

    qDebug()<<__FUNCTION__<<"更新当前设备"<<TableName;
    //qDebug()<<__FUNCTION__<<"更新前"<<CurrentDevice["Table"].toObject()[TableName];
    QJsonObject Data = downloadJson["Data"].toObject();
    QJsonObject Table = CurrentDevice["Table"].toObject();
    /*
    if(TableName == "SprayModeTable")
    {
        if(Data["Duration"].toInt() == DurationMin)
        {
            Data["Duration"] = 2;
        }
    }
*/
    if(subJson)
    {
        QJsonArray table = Table[TableName].toArray();
        int ID = Data["ID"].toInt();
        for (int i = 0; i < table.size(); ++i)
        {
            if(table[i].toObject()["ID"].toInt() == ID)
            {
                table.removeAt(i);
                i--;
            }
        }
        table.push_back(Data);
        Table[TableName] = table;
    }
    else
    {
        QJsonArray table;
        table.push_back(Data);
        Table[TableName] = table;
    }
    CurrentDevice["Table"] = Table;
    //qDebug()<<__FUNCTION__<<"更新后"<<CurrentDevice["Table"].toObject()[TableName];
    if(workState == "队列完成")
    {
        updatePlan();
        saveHost();
        workState = "处理完成";
    }
}

void mainwork::updatePlan()
{
    if(Plans.size() != PLAN_COUNT)return;
    for (int planNumber = 0; planNumber < 3; ++planNumber)
    {
        QJsonObject Plan = Plans[planNumber].toObject();

        QJsonArray PlanSpray;
        QJsonArray Sprays = CurrentDevice["Table"].toObject()
                ["SprayHeadGroupingTable"].toArray()[0].toObject()
                ["Groups"].toArray()[planNumber].toObject()
                ["Sprays"].toArray();
        for (int i = 0; i < 16; ++i) {
            PlanSpray.push_back(false);
        }
        for (int i = 0; i < Sprays.size(); ++i)
        {
            PlanSpray[Sprays[i].toObject()["ID"].toInt() - 1] = true;
        }
        Plan["PlanSpray"] = PlanSpray;                                                             //SprayHeadGroupingTable  >> 喷头

        QJsonArray SprayModeTable = CurrentDevice["Table"].toObject()
                ["SprayModeTable"].toArray();

        for (int SprayModeRowNumber = 0; SprayModeRowNumber < SprayModeTable.size(); SprayModeRowNumber++)
        {
            for (int PlanRow = 0; PlanRow < 3; ++PlanRow)
            {
                if(SprayModeTable[SprayModeRowNumber].toObject()["ID"].toInt() == planNumber * 3 + PlanRow + 1)
                {
                    QJsonObject SprayMode = SprayModeTable[planNumber * 3 + PlanRow].toObject();
                    int Duration = SprayMode["Duration"].toInt();
                    Plan["pt" + QString::number(PlanRow) + "5"] = QDateTime::fromSecsSinceEpoch(Duration).toUTC().toString("hh:mm");
                    Plan["isVarSpeed"] = SprayMode["Mode"].toInt() == 2;
                    Plan["isConstSpead"] = SprayMode["Mode"].toInt() == 1;
                }
            }
        }   //SprayModeTable

        QJsonArray DailyTimeTable = CurrentDevice["Table"].toObject()
                ["DailyTimeTable"].toArray();

        for (int SprayModeRowNumber = 0; SprayModeRowNumber < DailyTimeTable.size(); SprayModeRowNumber++)
        {
            for (int PlanRow = 0; PlanRow < 3; ++PlanRow)
            {
                if(DailyTimeTable[SprayModeRowNumber].toObject()["ID"].toInt() == planNumber * 3 + PlanRow + 1)
                {
                    QJsonObject DailyTime = DailyTimeTable[planNumber * 3 + PlanRow].toObject();
                    Plan["isLoop"] = DailyTime["Mode"] == 1;
                    Plan["isTiming"] = !Plan["isLoop"].toBool();
                    if(Plan["isTiming"].toBool())
                    {
                        Plan["pt" + QString::number(PlanRow) + "3"] = QString("%1:%2")
                                .arg(DailyTime["Times"].toArray()[0].toObject()["Hour"].toInt(), 2, 10, QLatin1Char('0'))
                                .arg(DailyTime["Times"].toArray()[0].toObject()["Min"].toInt(), 2, 10, QLatin1Char('0'));
                    }
                    else
                    {
                        Plan["pt" + QString::number(PlanRow) + "3"] = QString("%1:%2")
                                .arg(DailyTime["Hour"].toInt(), 2, 10, QLatin1Char('0'))
                                .arg(DailyTime["Min"].toInt(), 2, 10, QLatin1Char('0'));

                        Plan["pt" + QString::number(PlanRow) + "6"] = QString("%1:%2")
                                .arg(DailyTime["IntMin"].toInt() / 60, 2, 10, QLatin1Char('0'))
                                .arg(DailyTime["IntMin"].toInt() % 60, 2, 10, QLatin1Char('0'));

                        QDateTime startTime    = QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "3"].toString(),"yyyy-MM-dd hh:mm");
                        QDateTime sprayTime    = QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "5"].toString(),"yyyy-MM-dd hh:mm");
                        QDateTime intervalTime = QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "6"].toString(),"yyyy-MM-dd hh:mm");

                        startTime   .setTimeSpec(Qt::UTC);
                        sprayTime   .setTimeSpec(Qt::UTC);
                        intervalTime.setTimeSpec(Qt::UTC);

                        Plan["pt" + QString::number(PlanRow) + "4"] =
                                QDateTime::fromSecsSinceEpoch(startTime.toSecsSinceEpoch() + (sprayTime.toSecsSinceEpoch() + 2 + intervalTime.toSecsSinceEpoch()) *
                                                              DailyTime["Length"].toInt()).toUTC().toString("hh:mm");                  //结束=开始3+(喷洒5+间隔6)*次数
                        /*
                        qDebug().noquote()<<"\r\npt" + QString::number(PlanRow) + "3"
                                <<"1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "3"].toString()
                                <<QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "3"].toString(),"yyyy-MM-dd hh:mm")
                                <<QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "3"].toString(),"yyyy-MM-dd hh:mm").toSecsSinceEpoch()
                                <<"\r\npt"+ QString::number(PlanRow) + "5"
                                <<"1970-01-01 " + Plan["pt"+ QString::number(PlanRow) + "5"].toString()
                                <<QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "5"].toString(),"yyyy-MM-dd hh:mm")
                                <<QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "5"].toString(),"yyyy-MM-dd hh:mm").toSecsSinceEpoch()
                                <<"\r\npt"+ QString::number(PlanRow) + "6"
                                <<"1970-01-01 " + Plan["pt"+ QString::number(PlanRow) + "6"].toString()
                                <<QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "6"].toString(),"yyyy-MM-dd hh:mm")
                                <<QDateTime::fromString("1970-01-01 " + Plan["pt" + QString::number(PlanRow) + "6"].toString(),"yyyy-MM-dd hh:mm").toSecsSinceEpoch()  ;
                        */    //调试pt*4
                    }
                }
            }
        }   //DailyTimeTable

        QJsonArray Groups;
        if(CurrentDevice["Table"].toObject()["SingleDayTaskTable"].toArray().size())
        {
            Groups = CurrentDevice["Table"].toObject()
                    ["SingleDayTaskTable"].toArray()[0].toObject()["Groups"].toArray();
        }

        Plan["isEveryday"] = false;
        Plan["isWorkday"] = true;

        for (int GroupNumber = 0; GroupNumber < Groups.size(); ++GroupNumber)
        {
            if( in(Groups[GroupNumber].toObject()["TimeID"].toInt(), planNumber * 3 + 1, (planNumber + 1) * 3))
            {
                Plan["isEveryday"] = true;
                Plan["isWorkday"] = false;
                /*
                qDebug()<<"Plan"<<planNumber<<"Row"<<GroupNumber
                    <<Groups[GroupNumber].toObject()["TimeID"].toInt()<<"in"
                   <<planNumber * 3 + 1<<(planNumber + 1) * 3<<"pb5"<<Plan["pb5"]<<"pb4"<<Plan["pb4"];
                */
            };
        }

        Plans[planNumber] = Plan;
        /*qDebug().noquote()<<__FUNCTION__<< Plan;*/
    }
    CurrentDevice["Plans"] = Plans;
    CachePlans = Plans;
}

void mainwork::getPlan(int index)
{
    NetDebug::post("getPlan(): num:" + QString::number(index));

    CurrentPlanIndex = index;
    //CurrentPlan = Plans[index].toObject();
    CurrentPlan = CachePlans[index].toObject();

    QJsonArray PlanSpray = CurrentPlan["PlanSpray"].toArray();
    emit showPlan(index,
                  CurrentPlan["isVarSpeed"].toBool(),
            CurrentPlan["isConstSpead"].toBool(),
            CurrentPlan["isWorkday"].toBool(),
            CurrentPlan["isEveryday"].toBool(),
            CurrentPlan["isTiming"].toBool(),
            CurrentPlan["isLoop"].toBool(),

            CurrentPlan["Starttime1"].toString(),
            CurrentPlan["Endtime1"].toString(),
            CurrentPlan["Spraytime1"].toString(),
            CurrentPlan["Intervaltime1"].toString(),

            CurrentPlan["Starttime2"].toString(),
            CurrentPlan["Endtime2"].toString(),
            CurrentPlan["Spraytime2"].toString(),
            CurrentPlan["Intervaltime2"].toString(),

            CurrentPlan["Starttime3"].toString(),
            CurrentPlan["Endtime3"].toString(),
            CurrentPlan["Spraytime3"].toString(),
            CurrentPlan["Intervaltime3"].toString(),

            PlanSpray[0].toBool(),
            PlanSpray[1].toBool(),
            PlanSpray[2].toBool(),
            PlanSpray[3].toBool(),
            PlanSpray[4].toBool(),
            PlanSpray[5].toBool(),
            PlanSpray[6].toBool(),
            PlanSpray[7].toBool(),

            PlanSpray[8].toBool(),
            PlanSpray[9].toBool(),
            PlanSpray[10].toBool(),
            PlanSpray[11].toBool(),
            PlanSpray[12].toBool(),
            PlanSpray[13].toBool(),
            PlanSpray[14].toBool(),
            PlanSpray[15].toBool()
            );
}

bool mainwork::cachePlan(int numplan,
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
                         bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12, bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16)
{
    QJsonObject plan;

    NetDebug::post("cachePlan(): num:" + QString::number(numplan) );

    genPlanInfo(plan, numplan,
                isVarSpeed, isConstSpead,
                isWorkday, isEveryday,
                isTiming, isLoop,
                Starttime1, Endtime1,
                Spraytime1, Intervaltime1,
                Starttime2, Endtime2,
                Spraytime2, Intervaltime2,
                Starttime3, Endtime3,
                Spraytime3, Intervaltime3,
                isSparyer01, isSparyer02, isSparyer03, isSparyer04, isSparyer05, isSparyer06, isSparyer07, isSparyer08,
                isSparyer09, isSparyer10, isSparyer11, isSparyer12, isSparyer13, isSparyer14, isSparyer15, isSparyer16);

    if (CachePlans.size() < PLAN_COUNT)
    {
        for (int i = CachePlans.size(); i < PLAN_COUNT; ++i)
        {
            CachePlans.insert(i, QJsonObject());
        }
        //NetDebug::post("cachePlan(): adap, CachePlans:" + QString::number(CachePlans.size()));
    }

    CachePlans.removeAt(numplan);
    CachePlans.insert(numplan, plan);

    // to check 方案冲突:
    if (checkTimeConflict(numplan, CachePlans))
    {
        NetDebug::post("cachePlan() checkTimeConflict failed");
        emitShowMessage(conflictMsg);
        return false;
    }

    return true;
}

bool mainwork::getPlan(int index, PlanInfo &aPlan)
{
    //QJsonObject plan = Plans[index].toObject();
    QJsonObject plan = CachePlans[index].toObject();
    QJsonArray PlanSpray = plan["PlanSpray"].toArray();

    aPlan.isValidPlan = true;

    if (plan["isVarSpeed"].toBool())
    {
        aPlan.sprayMod = 0;
    }
    else {
        aPlan.sprayMod = 1;
    }

    if (plan["isWorkday"].toBool())
    {
        aPlan.workDay = 0;
    }
    else {
        aPlan.workDay = 1;
    }

    if (plan["isTiming"].toBool())
    {
        aPlan.workMode = 0;
    }
    else {
        aPlan.workMode = 1;
    }

    aPlan.periods[0].startTime = plan["Starttime1"].toString();
    aPlan.periods[0].endTime = plan["Endtime1"].toString();
    aPlan.periods[0].sprayTime = plan["Spraytime1"].toString();
    aPlan.periods[0].intervalTime = plan["Intervaltime1"].toString();

    aPlan.periods[1].startTime = plan["Starttime2"].toString();
    aPlan.periods[1].endTime = plan["Endtime2"].toString();
    aPlan.periods[1].sprayTime = plan["Spraytime2"].toString();
    aPlan.periods[1].intervalTime = plan["Intervaltime2"].toString();

    aPlan.periods[2].startTime = plan["Starttime3"].toString();
    aPlan.periods[2].endTime = plan["Endtime3"].toString();
    aPlan.periods[2].sprayTime = plan["Spraytime3"].toString();
    aPlan.periods[2].intervalTime = plan["Intervaltime3"].toString();

    for (int i = 0; i < MAX_SPRAYHEAD_COUNT; i++)
    {
        if (PlanSpray[i].toBool())
        {
            aPlan.sprays[i] = i + 1;
        }
        else {
            aPlan.sprays[i] = 0;
        }
    }

    return true;
}

bool isSprayerGroupIntersected(int planA, int planB, std::set<int> groups[PLAN_COUNT])
{
    bool ret = false;

    if ((planA >= PLAN_COUNT) || (planB >= PLAN_COUNT))
    {
        return ret;
    }

    if (planA == planB)
    {
        if (groups[planA].empty())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else {
        if ((groups[planA].empty()) || groups[planB].empty())
        {
            return false;
        }

        for (auto a = groups[planA].begin(); a != groups[planA].end(); a++)
        {
            if (groups[planB].find(*a) !=  groups[planB].end())
            {
                return true;
            }
        }
    }

    return ret;
}

bool isTimeConflict( QDateTime sTime1, QDateTime eTime1,  QDateTime sTime2, QDateTime eTime2)
{
    bool ret = true;

    if ((sTime2.toTime_t() > eTime1.toTime_t()) ||
            (eTime2.toTime_t() < sTime1.toTime_t()))
    {
        ret = false;
    }

    return ret;
}

bool mainwork::checkTimeConflict(int numplan, QJsonArray& allPlans)
{
    bool ret = false;

    checkTimingAvalid(allPlans);

    //    QString logMsg;
    //    for (int i = 0; i < PLAN_COUNT; i++)
    //    {
    //        logMsg += QString("plan%1: %2-%3-%4;").arg(i+1).arg(isTimingAvalid[i][0]).arg(isTimingAvalid[i][1]).arg(isTimingAvalid[i][2]);
    //        logMsg +=  "\r\n";
    //    }

    std::set<int> plansGroups[PLAN_COUNT];

    for (int i = 0; i < PLAN_COUNT; i++)
    {
        QJsonObject aPlan = allPlans[i].toObject();
        QJsonArray PlanSpray = aPlan["PlanSpray"].toArray();

        for (int j = 0; j < MAX_SPRAYHEAD_COUNT; j++)
        {
            if (PlanSpray[j].toBool())
            {
                plansGroups[i].insert(j+1);
            }
        }
    }

    bool aTimingMode, bTimingMode;
    QString aItemName, bItemName;
    QJsonObject aPlan, bPlan;
    QDateTime sTime1, eTime1, sTime2, eTime2;

    for (int i = 0; i < PLAN_COUNT; i++)
    {
        aPlan = allPlans[i].toObject();
        aTimingMode = aPlan["isTiming"].toBool();

        for (int aTimingNum = 0; aTimingNum < TIMING_COUNT; aTimingNum++)
        {
            if (isTimingAvalid[i][aTimingNum] == false)
            {
                continue;
            }

            //获得开始时间、结束时间
            aItemName = "Starttime" + QString::number(aTimingNum+1);
            sTime1 = QDateTime::fromString("1970-01-01 " + aPlan[aItemName].toString(), "yyyy-MM-dd hh:mm");
            sTime1.setTimeSpec(Qt::UTC);

            if (aTimingMode)    //定时
            {
                aItemName = "Spraytime" + QString::number(aTimingNum+1);

                QDateTime durantion = QDateTime::fromString("1970-01-01 " + aPlan[aItemName].toString(), "yyyy-MM-dd hh:mm");
                durantion.setTimeSpec(Qt::UTC);
                int min = durantion.toString("mm").toInt();
                eTime1 = sTime1;
                eTime1 = eTime1.addSecs(min*60);
            }
            else    //循环
            {
                aItemName = "Endtime" + QString::number(aTimingNum+1);
                eTime1 = QDateTime::fromString("1970-01-01 " + aPlan[aItemName].toString(), "yyyy-MM-dd hh:mm");
                eTime1.setTimeSpec(Qt::UTC);
            }

            for (int j = 0; j < PLAN_COUNT; j++)
            {
                if (i < j)
                {
                    continue;
                }

                bPlan = allPlans[j].toObject();
                bTimingMode = bPlan["isTiming"].toBool();

                bool isGroupIntersected = isSprayerGroupIntersected(i, j, plansGroups);

                for (int bTimingNum = 0; bTimingNum < TIMING_COUNT; bTimingNum++)
                {
                    if ((i == j) && (bTimingNum <= aTimingNum))
                    {
                        continue;
                    }

                    if (isTimingAvalid[j][bTimingNum] == false)
                    {
                        continue;
                    }

                    //获得开始时间、结束时间
                    bItemName = "Starttime" + QString::number(bTimingNum+1);
                    sTime2 = QDateTime::fromString("1970-01-01 " + bPlan[bItemName].toString(), "yyyy-MM-dd hh:mm");
                    sTime2.setTimeSpec(Qt::UTC);

                    if (bTimingMode)    //定时
                    {
                        bItemName = "Spraytime" + QString::number(bTimingNum+1);

                        QDateTime durantion = QDateTime::fromString("1970-01-01 " + bPlan[bItemName].toString(), "yyyy-MM-dd hh:mm");
                        durantion.setTimeSpec(Qt::UTC);
                        int min = durantion.toString("mm").toInt();
                        eTime2 = sTime2;
                        eTime2 = eTime2.addSecs(min * 60);
                    }
                    else    //循环
                    {
                        bItemName = "Endtime" + QString::number(bTimingNum+1);
                        eTime2 = QDateTime::fromString("1970-01-01 " + bPlan[bItemName].toString(), "yyyy-MM-dd hh:mm");
                        eTime2.setTimeSpec(Qt::UTC);
                    }

                    if (isTimeConflict(sTime1, eTime1,  sTime2, eTime2))
                    {
                        if (!isGroupIntersected)
                        {
                            //喷头不在一个组的，时间上允许冲突
                            continue;
                        }

                        if (!ret)
                        {
                            ret = true;
                            conflictMsg = "方案时间设置有冲突：";

                        }

                        //                        logMsg += QString("%1.%2 vs %3.%4: ").arg(i+1).arg(aTimingNum+1).arg(j+1).arg(bTimingNum+1);
                        //                        logMsg += sTime1.toString("hh:mm:ss") + "-" + eTime1.toString("hh:mm:ss") + ",\r\n";
                        //                        logMsg += sTime2.toString("hh:mm:ss") + "--" + eTime2.toString("hh:mm:ss") + ";\r\n";

                        conflictMsg += "\r\n";
                        QString conf = QString("方案%1的时间段%2--方案%3的时间段%4").arg(i+1).arg(aTimingNum+1).arg(j+1).arg(bTimingNum+1);
                        conflictMsg += conf;
                        //qDebug() << "方案时间设置有冲突：" << "方案" << i+1 << "的时间段" << aTimingNum+1 << " 和 方案" << j+1 << "的时间段" << bTimingNum+1 << endl;
                        //qDebug() << "sTime1: " << sTime1.toString("yyyy-MM-dd hh:mm")  << "eTime1: " << eTime1.toString("yyyy-MM-dd hh:mm")  << "sTime2: " << sTime2.toString("yyyy-MM-dd hh:mm")  << "eTime2: " << eTime2.toString("yyyy-MM-dd hh:mm") << endl;
                    }
                }
            }
        }
    }

    if (ret)
    {
        conflictMsg += "\r\n";
        conflictMsg += "请检查修改后重新提交方案";
    }

    //    emitShowMessage(logMsg);
    return ret;
}

bool mainwork::checkTimingAvalid(QJsonArray& allPlans)
{
    //isTimingAvalid[PLAN_COUNT][TIMING_COUNT] = {{true}};
    for (int i = 0; i < PLAN_COUNT; i++)
    {
        for (int j = 0; j < TIMING_COUNT; j++)
        {
            isTimingAvalid[i][j] = true;
        }
    }

    QJsonObject currentPlan;

    QDateTime sprayTime;
    int Duration = 0;

    for (int i = 0; i < PLAN_COUNT; i++)
    {
        currentPlan = allPlans[i].toObject();

        QDateTime startTime    = QDateTime::fromMSecsSinceEpoch(0).toUTC();
        QDateTime endTime      = QDateTime::fromMSecsSinceEpoch(0).toUTC();
        QDateTime intervalTime = QDateTime::fromMSecsSinceEpoch(0).toUTC();

        startTime   .setTimeSpec(Qt::UTC);
        endTime     .setTimeSpec(Qt::UTC);
        intervalTime.setTimeSpec(Qt::UTC);

        bool TimingMode ;

        TimingMode = currentPlan["isTiming"].toBool();

        for (int timingNum = 0; timingNum < TIMING_COUNT; timingNum++)
        {
            //先检查喷洒时间为0的，认为是无效的计划：
            QString itemName = "Spraytime" + QString::number(timingNum+1);
            sprayTime = QDateTime::fromString("1970-01-01 " + currentPlan[itemName].toString(), "yyyy-MM-dd hh:mm");

            sprayTime.setTimeSpec(Qt::UTC);

            if(!sprayTime.isValid())
                sprayTime = QDateTime::fromMSecsSinceEpoch(0).toUTC();

            Duration = sprayTime.toString("mm").toInt() * 60;

            if(Duration <= 0)
            {
                isTimingAvalid[i][timingNum] = false;
                continue;
            }

            //再检查开始时间、结束时间不合规的，认为是无效的计划：
            if(TimingMode)  //定时
            {
                if (!isTimingAvalid[i][timingNum])
                {
                    //未配置或无效的时间段，忽略
                    continue;
                }

                itemName = "Starttime" + QString::number(timingNum+1);
                startTime = QDateTime::fromString("1970-01-01 " + currentPlan[itemName].toString(), "yyyy-MM-dd hh:mm");
                startTime.setTimeSpec(Qt::UTC);

                if(!startTime.isValid())
                {
                    //启动时间无效，该时间段也视作无效,忽略
                    isTimingAvalid[i][timingNum] = false;
                    continue;
                }
            }
            else    // 循环
            {
                QString itemNameStart = "Starttime" + QString::number(timingNum+1);
                QString itemNameEnd = "Endtime" + QString::number(timingNum+1);
                QString itemNameInterval = "Intervaltime" + QString::number(timingNum+1);

                startTime    = QDateTime::fromString("1970-01-01 " + currentPlan[itemNameStart].toString(), "yyyy-MM-dd hh:mm");
                endTime      = QDateTime::fromString("1970-01-01 " + currentPlan[itemNameEnd].toString(), "yyyy-MM-dd hh:mm");
                intervalTime = QDateTime::fromString("1970-01-01 " + currentPlan[itemNameInterval].toString(), "yyyy-MM-dd hh:mm");

                startTime   .setTimeSpec(Qt::UTC);
                endTime     .setTimeSpec(Qt::UTC);
                intervalTime.setTimeSpec(Qt::UTC);

                if(!startTime.isValid() || !endTime.isValid() || !intervalTime.isValid())
                {
                    //有时间无效，该时间段也视作无效,忽略
                    isTimingAvalid[i][timingNum] = false;
                    continue;
                }

                int IntMin1 = intervalTime.toSecsSinceEpoch() / 60;
                int Length = fyDiv((endTime.toSecsSinceEpoch() - startTime.toSecsSinceEpoch()), IntMin1 * 60);       //计算次数

                if(Length == INT_MAX || Length < 0)Length = 0;

                if (Length == 0)
                {
                    //未配置或无效的时间段，忽略
                    isTimingAvalid[i][timingNum] = false;
                    continue;
                }
            }
        }
    }

    //    qDebug() << "TimingAvalid:" << "\r\n";
    //    for (int i = 0; i < PLAN_COUNT; i++)
    //    {
    //        qDebug() << "Plan " << i+1 << isTimingAvalid[i][0] << "\t" << isTimingAvalid[i][1] << "\t" << isTimingAvalid[i][2] << "\t" << "\r\n";
    //    }

    return true;
}

void mainwork::adddevices()
{
    WifiController.enableGps();
    WifiController.openWifi();
    CurrentDevice = QJsonObject();
    emit deviceInfoArrive(
                "请扫描主机二维码",
                "",
                "",
                "经0.000000,纬0.000000",
                "",
                "",
                "",
                "0",
                QList<int>());
}

void mainwork::openWifi()
{
    WifiController.openWifi();
}

void mainwork::disconnectHost()
{
    WifiController.closeWifi();
}

void mainwork::wifiRefresh()
{
    QString currentWifiAP = WifiController.getConntectedWifiSSID();                                //currentWifiAP.replace("\"","");

    LogFile("wifiRefresh()  currentWifi:" + currentWifiAP + ", Device:" + CurrentDevice["WIFI"].toString());

    if(currentWifiAP != CurrentDevice["WIFI"].toString()
            && currentWifiAP  != "\"" + CurrentDevice["WIFI"].toString()+ "\""
            && currentWifiAP.toLower() != "<unknown ssid>"
            && currentWifiAP.toLower() != "\"<unknown ssid>\""
            )                                                                                      //三星等手机返回("SSID")
    {
        connectHostWifiTimes++;
        if (connectHostWifiTimes > 3)
        {
            timerRefresh->stop();
            if (APState == "正在连接")
            {
                APState = "未连接";
            }
            emit hostScanStop();

            QString msg = "无法连接主机WIFI";
            if (currentWifiAP.isEmpty())
            {
                msg = "无法连接主机WIFI,主机开启否?";
            }
            emitHostResponse(msg , true);
            return;
        }

        netId = WifiController.getwifi(CurrentDevice["WIFI"].toString(),CurrentDevice["PWD"].toString());
        //qDebug()<<__FUNCTION__<<"正在连接主机WIFI netId:"<<netId<<" "<<getProcessTime()<<"ms";
        emitHostResponse("正在连接主机WIFI (" + QString::number(connectHostWifiTimes) + ")", true, 5);
        timerRefresh->start(10000);                                                                //VIVO切一次切不过来  有时候获取IP需要很长时间
        APState = "正在连接";
    }
    else
    {
        qDebug()<<__FUNCTION__<<"主机WIFI已连接"<<" "<<getProcessTime()<<"ms";
        timerRefresh->stop();
        emitHostResponse("主机WIFI已连接", true);
        APState = "热点已连接";
    }
}

void mainwork::connectHostWifi()
{
    connectHostWifiTimes = 0;
    wifiRefresh();
    debug(CurrentDevice["WIFI"].toString() + "," + CurrentDevice["PWD"].toString());
}

void mainwork::disconnectHostWifi()
{
    disconnectTcp();
    timerRefresh->stop();
    APState = "未连接";
    netId = WifiController.getwifi(CurrentDevice["WorkWifiSsid"].toString(), CurrentDevice["WorkWifiPassword"].toString());
}

bool mainwork::isConnect2Host()
{
    bool ret = false;
    QString hostWiFi;
    QString currentWifiAP = WifiController.getConntectedWifiSSID();                                //currentWifiAP.replace("\"","");

    for(int i = 0; i < Devices.size(); i++)
    {
        hostWiFi = Devices[i].toObject()["WIFI"].toString();
        if(currentWifiAP == hostWiFi ||
                currentWifiAP  == "\"" + hostWiFi + "\"")           //三星等手机返回("SSID")
        {
            ret = true;
            NetDebug::post("Connect to the Host: " + Devices[i].toObject()["HYID"].toString());
            return true;
        }
    }

    QString msg = "current WIFI:" + currentWifiAP;
    NetDebug::post(msg);

    return ret;
}

void mainwork::disconnectTcp()
{
    Connecter.tcpsender->abort();
    Connecter.tcpsender->disconnectFromHost();
    Connecter.tcpsender->close();
    netState = "没有连接";
    timerWritedTimeout->stop();
    timerConnectTimeout->stop();
    timerSendOneOder->stop();
    qDebug()<<__FUNCTION__<<"已手动从主机断开" <<" "<<getProcessTime() << "ms";
    emitHostResponse("已手动从主机断开", true);
}

QString mainwork::getHostWifiName(QString hostSN)
{
    QString wifi = "";
    if (hostSN.length() > 10)
    {
        wifi = hostSN.right(10);
    }
    return wifi;
    //CurrentDevice.insert("WIFI",right.right(10));
    //CurrentDevice.insert("PWD","fy12345678");
}

QString mainwork::getHostWifiPwd(QString hostSN)
{
    return QString("fy12345678");
}

void mainwork::onNewHost(QString sn)
{
    CurrentDevice = QJsonObject();
    CurrentDevice.insert("HYID", sn);
    CurrentDevice.insert("WIFI", getHostWifiName(sn));//right.right(10));
    CurrentDevice.insert("PWD", getHostWifiPwd());//"fy12345678");

    SprayHeads = QJsonArray();
    SpraysData.insert("data", SprayHeads);
}

int mainwork::getScreenWidth()
{
    return QGuiApplication::primaryScreen()->geometry().width() * QGuiApplication::primaryScreen()->devicePixelRatio();
}

int mainwork::getScreenHeight()
{
    return QGuiApplication::primaryScreen()->geometry().height() * QGuiApplication::primaryScreen()->devicePixelRatio();
}

void mainwork::debug(QString msg)
{
    udpw.dbg(msg);
}

//开始时间和结束时间的时间差
QDateTime sub(QDateTime a, QDateTime b)
{
    return QDateTime::fromSecsSinceEpoch(a.toSecsSinceEpoch() - b.toSecsSinceEpoch()).toUTC();
}

void mainwork::compareTime(QString element_msg, bool checked)
{
    if(checked) return;

    qDebug() << "已选择的element_msg为 " << element_msg;

    QStringList elements = element_msg.split(',');
    //获取当前时间
    QString currentElemet = elements.last();
    //删除时间
    elements.pop_back();

    QList<QDateTime> times;
    foreach(QString element, elements)
    {
        if(element == "")
        {
            times.push_back(QDateTime::fromSecsSinceEpoch(-1));
        }
        else {
            //格式化时间
            QDateTime tim = QDateTime::fromString("1970-01-01 " + element, "yyyy-MM-dd hh:mm");
            tim.setTimeSpec(Qt::UTC);
            times.push_back(tim);
        }
    }

    QList<QDateTime> times4;            //一维数组，存放时间
    QList<QList<QDateTime>> times44;    //二维数组，存放时间段

    //保存时间
    times4.push_back(times[0]);
    qDebug() << "times.size()的大小为 " << times.size() << ",elements.size()的大小为 " << elements.size();
    times4.push_back(times[1]); //启动时间
    times4.push_back(times[2]); //结束时间
    times4.push_back(times[3]); //间隔时间
    times44.push_back(times4);
    times4.clear();

    times4.push_back(times[4]);
    times4.push_back(times[5]);
    times4.push_back(times[6]);
    times4.push_back(times[7]);
    times44.push_back(times4);
    times4.clear();

    times4.push_back(times[8]);
    times4.push_back(times[9]);
    times4.push_back(times[10]);
    times4.push_back(times[11]);
    times44.push_back(times4);
    times4.clear();

    //当前选中的时间段
    int m = currentElemet.right(3).left(1).toInt();
    qDebug().noquote() << "当前选中的时间段为 " << QString("").right(3).left(1).toInt();

    if(times44[m][0] > times44[m][1])
    {
        emitHostResponse("结束时间不能小于开始时间！");
    }
    else if(times44[m][0] < times44[m][1])
    {
        if(times44[m][3] > sub(times44[m][1], times44[m][0]))
        {
            emitHostResponse("间隔时间不能大于循环时间段！");
        }
    }
    else {
        emitHostResponse("结束时间不能与开始时间相同！");
    }
    qDebug().noquote() << "间隔时间 " << times44[m][3] << ",时间差为 " << sub(times44[m][1], times44[m][0]);
    qDebug().noquote() << "开始时间 " << times44[m][0] << ",结束时间 " << times44[m][1];

    if(times44[m][3] == QDateTime::fromSecsSinceEpoch(0))
    {
        emitHostResponse("间隔时间不能小于1分钟！");
    }
    if(times44[m][3].toSecsSinceEpoch() > 0 && times44[m][3].toSecsSinceEpoch() > 195*60)
    {
        emitHostResponse("间隔时间不能大于195分钟！");
    }
    qDebug().noquote() << "间隔时间（秒）为 " << times44[m][3].toSecsSinceEpoch();
}

void mainwork::connectMqtt()
{
    deviceSN = CurrentDevice["HYID"].toString().toLatin1();
    QByteArray authmode = "1";
    QString mqttUsername = QLatin1String(productSN + "|" + deviceSN + "|" + authmode);
    QString mqttPassword = QLatin1String("45rrpjs5exawurog" /*"vdf04pkj13xq65pd"*/);
    QString mqttClientId = QLatin1String(productSN + "." + deviceSN);

    mqtt->setUsername(mqttUsername);
    mqtt->setPassword(mqttPassword);
    mqtt->setClientId(mqttClientId);

    Topic = QLatin1String(System + "/" +productSN + "/" + deviceSN + "/tmodel/property/post");
    mqtt->connectToHost();

    QEventLoop loop;
    connect(mqtt, &QMqttClient::stateChanged, &loop, &QEventLoop::quit);
    loop.exec();
}

void mainwork::initMqtt()
{
    mqtt = new QMqttClient(this);

    mqtt->setProtocolVersion(QMqttClient::MQTT_3_1);
    const QString mqttHostname = QLatin1String("iotstack.hyairo.tech"); // new IoT Server 20220305 // ("mqtt-cn-sh2.iot.ucloud.cn");
    quint16 mqttPort = 1883;
    quint16 mqttKeepAlive = 360;
    mqtt->setHostname(mqttHostname);
    mqtt->setPort(mqttPort);
    mqtt->setKeepAlive(mqttKeepAlive);

    connect(mqtt, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic)
    {
        const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" Received Topic: ")
                + topic.name()
                + QLatin1String(" Message: ")
                + message
                + QLatin1Char('\n');
        NetDebug::post(content);
        qDebug()<<__FUNCTION__<<"messageReceived"<<content<<mqtt->error();
    });

    connect(mqtt, &QMqttClient::stateChanged, this, [this]()
    {
        const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" stateChanged")
                + QLatin1Char('\n');
        qDebug()<<__FUNCTION__<<"stateChanged"<<content<<mqtt->error()<<mqtt->state();
    });

    connect(mqtt, &QMqttClient::disconnected, this, [this]()
    {
        const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" disconnected")
                + QLatin1Char('\n');
        qDebug()<<__FUNCTION__<<"disconnected"<<content<<mqtt->error();
    });

    connect(mqtt, &QMqttClient::pingResponseReceived, this, [this]()
    {
        const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" pingResponseReceived")
                + QLatin1Char('\n');
        qDebug()<<__FUNCTION__<<"pingResponseReceived"<<content<<mqtt->error();
    });

    connect(mqtt, &QMqttClient::connected, this, [this]()
    {
        const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" pingResponseReceived")
                + QLatin1Char('\n');
        qDebug()<<__FUNCTION__<<"connected"<<content<<mqtt->error();
    });
}

void mainwork::getHostPlan()
{
    debug("getHostPlan: \r\n" );
    QJsonObject SprayModeRow;
    for (int i = 1; i < 11; ++i)
    {
        SprayModeRow.insert("ID", i);
        SendOder(ReadSprayMode, SprayModeRow);
    }

    QJsonObject DailyScheduleRow;
    for (int i = 1; i < 11; ++i)
    {
        DailyScheduleRow.insert("ID", i);
        SendOder(ReadDailySchedule, DailyScheduleRow);
    }

    SendOder(ReadThePrintHeadGroupingTable, QJsonObject());

    QJsonObject Single_dayTaskRow;
    for (int i = 1; i < 8; ++i)
    {
        Single_dayTaskRow.insert("ID", i);
        SendOder(ReadSingle_dayTaskTable, Single_dayTaskRow);
    }

    SendOder(ReadTheWeeklyTaskList, QJsonObject());
}

#if 0
//不需要了，整合到postDeviceInfo()
void mainwork::postAddressInfo()
{
    QString sn = CurrentDevice["HYID"].toString().toLatin1();
    QJsonObject property;
    QString errorMsg;

    QJsonObject item;
    QString val;

    item.insert("Name", "Address");
    val = CurrentDevice["Address"].toString().toLatin1();
    item.insert("Value", val);

    val = CurrentDevice["UpdateTime"].toString().toLatin1();
    item.insert("LastUpdateTime", val);
    property.insert("Address", item);

    bool ret = iotMsg.deviceUpdate(sn, property, errorMsg);
    if (ret)
    {
        errorMsg = "提交安装位置成功";
    }
    else {
        errorMsg = "提交安装位置失败:\n" + errorMsg;
    }

    emitHostResponse(errorMsg,true);

#if 0
    QJsonObject address;
    address.insert("province", province);
    address.insert("city", city);
    address.insert("district", district);
    address.insert("detail_address", CurrentDevice["Address"].toString());
    SetUIoTCoreDeviceProperty("address", address);
#endif
}

QJsonObject mainwork::gpsDescription()
{
    QString sn = CurrentDevice["HYID"].toString().toLatin1();
    QJsonObject property;
    QString errorMsg;

    QJsonObject item;
    QJsonObject val;

    item.insert("Name", "Location");

    val.insert("Longitude", gpsd.nowLongitude);
    val.insert("Latitude", gpsd.nowLatitude);
    item.insert("Value", JsonObject2String(val));

    item.insert("LastUpdateTime", CurrentDevice["UpdateTime"].toString());

    QByteArray itemArray = JsonObject2ByteArray(item);
    shortJson(itemArray);
    item = ByteArray2JsonObject(itemArray);
    property.insert("Location", item);

    bool ret = iotMsg.deviceUpdate(sn, property, errorMsg);
    if (ret)
    {
        errorMsg = "提交GPS位置成功";
    }
    else {
        errorMsg = "提交GPS位置失败:\n" + errorMsg;
    }

    emitHostResponse(errorMsg,true);
    return QJsonObject();

#if 0
    //if(deviceSN != CurrentDevice["HYID"].toString().toLatin1())connectMqtt();
    QJsonArray location;
    location.push_back(gpsd.nowLongitude);
    location.push_back(gpsd.nowLatitude);
    location.push_back(1);

    SetUIoTCoreDeviceProperty("location", location);
    return QJsonObject();
#endif
}
#endif
#if 1
//切换到私服
//submit mainboard and sprinklers to server
void mainwork::sumitIotPlan()
{
    postDeviceInfo();
    postSprayPlan();
}

bool mainwork::postDeviceInfo()
{
    QString sn = CurrentDevice["HYID"].toString().toLatin1();
    QString errorMsg;
    bool ret;

    ret = postDeviceInfoToServer(sn, errorMsg);
    emitHostResponse(errorMsg, true);

    return ret;
}

bool mainwork::dlCurrentDevInfo()
{
    bool ret = false;
    QString errorMsg;

    QString sn = CurrentDevice["HYID"].toString().toLatin1();
    if (sn.isNull() || sn.isEmpty())
    {
        return ret;
    }

    emitHostResponse("下载 " +  sn , true);
    ret = updateMainCtrllerFromServer(sn, errorMsg,true);
    if (ret)
    {
        RefreshDevicesList();
        setCurrentDevice(sn);
    }
    else {
        emitHostResponse("未找到该主机信息!", true);
    }

    return ret;
}

void mainwork::readSpriklers()
{
    bool ret;
    bool firstCheck = true;

    int sprinklerID = 1;
    QString devSN = CurrentDevice["HYID"].toString();
    QString errorMsg;
    QString sprinklerSN;

    int waitSecs = 1;
    const int SEARCH_TIMES = 3;

    int count = 0;
    int maxID = -1;

    for (int times = 0; times < SEARCH_TIMES; ++times)
    {
        if ((times > 0) && (SprayHeads.size() == 0))
        {
            //找了一遍，一个都没有找到，就不找第二遍
            break;
        }

        if (times == 0)
        {
            //第一遍，全部ID扫一遍
            count = 0;
            maxID = MAX_SPRAYHEAD_COUNT;
        }
        else
        {
            count = getSprayCountAndMaxID(maxID);
            if (maxID < MAX_SPRAYHEAD_COUNT)
            {
                maxID++;
            }
            NetDebug::post("getSprayCountAndMaxID():" + QString::number(count));
        }

        for (int i = 0; i < maxID; i++)
        {
            if (SprayIdExits(i + 1))
            {
                continue;
            }
            sprinklerID = i + 1;

            if(APState == "已连接")
            {
                waitSecs = 2;
                emitHostResponse("获取喷头: " + QString::number(sprinklerID), true, waitSecs);

                QJsonObject ControlOderData;
                ControlOderData.insert("SprayID", sprinklerID);

                SendOder(GetSerialNumberSN, ControlOderData, waitSecs*10);
                Sleep(waitSecs*1000);
            }
            else {
                if (firstCheck)     // check if the Host is online ?
                {
                    firstCheck = false;
                    MainCtrlInfo mcInfo;

                    ret = requestMainCtrlInfo(devSN, mcInfo, errorMsg);
                    if (mcInfo.onlineStatus == HOST_ONLINE_STATUS_OFFLINE)
                    {
                        emitHostResponse("设备不在线！", true);
                        return;
                    }
                }

                waitSecs = 10;
                emitHostResponse("获取喷头: " + QString::number(sprinklerID), true, waitSecs+2);

                ret = iotMsg.getSprinklerSN(devSN, sprinklerID, sprinklerSN, errorMsg, waitSecs);
                if (ret)
                {
                    NetDebug::post("Get sprinkler " + QString::number(sprinklerID) + ", SN:" + sprinklerSN);
                    onGotSpray(sprinklerSN, sprinklerID);
                }
            }
        }
    }

    emitHostResponse("读取喷头完成!", true);
}

bool mainwork::postSprayPlan()
{
    QString sn = CurrentDevice["HYID"].toString().toLatin1();
    QString errorMsg;

    bool ret;
    ret = postSprayPlanToServer(sn, errorMsg);

    emitHostResponse(errorMsg,true);

    return ret;
}

bool mainwork::getSprayPlan()
{
    QString sn = CurrentDevice["HYID"].toString().toLatin1();
    QJsonArray plan;
    QString errorMsg;

    //Plans.removeAt(numplan);
    //Plans.insert(numplan, plan);

    bool ret = getSprayPlanFromServer(sn, plan, errorMsg);
    if (ret)
    {
        QJsonObject temp;
        temp.insert("Plans", plan);
        NetDebug::post(temp);
        CurrentDevice["Plans"] = plan;
    }

    return ret;
}

void mainwork::sumitToServer(int numplan,
                             bool isVarSpeed, bool isConstSpead,
                             bool isWorkday, bool isEveryday,
                             bool isTiming, bool isLoop,
                             QString Starttime1, QString Endtime1, QString Spraytime1, QString Intervaltime1,
                             QString Starttime2, QString Endtime2, QString Spraytime2, QString Intervaltime2,
                             QString Starttime3, QString Endtime3, QString Spraytime3, QString Intervaltime3,
                             bool isSparyer01, bool isSparyer02, bool isSparyer03, bool isSparyer04,
                             bool isSparyer05, bool isSparyer06, bool isSparyer07, bool isSparyer08,
                             bool isSparyer09, bool isSparyer10, bool isSparyer11, bool isSparyer12,
                             bool isSparyer13, bool isSparyer14, bool isSparyer15, bool isSparyer16
                             )
{
    setPlan(numplan,
            isVarSpeed, isConstSpead,
            isWorkday, isEveryday,
            isTiming, isLoop,
            Starttime1, Endtime1,
            Spraytime1, Intervaltime1,
            Starttime2, Endtime2,
            Spraytime2, Intervaltime2,
            Starttime3, Endtime3,
            Spraytime3, Intervaltime3,
            isSparyer01, isSparyer02, isSparyer03, isSparyer04, isSparyer05, isSparyer06, isSparyer07, isSparyer08,
            isSparyer09, isSparyer10, isSparyer11, isSparyer12, isSparyer13, isSparyer14, isSparyer15, isSparyer16);

    // to check 方案冲突:
    if (checkTimeConflict(numplan, CachePlans))
    {
        qDebug() << "方案有冲突！" << endl;
        emitShowMessage(conflictMsg);
        return;
    }

    Plans = CachePlans;
    sumitIotPlan();
}

bool mainwork::getDeviceInfoFromServer(QString sn, QJsonObject& devInfo, QString& errorMsg)
{
    QString address, updateTime, location, TaskPlanLastUpdateTime;
    QJsonObject property;
    int NumOfSprinklers = 0;

    devInfo = QJsonObject();
    QJsonObject dlDevInfo;

    //获取主机信息
    bool ret = iotMsg.deviceInfo(sn, property, errorMsg);
    if (ret)
    {
        NetDebug::post(property);

        QString val;
        dlDevInfo = property;

        if (property.find("Address") != property.end())    //Address
        {
            QJsonObject item = property["Address"].toObject();
            val = item["Value"].toString();
            address = val;
        }
        else {
            val = "";
        }
        dlDevInfo["Address"] = val;

        if (property.find("Location") != property.end())    //gps
        {
            QJsonObject item = property["Location"].toObject();

            val = item["Value"].toString();

            location = val;
        }
        else {
            val = "";
        }
        dlDevInfo["Location"] = val;

        if (property.find("NumOfSprinklers") != property.end())    //
        {
            QJsonObject item = property["NumOfSprinklers"].toObject();

            NumOfSprinklers = item["Value"].toString().toInt();
        }
        else {
            NumOfSprinklers = 0;
        }
        dlDevInfo["NumOfSprinklers"] = NumOfSprinklers;


        if (property.find("TaskPlanLastUpdateTime") != property.end())    //
        {
            QJsonObject item = property["TaskPlanLastUpdateTime"].toObject();

            val = item["Value"].toString();
            TaskPlanLastUpdateTime = val;
        }
        else {
            val = "";
        }
        dlDevInfo["TaskPlanLastUpdateTime"] = val;

        if (property.find("Organization") != property.end())    //
        {
            QJsonObject item = property["Organization"].toObject();

            val = item["Value"].toString();
        }
        else {
            val = "";
        }
        dlDevInfo["Organization"] = val;

        NetDebug::post(sn + ", NumOfSprinklers:" + QString::number(NumOfSprinklers) + ", Address:" + address
                       + ", Location:" + location + ", TaskPlanLastUpdateTime:" + TaskPlanLastUpdateTime);

        errorMsg = "获取信息成功";
        ret = true;
    }
    else {
        errorMsg = "获取信息失败:\n" + errorMsg;
        return ret;
    }

#ifdef MOVE_DATA_OPER
    NumOfSprinklers = getSprayNum(sn);

    dlDevInfo["NumOfSprinklers"] = NumOfSprinklers;
    NetDebug::post(sn + ", NumOfSprinklers:" + QString::number(NumOfSprinklers));

#endif

    if (NumOfSprinklers > 0)
    {
        //获取喷头信息
        QString sprinklerSN;

        QJsonArray sprinklers;
        //SprayHeads = QJsonArray();
        for (int i = 0; i < NumOfSprinklers; i++)
        {
            QJsonObject sprinklerProp;
            bool isOk = iotMsg.sprinklerInfo(sn, i + 1, sprinklerProp, errorMsg);
            if (isOk)
            {
                QJsonObject aSpray;
                QJsonObject item;

                sprinklerSN = sprinklerProp["SprinklerSN"].toString();
                item = sprinklerProp["Position"].toObject();

                aSpray.insert("HYID", sprinklerSN);
                aSpray.insert("SprayID", QString::number(i+1));
                aSpray.insert("Position", item["Value"].toString());
                aSpray.insert("UpdateTime", item["LastUpdateTime"].toString());

                NetDebug::post(aSpray);

                if (!sprinklerSN.isEmpty())
                {
                    //sumitSpray(address, QString::number(i));
                    //sprinklers.insert(i, aSpray);
                    sprinklers.append(aSpray);
                }
            }
        }

        QJsonObject allSprayData;
        allSprayData.insert("data", sprinklers);
        dlDevInfo["allSpray"] = allSprayData;
    }

    MainController mainCtrl(sn);
    devInfo = mainCtrl.dlJson2SaveJson(dlDevInfo);

    devInfo.insert("WIFI", getHostWifiName(sn));
    devInfo.insert("PWD", getHostWifiPwd());

    return ret;
}

bool mainwork::updateLocalDevice(QString sn, QJsonObject &devInfo, bool autoSave)
{
    bool ret = true;
    QJsonObject localDev;

    if (autoSave)
    {
        saveAllDevices();
    }

    return ret;
}

bool mainwork::postSprayPlanToServer(QString sn, QString &errorMsg)
{
    QJsonObject plan;
    QJsonObject item;
    QString val;

    PlanInfo planInfo;
    QDateTime startTime, endTime, durationTime, intervalTime;
    int Hour1, Min1, Hour2, Min2, duration, interval;

    for (int i = 0;i < PLAN_COUNT; i++)
    {
        if (getPlan(i, planInfo))
        {
            bool isValidPlan = true;

            plan.insert("ID", i + 1);
            plan.insert("Spraying", planInfo.sprayMod);
            plan.insert("Day", planInfo.workDay);
            plan.insert("Working", planInfo.workMode);

            // fill time periods:
            QJsonObject periods;
            QJsonArray periodInfo;
            int periodCount = 0;

            if (planInfo.workMode == 0) //Working mode, 0 for no cycling, 1 for cycling
            {
                for (int j = 0; j < TIMING_COUNT; j++) {

                    item = QJsonObject();

                    startTime = QDateTime::fromString("1970-01-01 " + planInfo.periods[j].startTime, "yyyy-MM-dd hh:mm");
                    durationTime = QDateTime::fromString("1970-01-01 " + planInfo.periods[j].sprayTime, "yyyy-MM-dd hh:mm");

                    if (startTime.isValid() && durationTime.isValid())
                    {
                        startTime.setTimeSpec(Qt::UTC);
                        Hour1 = startTime.toString("hh").toInt();
                        Min1  = startTime.toString("mm").toInt();

                        durationTime.setTimeSpec(Qt::UTC);
                        duration = durationTime.toSecsSinceEpoch() / 60;
                    }
                    else {
                        Hour1 = 0;
                        Min1 = 0;
                        duration = 0;
                    }

                    item.insert("ID", j+1);
                    item.insert("Hour", Hour1);
                    item.insert("Min", Min1);
                    item.insert("Duration", duration);

                    if (duration > 0)
                    {
                        periodInfo.push_back(item);
                        periodCount++;
                    }
                }
            }
            else {
                for (int j = 0; j < TIMING_COUNT; j++)
                {
                    item = QJsonObject();

                    startTime = QDateTime::fromString("1970-01-01 " + planInfo.periods[j].startTime, "yyyy-MM-dd hh:mm");
                    endTime = QDateTime::fromString("1970-01-01 " + planInfo.periods[j].endTime, "yyyy-MM-dd hh:mm");

                    intervalTime = QDateTime::fromString("1970-01-01 " + planInfo.periods[j].intervalTime, "yyyy-MM-dd hh:mm");
                    durationTime = QDateTime::fromString("1970-01-01 " + planInfo.periods[j].sprayTime, "yyyy-MM-dd hh:mm");

                    if (startTime.isValid() && durationTime.isValid() && endTime.isValid() && intervalTime.isValid())
                    {
                        startTime.setTimeSpec(Qt::UTC);
                        Hour1 = startTime.toString("hh").toInt();
                        Min1  = startTime.toString("mm").toInt();

                        durationTime.setTimeSpec(Qt::UTC);
                        duration = durationTime.toSecsSinceEpoch() / 60;

                        endTime.setTimeSpec(Qt::UTC);
                        Hour2 = endTime.toString("hh").toInt();
                        Min2  = endTime.toString("mm").toInt();

                        intervalTime.setTimeSpec(Qt::UTC);
                        interval = intervalTime.toSecsSinceEpoch() / 60;
                    }
                    else {
                        Hour1 = 0;
                        Min1 = 0;
                        duration = 0;
                    }

                    item.insert("ID", j+1);
                    item.insert("StartHour", Hour1);
                    item.insert("StartMin", Min1);
                    item.insert("EndHour", Hour2);
                    item.insert("EndMin", Min2);

                    item.insert("Duration", duration);
                    item.insert("Interval", interval);

                    if (duration > 0)
                    {
                        periodInfo.push_back(item);
                        periodCount++;
                    }
                }
            }
            periods.insert("NumOfPeriod", periodCount);
            periods.insert("Period", periodInfo);

            plan.insert("Periods", periods);

            // fill Group:
            QJsonObject group;
            QJsonArray sprays;
            int sprayCount = 0;

            for (int count = 0; count < MAX_SPRAYHEAD_COUNT; count++)
            {
                if ((planInfo.sprays[count] > 0) && (planInfo.sprays[count] < 255))
                {
                    QJsonObject sprinkler;
                    sprinkler.insert("ID", planInfo.sprays[count]);
                    sprays.push_back(sprinkler);
                    sprayCount++;
                }
            }

            group.insert("SprayNum", sprayCount);
            group.insert("Sprays", sprays);

            plan.insert("Group", group);

            bool ret = iotMsg.planUpdate(sn, plan, errorMsg);
            if (ret)
            {
                errorMsg = "提交喷雾方案成功";
            }
            else {
                errorMsg = "提交喷雾方案失败:\n" + errorMsg;
                return false;
            }
        }
    }

    return true;
}

bool mainwork::getSprayPlanFromServer(QString sn, QJsonArray &plans, QString &errorMsg)
{
    bool ret = false;
    errorMsg = "未获取喷雾方案失败";
    plans = QJsonArray();

    QJsonObject plan;
    QString msg;
    for (int planNum = 1; planNum < (PLAN_COUNT + 1); planNum++)
    {
        try {
            bool hasGot = iotMsg.planInfo(sn, planNum, plan, msg);
            if (hasGot)
            {
                PlanInfo planInfo;
                if (parseSprayPlan(plan, planInfo))
                {
                    if (planInfo.ID > 0 && planInfo.ID <= PLAN_COUNT)
                    {
                        plans.insert(plans.size(), planInfo.toJson());
                    }
                    errorMsg = "获取喷雾方案成功";
                    ret = true;
                }
                else {
                    NetDebug::post(QString::asprintf("parseSprayPlan() failed planNum:%d line:%d", planNum, __LINE__));
                }
            }
            else {
                //errorMsg = "获取喷雾方案失败:\n" + errorMsg;
            }
        } catch (...) {
            NetDebug::post("Exception!");
            break;
        }
    }

    if (!ret)
    {
        errorMsg = msg;
    }

    return ret;
}

bool mainwork::updateLocalSprayPlan(QString sn, QJsonArray &plans, bool autoSave)
{
    NetDebug::post(QString::asprintf("%s(): line:%d", __FUNCTION__, __LINE__));

    if (!isDeviceExist(sn))
    {
        NetDebug::post(QString::asprintf("%s(): line:%d, update paln, but can't find the device", __FUNCTION__, __LINE__));
        return false;
    }

    QJsonObject dev = getDevice(sn);
    if (dev.isEmpty())
        return false;

    dev.insert("Plans", plans);
    setDevice(dev);

    if (autoSave)
    {
        saveAllDevices();
    }

    return true;
}

bool mainwork::getMainCtrllerFromServer(QString sn, QJsonObject &mainCtrller, QString &errorMsg)
{
    bool ret;
    QJsonObject devInfo;
    QJsonArray plans;

    ret = getDeviceInfoFromServer(sn, devInfo, errorMsg);
    NetDebug::post(devInfo);
    if (ret)
    {
        mainCtrller = devInfo;
        QString planErrorMsg;
        if (getSprayPlanFromServer(sn, plans, planErrorMsg))
        {
            mainCtrller.insert("Plans", plans);
        }
        else {
            NetDebug::post("Get Plan: " + planErrorMsg);
        }
    }
    else {
        NetDebug::post(errorMsg);
    }

    return ret;
}

bool mainwork::updateLocalMainCtrller(QString sn, QJsonObject &mainCtrller, bool autoSave)
{
    setDevice(mainCtrller);

    if (autoSave)
    {
        saveAllDevices();
    }

    return true;
}

bool mainwork::updateMainCtrllerFromServer(QString sn, QString &errorMsg, bool autoSave)
{
    QJsonArray plans;
    QJsonObject mainCtrller;
    bool ret;

    //NetDebug::post(QString::asprintf("%s(): line:%d", __FUNCTION__, __LINE__));

    ret = getMainCtrllerFromServer(sn, mainCtrller, errorMsg);
    if (ret)
    {
        NetDebug::post(mainCtrller);
        updateLocalMainCtrller(sn, mainCtrller, autoSave);
    }

    return ret;
}

bool mainwork::updateSprayPlanFromServer(QString sn, QString &errorMsg, bool autoSave)
{
    bool ret;
    QJsonArray plans;

    ret = getSprayPlanFromServer(sn, plans, errorMsg);
    if (ret)
    {
        updateLocalSprayPlan(sn, plans, autoSave);
    }

    return ret;
}

void mainwork::moveData()
{
    QString sn ;
    QString errorMsg;
    bool ret;
    NetDebug::post(Devices);

    QJsonObject aDev;
    for (int index = 0; index < Devices.size(); index++)
    {
        aDev = Devices[index].toObject();
        sn = aDev["HYID"].toString();

        if (sn.isEmpty())
        {
            continue;
        }

        emitHostResponse("Move " +  sn , true);
        NetDebug::post("迁移 " + sn);
        ret = postDeviceInfoToServer(sn, errorMsg);
    }

    emitHostResponse(errorMsg,true);
}

int mainwork::getSprayNum(QString sn)
{
    int num = 0;
    QJsonObject property;
    QString errorMsg;
    bool ret;

    ret = iotMsg.cbInfo(sn, property, errorMsg);
    if (ret)
    {
        num = property["NumOfSprinklers"].toInt(0);
        NetDebug::post(property);
    }

    return num;
}

void mainwork::getIotPlan()
{
    QJsonArray plans;
    QJsonObject mainCtrller;
    QString sn = CurrentDevice["HYID"].toString().toLatin1();
    QString errorMsg;
    bool ret;

    ret = updateSprayPlanFromServer(sn, errorMsg, true);
    if (ret)
    {
        setCurrentDevice(sn);
    }

    emitHostResponse(errorMsg,true);

    return;
}
#else
void mainwork::sumitIotPlan()
{
    if(!SprayHeads.size())  return;
    SetUIoTCoreDeviceProperty("num_of_sprinklers", SprayHeads.size());
    int Heads[17];
    foreach (QJsonValue SprayHead, SprayHeads)
    {
        QJsonValue SprayID = SprayHead.toObject()["SprayID"];
        SetUIoTCoreDeviceProperty("sprinklers_sn_" + SprayID.toString(), SprayHead.toObject()["HYID"].toString());
        SetUIoTCoreDeviceProperty("sprinklers_position_" + SprayID.toString(), SprayHead.toObject()["Position"].toString());
        Heads[SprayID.toString().toInt()] = 1;
    }
    for (int i = 1; i < 17; i++)
    {
        if(Heads[i] != 1)
        {
            SetUIoTCoreDeviceProperty("sprinklers_sn_" + QString::number(i), "");
            SetUIoTCoreDeviceProperty("sprinklers_position_" + QString::number(i), "");
        }
    }
}

void mainwork::getIotPlan()
{
    iot.DeviceSN = CurrentDevice["HYID"].toString().toLatin1();
    QJsonObject reJson = ByteArray2JsonObject(iot.Action("GetUIoTCoreDeviceProperty"));
    if(reJson.empty())
    {
        emitHostResponse("云指令发送失败", true);
    }
    QByteArray re = JsonObject2ByteArray(reJson);
    qDebug().noquote()<<__FUNCTION__<<JsonObject2ByteArray(String2JsonObject(re)) ;
    int num_of_sprinklers = reJson["Property"].toObject()["num_of_sprinklers"].toObject()["Desired"].toInt();
    if(!(num_of_sprinklers > 0))    return;

    SprayHeads = QJsonArray();
    for (int i = 1; i < 16; i++)
    {
        QString HYID = reJson["Property"].toObject()["sprinklers_sn_" + QString::number(i)].toObject()["Desired"].toString();
        QString Position = reJson["Property"].toObject()["sprinklers_position_" + QString::number(i)].toObject()["Desired"].toString();
        if(HYID != "")
        {
            ScanSpray.insert("HYID", HYID);
            ScanSpray.insert("State", "下载成功");
            ScanSpray.insert("UpdateTime", GetCurrentTime("yyyy.MM.dd hh:mm"));
            sumitSpray(Position, QString::number(i));
        }
    }
}
#endif
void mainwork::SetUIoTCoreDeviceProperty(QString propertyName, QJsonValue propertyValue)
{
    iot.DeviceSN = CurrentDevice["HYID"].toString().toLatin1();
    QJsonObject Property;
    Property.insert(propertyName, propertyValue);

    QByteArray re = JsonObject2ByteArray(ByteArray2JsonObject(iot.Action("SetUIoTCoreDeviceProperty", JsonObject2ByteArray(Property).toBase64())));
    QJsonObject reJson = ByteArray2JsonObject(re);
    qDebug().noquote()<<__FUNCTION__<<Property<<"\r\n"<<JsonObject2ByteArray(reJson) ;
    if(reJson["RetCode"].toInt() > 0)
    {
        QString Message = reJson["Message"].toString();
        Message.replace("Device not exist", "主机没有注册");
        Message.replace("Device is not online", "主机不在线");

        emitHostResponse(QString::number(ResponseNum++) + ": " + Message, true);
        qDebug().noquote()<<__FUNCTION__<< Message;
    }
    else
    {
        propertyName.replace("num_of_sprinklers", "喷头总数");
        propertyName.replace("sprinklers_sn_", "喷头序列号");
        propertyName.replace("sprinklers_position_", "喷头安装位置");
        emitHostResponse(QString::number(ResponseNum++) + ": " + propertyName + "上传成功", true);
    }
}

void mainwork::trySpray(int SprayID, int ModeId)
{
    //试喷

    emit sprayWork(SprayID);
    //qDebug() << "试喷 ID=" + QString::number(SprayID) + ",模式=" + QString::number(ModeId);
    QJsonObject ControlOderData;
    ControlOderData.insert("ID", SprayID);
    ControlOderData.insert("ModeID", ModeId);
    SendOder(TrySingelSpray, ControlOderData);
}

void mainwork::deleteSpray(int index)
{
    if(CurrentDevice["HYID"].toString() == "") return;
    debug("删除" + QString::number(index));
    int sprayId = SprayHeads[index].toObject()["SprayID"].toString().toInt();
    SprayHeads.removeAt(index);
    SpraysData.insert("data", SprayHeads);
    SpraysString = JsonObject2String(SpraysData);

    for (int i = 0; i < Plans.size(); ++i)
    {
        QJsonObject Plan = Plans[i].toObject();
        QJsonArray PlanSpray = Plan["PlanSpray"].toArray();
        if(PlanSpray.size() == 16)
        {
            PlanSpray[sprayId - 1] = false;
        }
        Plan["PlanSpray"] = PlanSpray;
        Plans[i] = Plan;
    }

    CurrentDevice.insert("Plans", Plans);
    CurrentDevice.insert("allSpray", SpraysData);
    saveHost(); // CachePlans will be updated as saveHost()
}

void mainwork::addWater()
{
    //一键加水开
    QJsonObject ControlOderData;
    ControlOderData.insert("Mode", 1);
    SendOder(ControlWaterPump, ControlOderData);
    //timerClosePump->start(6000);
}

void mainwork::ClosePump()
{
    qDebug("一键加水关");
    QJsonObject ControlOderData;
    ControlOderData.insert("Mode", 0);
    SendOder(ControlWaterPump, ControlOderData);
    timerClosePump->stop();
}

void mainwork::uiDebug(QString msg)
{
    NetDebug::post(msg);
}

void mainwork::spray(int ModeID, int GroupID)
{
    QJsonObject oderSpray;
    oderSpray.insert("ModeID", ModeID);
    oderSpray.insert("GroupID", GroupID);

    SendOder(ExecuteSprayModeImmediately, oderSpray);
}

void mainwork::sprayAll(int ModeID)
{
    spray(ModeID,4);
    for (int i = 0; i < SprayHeads.size(); ++i)
    {
        emit sprayWork(i + 1);
    }

    int stopTimeMs = getDuration0() * 1000;
    timerSprayStop->start(stopTimeMs);
}

void mainwork::clearSpray()
{
    pointerOneByOne = SprayHeads.size();
    pointerRowByRow = 1;

#if 0
    if(ClearSprayState == "开始")
    {
        stopWork(true);
        timerClearSpray->start(3000);
        ClearSprayState = "暂停工作";
        return;
    }

    if(ClearSprayState == "暂停工作")
    {
        if(HostMessageStr.contains("StopTask"))
        {
            spray(10,4);
            ClearSprayState = "发送清理命令";
        }
        timerClearSpray->start(100);
        return;
    }

    if(ClearSprayState == "发送清理命令")
    {
        if(HostMessageStr.contains("Exe"))
        {
            ClearSprayState = "正在清理";
        }
        timerClearSpray->start(3000);
        return;
    }

    if(ClearSprayState == "正在清理")
    {
        stopWork(false);
        for (int i = 0; i < SprayHeads.size(); ++i)
        {
            emit sprayTimeUp(i + 1);
        }
        timerClearSpray->stop();
        ClearSprayState = "完成";
        ClearSprayState = "开始";
    }
#else
    //spray(10,4);
    stopWork(false);
    for (int i = 0; i < SprayHeads.size(); ++i)
    {
        emit sprayTimeUp(i + 1);
    }
    timerClearSpray->stop();
    QJsonObject oderSpray;
    SendOder(ClearSpray, oderSpray);
#endif
}

void mainwork::sprayRowByRow()
{
    if(pointerOneByOne < SprayHeads.size() && pointerRowByRow == 0)
    {
        emit sprayTimeUp(pointerRowByRowId);
        pointerRowByRowId = SprayHeads[pointerOneByOne].toObject()["SprayID"].toString().toInt();
        trySpray(pointerRowByRowId, 10);
        pointerOneByOne += 2;
        timerSprayRowByRow->start(sprayTime * 1000);
        return;
    }

    if(pointerRowByRow == 0)
    {
        emit sprayTimeUp(pointerRowByRowId);
        pointerOneByOne = 1;
        pointerRowByRow = 1;
        timerSprayRowByRow->start(100);                                                            //更新pointerOneByOne的值
        return;
    }

    if(pointerOneByOne < SprayHeads.size())
    {
        emit sprayTimeUp(pointerRowByRowId);
        pointerRowByRowId = SprayHeads[pointerOneByOne].toObject()["SprayID"].toString().toInt();
        trySpray(pointerRowByRowId, 10);
        pointerOneByOne += 2;
        timerSprayRowByRow->start(sprayTime * 1000);
        return;
    }

    emit sprayTimeUp(pointerRowByRowId);
    pointerOneByOne = 0;
    pointerRowByRow = 0;
    timerSprayRowByRow->stop();
}

void mainwork::sprayStop()
{
    emit sprayTimeUp(pointerOneByOne + 1);
    qint64 endTick = getCurrentMSecsSinceEpoch();
    qDebug() << GetCurrentTime() <<"喷雾停止" <<" 用时:"
             << endTick - startTick << "ms endTick:" << endTick;
    timerSprayStop->stop();
    for (int i = 0; i < SprayHeads.size(); ++i)
    {
        emit sprayTimeUp(i + 1);
    }
}

void mainwork::sprayOneByOne()
{
    if(pointerOneByOne < SprayHeads.size())
    {
        emit sprayTimeUp(pointerOneByOneId);
        pointerOneByOneId = SprayHeads[pointerOneByOne].toObject()["SprayID"].toString().toInt();
        trySpray(pointerOneByOneId, 10);
        timerSprayOneByOne ->start(sprayTime * 1000);
        qDebug() << "位置:" + QString::number(pointerOneByOne + 1) + ",ID:" << pointerOneByOneId;
        pointerOneByOne++;
        return;
    }
    emit sprayTimeUp(pointerOneByOneId);
    pointerOneByOne = 0;
    timerSprayOneByOne->stop();
}

int mainwork::getDuration0()
{
    QJsonArray SprayModeTable = CurrentDevice["Table"].toObject()["SprayModeTable"].toArray();
    int Duration = 0;
    if(SprayModeTable.size())
    {
        Duration = SprayModeTable[0].toObject()["Duration"].toInt();
        startTick = getCurrentMSecsSinceEpoch();
        qDebug() << GetCurrentTime() << "本次喷雾时长:" << Duration << "startTick:" << startTick;
    }
    return Duration;
}

void mainwork::sprayByTime(){}

void mainwork::resetHost()
{
    QJsonObject oderSpray;
    SendOder(SystemReset, oderSpray);
}

void mainwork::stopWork(bool nowork)                                                               //true 停止    false 取消停止
{
    QJsonObject oderSpray;
    oderSpray.insert("Status", nowork ? 1 : 0);                                                    //0 取消停止    1 停止
    SendOder(StopTaskExecution, oderSpray);
}

void mainwork::clearSprinklers()
{
    //清空喷头
    NetDebug::post(QString(__FUNCTION__));

    if(CurrentDevice["HYID"].toString() == "") return;

    SprayHeads = QJsonArray();
    SpraysData.insert("data",SprayHeads);
    SpraysString = JsonObject2String(SpraysData);

    for (int i = 0; i < Plans.size(); ++i)
    {
        QJsonObject Plan = Plans[i].toObject();
        QJsonArray PlanSpray = Plan["PlanSpray"].toArray();
        if(PlanSpray.size() == MAX_SPRAYHEAD_COUNT)
        {
            for (int j = 0; j < MAX_SPRAYHEAD_COUNT; ++j)
            {
                PlanSpray[j] = false;
            }
        }
        Plan["PlanSpray"] = PlanSpray;
        Plans[i] = Plan;
    }

    CurrentDevice.insert("Plans", Plans);
    CurrentDevice.insert("allSpray", SpraysData);

    saveHost();
}

bool mainwork::setSprayID(QString spraySN, int ID)
{
    QJsonObject ControlOderData;

    ControlOderData.insert("SprayID", ID);
    ControlOderData.insert("SpraySN", spraySN);

    return SendOder(SetSprayHeadID, ControlOderData);
}

void mainwork::onGotSpray(QString spraySN, int ID)
{
    NetDebug::post("find a sprinkler, ID:" + QString::number((ID))+ ", SN:" + spraySN );
    //emitHostResponse("喷头"+ spraySN, true);

    QJsonObject aSpray;
    aSpray.insert("HYID", spraySN);
    aSpray.insert("State", "添加成功");
    aSpray.insert("UpdateTime", GetCurrentTime("yyyy.MM.dd hh:mm"));

    CurrentSpray = aSpray;

    QString method = SprayHyidExits(CurrentSpray["HYID"].toString()) ? "更新" : "新增";

    if(method == "更新")
    {
        CurrentSpray.insert("Plan", SprayHeads[CurrentSprayindex].toObject()["Plan"].toString());
        CurrentSpray.insert("Position", SprayHeads[CurrentSprayindex].toObject()["Position"].toString());
        SprayHeads.removeAt(CurrentSprayindex);
    }

    if(method == "新增")
    {
        CurrentSpray.insert("Plan", "");
        CurrentSprayindex = SprayHeads.size();
    }

    //CurrentSpray.insert("SprayID", ID);
    CurrentSpray.insert("SprayID", QString::number(ID));

    SprayHeads.insert(CurrentSprayindex, CurrentSpray);

    std::sort(SprayHeads.begin(), SprayHeads.end(), [](const QJsonValue &v1, const QJsonValue &v2)
    {
        return v1.toObject()["SprayID"].toString().toInt() < v2.toObject()["SprayID"].toString().toInt();
    }); //必须使用 inline void swap(QJsonValueRef v1, QJsonValueRef v2)...

    SpraysData.insert("data", SprayHeads);
    SpraysString = JsonObject2String(SpraysData);
    if(CurrentDevice["HYID"].toString() == "") return;

    CurrentDevice.insert("allSpray", SpraysData);
    saveHost();

    viewDevice(CurrentDeviceIndex);
}

int mainwork::getSprayCountAndMaxID(int &maxID)
{
    int count = SprayHeads.size();
    int id;
    maxID = -1;

    if (count == 0)
    {
        ;
    }
    else {
        for (int i = 0; i < count; ++i) {
            id = SprayHeads[i].toObject()["SprayID"].toString().toInt();
            if (id > maxID)
            {
                maxID = id;
            }
        }
    }

    return count;
}

bool mainwork::parseSpraySNFromResp(QJsonObject &data)
{
    bool ret = false;
    //{"SprayID":1,"SpraySN":"HYACHNSDSSH0202012310308"}
    if ((data.find("SprayID") != data.end()) && (data.find("SpraySN") != data.end()))
    {
        ret = true;

        LogFile("SpraySN + SprayID");

        QString sn = data["SpraySN"].toString();
        int id = data["SprayID"].toInt(0);

        if (!sn.isEmpty()  && (id != 0))
        {
            onGotSpray(sn, id);
        }
    }

    return ret;
}

bool mainwork::requestMainCtrlInfo(QString sn, MainCtrlInfo &info, QString &errorMsg)
{
    bool ret = true;
    QJsonObject property;

    info.reset();

    if (isConnect2Host())
    {
        return false;
    }

    ret = iotMsg.deviceStatus(sn, property, errorMsg);
    if (!ret)
    {
        return false;
    }

    /*
    {
        "DeviceID": 517,
        "DeviceSN": "HYACHNSDSCM0202206062222",
        "Enabled": true,
        "Error": 262154,
        "I": 0,
        "IsNB": false,
        "LastOfflineTime": 1654875343,
        "LastOnlineTime": 1654869823,
        "NCSQ": 24,
        "OnlineStatus": "online",
        "T": 2668,
        "V": 3581,
        "W": 1,
        "WCSQ": 0
    }
    */

    NetDebug::post(property);
    if (property.find("OnlineStatus") != property.end())
    {
        QString onlineStatus = property.take("OnlineStatus").toString();

        if (onlineStatus.compare("online", Qt::CaseInsensitive) != 0)
        {
            //errorMsg = "设备不在线！";
            info.onlineStatus = HOST_ONLINE_STATUS_OFFLINE;
        }
        else {
            info.onlineStatus = HOST_ONLINE_STATUS_ONLINE;
        }

        NetDebug::post("device is online");
    }

    if (property.find("Error") != property.end())
    {
        unsigned int error = static_cast<uint>(property.take("Error").toInt());
        unsigned int status = error & (1 << 19);

        QString msg = "ErrorCode:" + QString::number(error);
        msg += " status:" + QString::number(status);

        if (status == 0) //1表示停机；0表示复机（正常计划喷雾）
        {
            info.workStatus = HOST_WORK_STATUS_WORKING;
            msg += " (Working)";
        }
        else {
            info.workStatus = HOST_WORK_STATUS_HOLD;
            msg += " (Non-Working)";
        }

        NetDebug::post(msg);
    }

    if (property.find("IsNB") != property.end())
    {
        bool isNBConnection = property.take("IsNB").toBool(false);

        if (isNBConnection)
        {
            info.connectionMode = HOST_CONNECTION_MODE_NB;
            NetDebug::post("device' connectionMode is NB");
        }
        else {
            info.connectionMode = HOST_CONNECTION_MODE_WIFI;
            NetDebug::post("device' connectionMode is WiFi");
        }
    }

    return true;
}

void mainwork::enterWriteIotPlan(QString sn, MainCtrlInfo& info, QString &errorMsg)
{
    if (info.workStatus == HOST_WORK_STATUS_WORKING)
    {
        iotMsg.setHostStatus(sn, false, errorMsg);
    }

    return;
}

void mainwork::leaveWriteIotPlan(QString sn, MainCtrlInfo& info)
{
    // lastStatus:true 表示停机, false表示复机（正常计划喷雾）
    if (info.workStatus == HOST_WORK_STATUS_WORKING)
    {
        QString errorMsg;
        iotMsg.setHostStatus(sn, true, errorMsg);
    }

    return;
}

void mainwork::sendOrderToHost()
{
    if(APState == "未连接")
    {
        emitHostResponse("请连接主机WiFi");
        qDebug().noquote() << "请连接主机WiFi";
        return;
    }
    int waitSecs = 1;
    Address = 134344704;
    // 清空
    Spray = "";
    ControlOders GSR = GetSprayRecord;
    QJsonObject recordOrder;

    for(int i = 0; i < 16; ++i)
    {
        recordOrder.insert("Add", Address + i * 256);
        recordOrder.insert("Len", 256);
        SendOder(GSR, recordOrder, waitSecs * 10);
    }

    qDebug().noquote() << QString("指令加入队列");
}

void mainwork::processingSprinklersInformation(QJsonObject Rep)
{
    // Spray的拼接
    Spray += Rep["Data"].toObject()["Spray"].toString().toLatin1();

    if(Spray.size() == 8192)
    {
        // 文件存放路径
        QString Path = rootPath + jsonPath;

        // 获取当前主机序列号
        QString hostSN = CurrentDevice.value("HYID").toString().toLatin1();

        // 获取当前时间
        QDateTime dateTime = QDateTime::currentDateTime();
        QString currentDate = dateTime.toString("yyyyMMdd");
        QString str_dateTime = dateTime.toString("yyyyMMddhhmmss");
        QString str_dateTimez = dateTime.toString("yyyyMMddhhmmsszzz");

        if(!QDir(Path).exists())
        {
            QDir().mkpath(Path);
        }

        // 创建QFile对象，同时指定要操作的文件
        QFile sprayFile(Path + "/" + hostSN + "(" + currentDate + ")");

        // 对文件进行写操作
        if(!sprayFile.open(QIODevice::WriteOnly))
        {
            udpw.dbg("打开" + Path + "/" + hostSN + "(" + currentDate + ")出错");
            return;
        }

        int ss = Spray.size();
        qDebug().noquote() << "Spray的大小为: " << ss;

        QJsonObject jsonData;
        jsonData.insert("Spray", QString(Spray));

        QJsonObject jsonPayload;
        jsonPayload.insert("Cmd", GetSprayRecord);
        jsonPayload.insert("Data", jsonData);
        jsonPayload.insert("IdNum", 1);

        QString productid = "D4CE5WEHQB";
        QString topic = productid + "/" + hostSN + "/" + "rs485_reply";
        QJsonObject jsonHost;
        jsonHost.insert("payload", jsonPayload);
        jsonHost.insert("timemills", str_dateTimez);
        jsonHost.insert("seq", 1);
        jsonHost.insert("timestamp", str_dateTime);
        jsonHost.insert("topic", topic);
        jsonHost.insert("devicename", hostSN);
        jsonHost.insert("productid", productid);

        QJsonDocument mydoc;
        mydoc.setObject(jsonHost);
        // 紧凑型
        //        QByteArray myjson = mydoc.toJson(QJsonDocument::Compact);
        // 标准
        QByteArray myjson = mydoc.toJson(QJsonDocument::Indented);

        // 将Spray写入文件
        sprayFile.write(myjson);
        sprayFile.close();

        return;
    }
}

QStringList mainwork::viewFiles()
{
    // 需要查找的路径
    QString Path = rootPath + jsonPath;
    QDir fileDir(Path);

    // 设置过滤配置,接收文件
    fileDir.setFilter(QDir::Files);
    QFileInfoList fileInfoList = fileDir.entryInfoList();
    QStringList fileList;

    for(int inf = 0; inf < fileInfoList.count(); ++inf)
    {
        // 如果需要筛选指定文件可以在这里添加判断
        QString fileName = fileInfoList.at(inf).fileName();
        fileList.push_back(fileName);
    }
    qDebug().noquote() << "文件名数量是: " << fileInfoList.count();
    qDebug().noquote() << "文件名是: " << fileList;
    qDebug().noquote() << "文件路径是: " << Path;

    return fileList;
}

void mainwork::uploadSprayRecord()
{
    QString Path = rootPath + jsonPath;

    QStringList fileList = viewFiles();
    int fileNum = fileList.size();

    for(int i = 0; i < fileNum; ++i)
    {
        QFile sprayFile(Path + "/" + fileList[i]);
        QString deviceSN = fileList[i].left(24);

        if(sprayFile.open(QIODevice::ReadOnly))
        {
            QByteArray myjson = sprayFile.readAll();
            sprayFile.close();

            QString url = "https://hysds.hyairo.vip";
            QString api = "/api/v2/receiver/rs485_reply";
            url += api;
            QByteArray res = iotMsg.sendReq(url, myjson);
            qDebug().noquote() << "res: " << res;
            emitHostResponse("正在上传");

            if(ByteArray2JsonObject(res)["Code"].toInt() == 400)
            {
                sprayFile.remove();
            }
        }
        else
        {
            qDebug().noquote() << "打开文件失败。";
        }
    }
    emitHostResponse("上传完成");
}
