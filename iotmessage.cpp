#include "iotmessage.h"
#include "mainwork.h"

#define WEB_SERVER_URL  "https://hysds.hyairo.vip"
#define INVALID_RETURN_CODE (-1)

#define USE_DEVICE_PUBLISH  (true)

#if USE_DEVICE_PUBLISH
#define RESPONSE_ITEM_NAME ("Resp")
#define RESPONSE_CODE_NAME ("Code")
#else
#define RESPONSE_ITEM_NAME ("Message")
#define RESPONSE_CODE_NAME ("Results")
#endif

iotMessage::iotMessage()
{
    hasLogin = false;
    token = "";
}

QString iotMessage::getMessage(QString url)
{
    acount_priority = USER_PRIORITY_NONE;

    Url = QUrl(url);
    IotGet.tcpsender->abort();
    IotGet.tcpsender->connectToHost(Url.host(), u_short(80));

    QEventLoop loop;
    connect(IotGet.tcpsender, &QTcpSocket::connected,  &loop, &QEventLoop::quit) ;
    loop.exec();

    httpConnected();

    connect(IotGet.tcpsender, &QTcpSocket::readyRead,  &loop, &QEventLoop::quit) ;
    loop.exec();

    return httpPageReady();
}

QString iotMessage::httpPageReady()
{
    QByteArray receivemsg = IotGet.tcpsender->readAll();
    //qDebug().noquote()<<__FUNCTION__<<receivemsg;
    QString rcvmsg(receivemsg);
    QString char13101310  = "\r\n\r\n";
    int startpoint = rcvmsg.indexOf(char13101310) + 4 ;
    QString data = rcvmsg.mid(startpoint);
    return data;
}

void iotMessage::httpConnected()
{
    QString request =
            QString(QString() +
                    "GET " + Url.path() + "?" + Url.query() + " HTTP/1.1\r\n" +
                    "Host: " + Url.host() + "\r\n" +
                    "Connection: keep-alive\r\n" +
                    "User-Agent: Mozilla/5.0\r\n" +
                    "Accept: application/json\r\n" +
                    "Accept-Encoding: identity\r\n" +
                    "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n"
                    );
    IotGet.tcpsender->write(request.toUtf8());
    //qDebug().noquote()<<__FUNCTION__<<request.toUtf8();
}
#if 0
QString iotMessage::login(QString url, QString username, QString password)
{
    Url = QUrl(url);
    httplogin(username, password);
    QEventLoop loop;

    QSslConfiguration conf = Request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1_2);
    Request.setSslConfiguration(conf);
    Request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    Request.setUrl(Url);                                               //https
    QNetworkReply *pReply = pManager->post(Request, loginJsonByteArray.data());
    qDebug().noquote()<<__FUNCTION__<<"loginJsonByteArray:"<<loginJsonByteArray;

    connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(pReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray Replydata = pReply->readAll();
    //qDebug()<<__FUNCTION__<<"Replydata:"<<Replydata;
    return Replydata;
}
#endif

void getErrorMsg(int code, QString& errorMsg)
{
    switch (code) {
    case -1:
        errorMsg = "请检查网络!";
        break;

    case 0:
        errorMsg = "success";
        break;

    case 400:
        errorMsg = "Parameters are wrong";
        break;

    case 401:
        errorMsg = "Token is wrong";
        break;

    case 402:
        errorMsg = "Authorization check is failed, the method is not allowed";
        break;

    case 500:
        errorMsg = "Internal server error";
        break;

    default:
        errorMsg = "Unknow error";
        break;
    }
}

int iotMessage::pickData(QJsonObject& content, QJsonObject& data)
{
    int ret = INVALID_RETURN_CODE;
    data.empty();
    if (content.find("Code") != content.end())
    {
        ret = content.take("Code").toInt();
        data = content.take("Data").toObject();
    }

    return ret;
}

bool iotMessage::checkServer()
{
    QString errorMsg;

    bool ret = false;
    QString url = WEB_SERVER_URL;
    QString api = "/api/v2/user/login";
    QJsonObject content;
//    content.insert("Username", username);
//    content.insert("Password", password);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        ret = true;
        if (data.find("Token") != data.end())
        {
            token = data.take("Token").toString();
            if (!token.isEmpty())
            {
                loginTime = QDateTime::currentDateTime();
                hasLogin = true;
            }
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::checkToken()
{
    QDateTime now = QDateTime::currentDateTime();

    if (token.isEmpty())
        return false;

    if (loginTime.secsTo(now) > 36000*24)
    {
        return false;
    }

    return true;
}

bool iotMessage::login(QString username, QString password, QString& errorMsg)
{
    bool ret = false;
    QString url = WEB_SERVER_URL;
    QString api = "/api/v2/user/login";
    QJsonObject content;
    content.insert("Username", username);
    content.insert("Password", password);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        ret = true;

        if (data.find("Token") != data.end())
        {
            token = data.take("Token").toString();
            if (!token.isEmpty())
            {
                loginTime = QDateTime::currentDateTime();
                hasLogin = true;
            }
        }

        if (data.find("Authority") != data.end())
        {
            QString userLevel = data.take("Authority").toString();

            if (userLevel.compare("guest", Qt::CaseInsensitive) == 0)
            {
                acount_priority = USER_PRIORITY_GUEST;
            }
            else if (userLevel.compare("superAdmin", Qt::CaseInsensitive) == 0)
            {
                acount_priority = USER_PRIORITY_SUPERADMIN;
            }
            else {
                acount_priority = USER_PRIORITY_USER;
            }
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

int iotMessage::getAccountPriority()
{
    return acount_priority;
}

bool iotMessage::deviceList(QJsonObject &devList, QString& errorMsg, bool isIncludeChild, int organizationID)
{
    bool ret = false;
    QString api = "/api/v2/device/list";
    QJsonObject content;
    content.insert("IncludeChild", isIncludeChild);
    if (organizationID != -1)
    {
        content.insert("OrganizationID", organizationID);
    }
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        int devCount = 0;
        devCount = data["Count"].toInt();
        if (devCount > 0)
        {

            //devList.insert("Devices", data["Devices"].toObject());
            devList = data;
        }
        else {
            devList = QJsonObject();
        }
        ret = true;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::deviceRegister(QString sn, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/device/register";
    QJsonObject content;
    content.insert("DeviceSN", sn);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        int result = data["Result"].toInt();    //0 for succeed, 1 for serial number error, 2 for duplicated
        if (result == 2)
        {
            errorMsg = "serial number error";
        }
        else {
            ret = true;
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::deviceUpdate(QString sn, QJsonObject property, QString& errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/device/update";
    QJsonObject content;
    NetDebug::post(property);
    QByteArray propBase64 = JsonObject2ByteArray(property).toBase64();
    content.insert("DeviceSN", sn);
    content.insert("Property", propBase64.toStdString().c_str());
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        ret = true;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::deviceInfo(QString sn, QJsonObject& property, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/device/info";
    QJsonObject content;
    content.insert("DeviceSN", sn);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        QByteArray propByteArr;

        if (data.find("Property") != data.end())
        {
            QByteArray propByteArr = QByteArray::fromBase64(QByteArray::fromStdString(data["Property"].toString().toStdString()));            
            QString propStr = QString::fromUtf8(propByteArr);

            content = String2JsonObject(propStr);
            data.remove("Property");

            QVariantMap jmap = data.toVariantMap();
            jmap.unite(content.toVariantMap());
            property = QJsonObject::fromVariantMap(jmap);
        }
        else {
            property = data;
        }
        ret = true;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::deviceStatus(QString sn, QJsonObject &property, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/device/status";
    QJsonObject content;
    content.insert("DeviceSN", sn);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        property = data;
        ret = true;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::deviceStatusUpdate(QString sn, QJsonObject &property, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/device/status/update";
    QJsonObject content;
    content.insert("DeviceSN", sn);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        ret = true;
        property = data;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::sprinklerRegister(QString deviceSN, QString sprinklerSN, int sprinklerID, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/sprinkler/register";
    QJsonObject content;
    content.insert("DeviceSN", deviceSN);
    content.insert("ID", sprinklerID);
    content.insert("SprinklerSN", sprinklerSN);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        if (data.find("Result") != data.end())
        {
            int re = data["Result"].toInt();
            //0 for succeed, 1 for serial number error, 2 for duplicated serial number, 3 for ID error, 4 for duplicated ID
            switch (re) {
            case 0:
                ret = true;
                break;

            case 1:
                errorMsg = "serial number error";
                break;

            case 2:
                //errorMsg = "duplicated serial number";
                ret = true;
                break;

            case 3:
                errorMsg = "ID error";
                break;

            case 4:
                errorMsg = "duplicated ID";
                ret = true; //支持重复操作
                break;

            default:
                errorMsg = "服务器返回值异常";
                break;
            }
        }
        else {
            errorMsg = "服务器返回数据异常";
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::sprinklerUnregister(QString deviceSN, QString sprinklerSN, int sprinklerID, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/sprinkler/unregister";
    QJsonObject content;
    content.insert("DeviceSN", deviceSN);
    content.insert("ID", sprinklerID);
    content.insert("SprinklerSN", sprinklerSN);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        if (data.find("Result") != data.end())
        {
            int re = data["Result"].toInt();
            //0 for succeed, 1 for serial number error, 2 for duplicated serial number, 3 for ID error, 4 for duplicated ID
            switch (re) {
            case 0:
                ret = true;
                break;

            case 1:
                errorMsg = "serial number error";
                break;

            case 2:
                errorMsg = "duplicated serial number";
                break;

            case 3:
                errorMsg = "ID error";
                break;

            case 4:
                errorMsg = "duplicated ID";
                ret = true; //支持重复操作
                break;

            default:
                errorMsg = "服务器返回值异常";
                break;
            }
        }
        else {
            errorMsg = "服务器返回数据异常";
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::sprinklerUpdate(QString deviceSN, int sprinklerID, QJsonObject property, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/sprinkler/update";
    QJsonObject content;
    content.insert("DeviceSN", deviceSN);
    content.insert("ID", sprinklerID);
    QByteArray propBase64 = JsonObject2ByteArray(property).toBase64();
    content.insert("Property", propBase64.toStdString().c_str());
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        if (data.find("Result") != data.end())
        {
            int re = data["Result"].toInt();
            //0 for succeed, 1 for failed
            switch (re) {
            case 0:
                ret = true;
                break;

            case 1:
                errorMsg = "failed";
                break;

            default:
                errorMsg = "服务器返回值异常";
                break;
            }
        }
        else {
            errorMsg = "服务器返回数据异常";
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::sprinklerInfo(QString deviceSN, int sprinklerID, QJsonObject &property, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/sprinkler/info";
    QJsonObject content;
    content.insert("DeviceSN", deviceSN);
    content.insert("ID", sprinklerID);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        if (data.find("Property") != data.end())
        {
            QByteArray propByteArr = QByteArray::fromBase64(QByteArray::fromStdString(data["Property"].toString().toStdString()));
            QString propStr = QString::fromUtf8(propByteArr);

            content = String2JsonObject(propStr);
            data.remove("Property");

            QVariantMap jmap = data.toVariantMap();
            jmap.unite(content.toVariantMap());
            property = QJsonObject::fromVariantMap(jmap);
        }
        else {
            property = data;
        }
        ret = true;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::sprinklerStatus(QString deviceSN, int sprinklerID, QJsonObject &property, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/sprinkler/status";
    QJsonObject content;
    content.insert("DeviceSN", deviceSN);
    content.insert("ID", sprinklerID);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        ret = true;
        property = data;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::planUpdate(QString sn, QJsonObject plan, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/plan/update";
    QJsonObject content;
    NetDebug::post(plan);
    QByteArray propBase64 = JsonObject2ByteArray(plan).toBase64();
    content.insert("DeviceSN", sn);
    content.insert("UserPlan", propBase64.toStdString().c_str());
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        ret = true;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::planInfo(QString sn, int planNumber, QJsonObject &plan, QString &errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/plan/info";
    QJsonObject content;
    plan = QJsonObject();
    errorMsg = "";
    content.insert("DeviceSN", sn);
    content.insert("ID",planNumber);
    if (planNumber > PLAN_COUNT) return false;

    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        QByteArray propByteArr;

        if (data.find("UserPlan") != data.end())
        {
            QByteArray propByteArr = QByteArray::fromBase64(QByteArray::fromStdString(data["UserPlan"].toString().toStdString()));
            plan = ByteArray2JsonObject(propByteArr);
        }
        else {
            ;
        }
        ret = true;
    }
    else
    {
        if (code == 400)
        {
            // errorMsg = "Parameters are wrong";
            // Plan is NOT exist
            errorMsg = "";
            return false;
        }
        else {
            getErrorMsg(code, errorMsg);
        }
    }

    return ret;
}

bool iotMessage::setHostStatus(QString sn, bool isOn, QString &errorMsg)
{
    bool ret = false;

    ControlOders Cmd = StopTaskExecution;
    QJsonObject Data;
    int status = isOn ? 0:1;
    Data.insert("Status", status);

    NetDebug::post("set Host WorkingStatus:" + QString::number(isOn));

    QByteArray result = action(sn, Cmd, Data);
    QJsonObject content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        int seq = data["Seq"].toInt(-1);
        if (seq == -1) return false;

        //to do:查询、等待操作结果
        for (int i = 0; i < 4; i++) {
            Sleep(1000);
            result = getActionResp(sn, seq);
            content = ByteArray2JsonObject(result);
            code = pickData(content, data);

            if (code == 0)
            {
                QString msg = data[RESPONSE_ITEM_NAME].toString();

                if (!msg.isEmpty())
                {
                    QByteArray propByteArr = QByteArray::fromBase64(QByteArray::fromStdString(msg.toStdString()));
                    QString propStr = QString::fromUtf8(propByteArr);

                    data = String2JsonObject(propStr);

                    if (data[RESPONSE_CODE_NAME].toInt(-1) == 0)
                    {
                        ret = true;
                        errorMsg = "";
                        break;
                    }
                    else {
                        errorMsg = "";
                        break;
                    }
                }
                break;
            }
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::sendPlan(QString devSN, ControlOders Cmd, QJsonObject Data, QString &errorMsg)
{
    bool ret = false;
    errorMsg = "方案发送异常";

    QByteArray result = action(devSN, Cmd, Data);
    QJsonObject content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);

    if (code == 0)
    {
        int seq = data["Seq"].toInt(-1);
        if (seq == -1) return false;

        for (int i = 0; i < 9; i++) {
            Sleep(1000);
            result = getActionResp(devSN, seq);
            content = ByteArray2JsonObject(result);
            code = pickData(content, data);

            if (code == 0)
            {
                QString msg = data[RESPONSE_ITEM_NAME].toString();

                if (!msg.isEmpty())
                {
                    QByteArray propByteArr = QByteArray::fromBase64(QByteArray::fromStdString(msg.toStdString()));
                    QString propStr = QString::fromUtf8(propByteArr);

                    data = String2JsonObject(propStr);

                    if (data["Results"].toInt(-1) == 0)
                    {
                        ret = true;
                        errorMsg = "";
                        break;
                    }
                    else {
                        errorMsg = "写入方案异常";
                        break;
                    }
                }
                break;
            }
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    if (!ret)
    {
        NetDebug::post("sendPlan() failed: Cmd " + QString::number(Cmd));
    }

    return ret;
}

bool iotMessage::cbInfo(QString sn, QJsonObject& property, QString& errorMsg)
{
    bool ret = false;
    QString api = "/api/v2/cb/info";
    QJsonObject content;
    content.insert("SN", sn);
    QByteArray result = post(api, content);
    content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);
    if (code == 0)
    {
        property = data;

        ret = true;
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

bool iotMessage::getSprinklerSN(QString devSN, int sprinklerID, QString& sprinklerSN, QString &errorMsg, int waitSecs)
{
    bool ret = false;
    QDateTime currentTime = QDateTime::fromString(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), "yyyy-MM-dd hh:mm:ss");    //精确到秒
    QDateTime respTime;
    sprinklerSN = "";
    ControlOders Cmd = GetSerialNumberSN;
    QJsonObject Data;

    Data.insert("SprayID", sprinklerID);
    QByteArray result = action(devSN, Cmd, Data);
    QJsonObject content = ByteArray2JsonObject(result);
    QJsonObject data;
    int code = pickData(content, data);

    if (code == 0)
    {
        int seq = data["Seq"].toInt(-1);
        if (seq == -1) return false;

        QString currTimeStr = data["UpdateTime"].toString();
        if (!currTimeStr.isEmpty())
        {
            NetDebug::post("currentTime from " + currentTime.toString("yyyy-MM-dd hh:mm:ss") + " to " + currTimeStr);
             currentTime = QDateTime::fromString(currTimeStr, "yyyy-MM-dd hh:mm:ss");
        }

        for (int i = 0; i < waitSecs; i++) {
            Sleep(1000);
            result = getActionResp(devSN, seq);
            content = ByteArray2JsonObject(result);
            code = pickData(content, data);

            if (code == 0)
            {
#if USE_DEVICE_PUBLISH
                QString updateTime = data["UpdateTime"].toString();

                if (!updateTime.isEmpty())
                {
                    respTime = QDateTime::fromString(updateTime, "yyyy-MM-dd hh:mm:ss");
                    if (respTime < currentTime)
                    {
                        NetDebug::post("Time not fitable - Send:" + currentTime.toString("yyyy-MM-dd hh:mm:ss") + ", resp:" + respTime.toString("yyyy-MM-dd hh:mm:ss"));
                        continue;
                    }
                }
#endif
                QString msg = data[RESPONSE_ITEM_NAME].toString();

                if (!msg.isEmpty())
                {
                    QByteArray propByteArr = QByteArray::fromBase64(QByteArray::fromStdString(msg.toStdString()));
                    QString propStr = QString::fromUtf8(propByteArr);
                    data = String2JsonObject(propStr);
                    NetDebug::post(data);
                    if (data["SprayID"].toInt() == sprinklerID)
                    {
                        sprinklerSN = data["SpraySN"].toString("");
                        if (!sprinklerSN.isEmpty())
                        {
                            ret = true;
                            errorMsg = "";
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    else
    {
        getErrorMsg(code, errorMsg);
    }

    return ret;
}

#if USE_DEVICE_PUBLISH
//  /api/v2/device/cmd
QByteArray iotMessage::action(QString dev, ControlOders Cmd, QJsonObject Data)
{
    QJsonObject Oder;
    Oder.insert("Cmd",Cmd);
    Oder.insert("Data",Data);
    QByteArray ShortOder = shortJson(JsonObject2ByteArray(Oder));                              //必须去掉空格回车换行符 否则主机内存不够用
    NetDebug::post(ShortOder);
    ShortOder = ShortOder.toBase64();
    QJsonObject content;
    content.insert("DeviceSN", dev);
    content.insert("Command", ShortOder.toStdString().c_str());

    //NetDebug::post(JsonObject2ByteArray(content));

    QByteArray ret;
    QString url = WEB_SERVER_URL;
    QString api = "/api/v2/device/cmd";
    url += api;
    ret = sendReq(url, JsonObject2ByteArray(content));
    return ret;
}

//  /api/v2/device/resp
QByteArray iotMessage::getActionResp(QString dev, int seq)
{
    QJsonObject content;
    content.insert("DeviceSN", dev);
    content.insert("Sequence",seq);

    //NetDebug::post(content);

    QByteArray ret;
    QString url = WEB_SERVER_URL;
    QString api = "/api/v2/device/resp";
    url += api;
    ret = sendReq(url, JsonObject2ByteArray(content));
    return ret;
}


#else
//     /api/rs485/publish
QByteArray iotMessage::action(QString dev, ControlOders Cmd, QJsonObject Data)
{
    QJsonObject Oder;
    Oder.insert("Cmd", Cmd);
    Oder.insert("Data", Data);
    QByteArray ShortOder = shortJson(JsonObject2ByteArray(Oder));                              //必须去掉空格回车换行副 否则主机内存不够用
    NetDebug::post(ShortOder);
    ShortOder = ShortOder.toBase64();
    QJsonObject content;
    content.insert("DeviceSN", dev);
    content.insert("Message", ShortOder.toStdString().c_str());

    //NetDebug::post(JsonObject2ByteArray(content));

    QByteArray ret;
    QString url;
    QString api = "/api/rs485/publish";
    url = WEB_SERVER_URL;
    //url += "/";
    url += api;
    ret = sendReq(url, JsonObject2ByteArray(content));
    //NetDebug::post(ret);
    return ret;
}

//  /api/rs485/resp
QByteArray iotMessage::getActionResp(QString dev, int seq)
{
    QJsonObject content;
    content.insert("DeviceSN", dev);
    content.insert("Seq", seq);

    //NetDebug::post(content);

    QByteArray ret;
    QString url;
    QString api = "/api/rs485/resp";
    url = WEB_SERVER_URL;
    //url += "/";
    url += api;
    ret = sendReq(url, JsonObject2ByteArray(content));
    NetDebug::post(ret);
    return ret;
}

#endif

QByteArray iotMessage::post(QString api, QJsonObject data)
{
    QString url;
    url = WEB_SERVER_URL;    
    url += api;

    QByteArray content = JsonObject2ByteArray(data);
    QByteArray ret = sendReq(url, content);
    return ret;
}

QByteArray iotMessage::sendReq(QString url, QByteArray content)
{
    Url = QUrl(url);
    QSslConfiguration conf = Request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1_2);
    Request.setSslConfiguration(conf);
    Request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (!token.isEmpty())
    {
        Request.setRawHeader("Authorization", token.toLatin1());
    }
    Request.setUrl(Url);

    NetDebug::post(url);
    NetDebug::post(content);

    QNetworkReply *pReply = pManager->post(Request, content.data());
    QEventLoop loop;
    connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(pReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray Replydata = pReply->readAll();
    NetDebug::post(Replydata);
    return Replydata;
}

void iotMessage::Sleep(int delay)
{
    QTimer *Delay =  new QTimer(this);
    Delay->start(delay);

    QEventLoop loop;
    connect(Delay, &QTimer::timeout, &loop, &QEventLoop::quit) ;
    loop.exec();
}

QString iotMessage::httpPageReady2()
{
    QByteArray receivemsg = IotGet.tcpsender->readAll();
    qDebug().noquote()<<__FUNCTION__<<""<<"\r"<<receivemsg;
    QString rcvmsg(receivemsg);
    QString char13101310  = "\r\n\r\n";
    int dateStartpoint = rcvmsg.indexOf(char13101310) + 4 ;
    QString head = rcvmsg.left(dateStartpoint);
    QStringList metas = head.split("\r\n");
    QJsonObject headJson;
    foreach (QString meta, metas) {
        QStringList Meta = meta.split(": ");
        if(Meta.size() > 1)headJson.insert(Meta[0], Meta[1]);
    }
    QString cookieStr = headJson["Set-Cookie"].toString().toUtf8();
    int p = cookieStr.indexOf("=");
    qDebug().noquote()<<__FUNCTION__<<JsonObject2String(headJson)<<"cookieStr.left(p):"<<cookieStr.left(p)<<"cookieStr.mid(p+1)"<<cookieStr.mid(p+1);
    cookie = QNetworkCookie(cookieStr.left(p).toUtf8(), cookieStr.mid(p+1).toUtf8());
    qDebug().noquote()<<__FUNCTION__<<cookie.isHttpOnly()<<cookie.path();
    QString data = rcvmsg.mid(dateStartpoint);
    return data;
}

void iotMessage::httpConnected2()
{
    QString request =
            QString("POST " + Url.path() + " HTTP/1.1\r\n" +
                    "Host: " + Url.host() + "\r\n" +
                    "Connection: keep-alive\r\n"
                    "Content-Length: " + QString::number(loginJsonByteArray.size()) + "\r\n"
                    "Accept: application/json\r\n"
                    "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36\r\n"
                    "Content-Type: application/json;charset=UTF-8\r\n"
                    "Origin: http://iot.fyairo.com\r\n"
                    "Referer: http://iot.fyairo.com/HySDS002/user/login?redirect=http%3A%2F%2Fiot.fyairo.com%2FHySDS002%2Fdashboard%2Fmonitor%2F\r\n"
                    "Accept-Encoding: gzip, deflate\r\n"
                    "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n"
                    //"Cookie: session=.1eJwlj0FuAzEIRa9SeZ0FeIxt5iqdKDIGmkrtJhOvoty9SN3A478F8Eo3_xnn3c60f77SxzNa-rXzHF-WLulYXcSPxWQW7MrHKpbHsZoTRt4bRW0AYblHQlh62Nok2Clsmx7cTD1d39dLLHzYeU_787Espm9Ne2qKs_a6iZOyckPijQAVqWkrTgXyAGPuYGoD3YtuoDYLgOXO\r\n"
                    "\r\n"
                    + loginJsonByteArray);
    IotGet.tcpsender->write(request.toUtf8());
    //QNetworkCookie cookie("session",".eJwlzjkOwkAMAMC_uKaw97TzmWh9KbQJqRB_B4l5wbxhzzOuA7bXeccD9qfDBtPJBo-q2V1cJnWpHcmpT58te8OyMEQYw2NRZvOKHtYQo7BIsIdGsWJEytIxVydEjVFQB-pyZxTSRa1MMQwycxpZaqayOvwi9xXnf0Pw-QK-MS_j.X6pQdw.DqrEeyKtU6933fYIajoi62uqntA; HttpOnly; Path=/");
    qDebug().noquote()<<__FUNCTION__<<"\r"<<request.toUtf8();
}

void iotMessage::httplogin(QString username, QString password)
{
    QJsonObject loginJson;

    loginJson.insert("userName", username); //20220309  //loginJson.insert("account", username);
    loginJson.insert("password", password);
    loginJson.insert("type", "account");

    loginJsonByteArray = JsonObject2ByteArray(loginJson);
    loginJsonByteArray.replace(" ","");
    loginJsonByteArray.replace("\n","");
    qDebug().noquote()<<__FUNCTION__<<"loginJsonByteArray"<<loginJsonByteArray.size();
}
