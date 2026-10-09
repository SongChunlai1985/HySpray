#include <QJsonArray>

#include "deviceman.h"
#include "mainwork.h"

bool parseSprayPlan(QJsonObject& plan, PlanInfo& planInfo)
{
    NetDebug::post(QString::asprintf("%s(): line:%d", __FUNCTION__, __LINE__));

    planInfo.reset();
    if (plan.find("ID") == plan.end())
    {
        return false;
    }

    int planID = plan["ID"].toInt(0);
    if ((planID < 1) || (planID > PLAN_COUNT))
        return false;

    planInfo.ID = planID;
    planInfo.workDay = plan["Day"].toInt(PLAN_WORKDAY_WORKINGDAY);
    planInfo.sprayMod = plan["Spraying"].toInt(PLAN_SPRAYMODE_VARSPEED);
    planInfo.workMode = plan["Working"].toInt(PLAN_WORKMODE_TIMLING);

    QJsonObject group;
    QJsonArray sprays;
    int sprayNum = 0;

    group = plan["Group"].toObject();
    if (!group.isEmpty())
    {
        sprayNum = group["SprayNum"].toInt(0);
        if (sprayNum > 0)
        {
            sprays = group["Sprays"].toArray();
            //NetDebug::post(sprays);
            for (int count = 0; count < sprayNum; count++) {
                if (count < sprays.size()){
                    QJsonObject item;

                    item = sprays[count].toObject();
                    int id = item["ID"].toInt(0);
                    //NetDebug::post(QString::asprintf("Sprays ID: id %d", id));
                    if ((id > 0) && (id <= MAX_SPRAYHEAD_COUNT))
                    {
                        planInfo.sprays[id-1] = id;
                    }
                }
            }
            //NetDebug::post(QString::asprintf("planInfo.sprays: %d %d %d %d", planInfo.sprays[0], planInfo.sprays[1], planInfo.sprays[2], planInfo.sprays[3]));
        }
    }

    QJsonObject periods;
    QJsonArray periodInfo;
    int NumOfPeriod = 0;

    periods = plan["Periods"].toObject();
    if (!periods.isEmpty())
    {
        NumOfPeriod = periods["NumOfPeriod"].toInt(0);
        if (NumOfPeriod > 0)
        {
            periodInfo = periods["Period"].toArray();

            for (int count = 0; count < NumOfPeriod; count++) {
                if (count < periodInfo.size()){
                    QJsonObject item;
                    //QString val;

                    item = periodInfo[count].toObject();
                    int id = item["ID"].toInt(0);
                    if ((id > 0) && (id <= TIMING_COUNT))
                    {
                        WorkingPeriod *pWorkingPeriod = &planInfo.periods[id-1];

                        if (planInfo.workMode == PLAN_WORKMODE_CYCLING)
                        {
                            pWorkingPeriod->sprayTime = PeriodTime::format(item["Duration"].toInt(0));
                            pWorkingPeriod->intervalTime = PeriodTime::format(item["Interval"].toInt(0));
                            pWorkingPeriod->startTime = PeriodTime::format(item["StartHour"].toInt(0), item["StartMin"].toInt(0));
                            pWorkingPeriod->endTime = PeriodTime::format(item["EndHour"].toInt(0), item["EndMin"].toInt(0));
                        }
                        else if (planInfo.workMode == PLAN_WORKMODE_TIMLING){
                            pWorkingPeriod->sprayTime = PeriodTime::format(item["Duration"].toInt(0));
                            pWorkingPeriod->startTime = PeriodTime::format(item["Hour"].toInt(0), item["Min"].toInt(0));
                        }
                    }
                }
            }
            planInfo.isValidPlan = true;
        }
    }

    return true;
}

QString PeriodTime::format(int value)
{
    QString ret = "";
    ret = QString("%1").arg(value/60, 2, 10, QLatin1Char('0')) + ":" + QString("%1").arg(value%60, 2, 10, QLatin1Char('0'));
    return ret;
}

QString PeriodTime::format(int val1, int val2)
{
    QString ret = "";
    ret = QString("%1").arg(val1, 2, 10, QLatin1Char('0')) + ":" + QString("%1").arg(val2, 2, 10, QLatin1Char('0'));
    return ret;
}

DeviceMan::DeviceMan(){}

QJsonObject PlanInfo::toJson()
{
    QJsonObject plan;
    QString name;

    if (ID > 0 && ID <= PLAN_COUNT)
    {
        plan.insert(QString::asprintf("Plan%d", ID-1), ID);
        if (sprayMod == PLAN_SPRAYMODE_VARSPEED)
        {
            plan.insert("isConstSpead", false);
            plan.insert("isVarSpeed", true);
        }
        else {
            plan.insert("isConstSpead", true);
            plan.insert("isVarSpeed", false);
        }

        if (workDay == PLAN_WORKDAY_WORKINGDAY)
        {           
            plan.insert("isWorkday", true);
            plan.insert("isEveryday", false);
        }
        else {
            plan.insert("isWorkday", false);
            plan.insert("isEveryday", true);
        }

        if (workMode == PLAN_WORKMODE_TIMLING)
        {
            plan.insert("isLoop", false);
            plan.insert("isTiming", true);
        }
        else {
            plan.insert("isLoop", true);
            plan.insert("isTiming", false);
        }

        for (int i = 1; i <= TIMING_COUNT; ++i) {
            plan.insert(QString::asprintf("Starttime%d", i), periods[i-1].startTime);
            plan.insert(QString::asprintf("Endtime%d", i), periods[i-1].endTime);
            plan.insert(QString::asprintf("Spraytime%d", i), periods[i-1].sprayTime);
            plan.insert(QString::asprintf("Intervaltime%d", i), periods[i-1].intervalTime);
        }

        QJsonArray sprayArray;
        NetDebug::post(QString::asprintf("sprays: %d, %d", sprays[0], sprays[1]));
        for (int i = 0; i < MAX_SPRAYHEAD_COUNT; i++)
        {
            sprayArray.insert(i, Sprayer::isValidID(sprays[i]));
        }
        plan.insert("PlanSpray", sprayArray);
    }
    else {
        isValidPlan = false;
    }

    return plan;
}

PlanInfo PlanInfo::fromJson(QJsonObject plan)
{
    return *this;
}

bool Sprayer::isValidID(int id)
{
    if ((id > 0) && (id <= MAX_SPRAYHEAD_COUNT))
    {
        return true;
    }
    else {
        return false;
    }
}

MainController::MainController(QString sn)
{
    SN = sn;
}

bool MainController::fromDownloadJson(QJsonObject dlDevInfo){}

QJsonObject MainController::toSaveJson(){}
/*{
    "ActiveTime": 1644675379,
    "Address": "鍗椾含璺?,
    "CreatedTime": 1632449897,
    "Description": "鍔炲叕瀹ら噷渚?,
    "DeviceID": 226,
    "DeviceSN": "HYACHNSDSCM0202109140031",
    "Enabled": true,
    "FirmwareVersion": "1_12_2_4",
    "LastOfflineTime": 1648766916,
    "LastOnlineTime": 1648769925,
    "Location": "{\n    \"Latitude\": 31.191815,\n    \"Longitude\": 121.377693\n}\n",
    "NumOfSprinklers": 0,
    "OnlineStatus": "online",
    "SSID": "xgh6",
    "TaskPlanLastUpdateTime": "",
    "WiFiPassword": "xgh110918"
}*/

QJsonObject MainController::dlJson2SaveJson(QJsonObject dlDevInfo)
{
#ifdef MOVE_DATA_OPER
    NetDebug::post(QString::asprintf("to do... %s(): line:%d; move data only!", __FUNCTION__, __LINE__));
    // move data only

    QJsonObject saveDev;
    QString addr;

    saveDev.insert("HYID", SN);

    //addr = dlDevInfo["Address"].toString();
    //if (addr.isEmpty())
    {
        addr = dlDevInfo["Description"].toString();
    }
    saveDev.insert("Address", addr);
    saveDev.insert("MapCoordinates", "");
    saveDev.insert("WorkWifiSsid", dlDevInfo["SSID"].toString());
    saveDev.insert("WorkWifiPassword", dlDevInfo["WiFiPassword"].toString());
    saveDev.insert("UpdateTime", GetCurrentTime("yyyy.MM.dd hh:mm"));

    if (dlDevInfo.find("allSpray") != dlDevInfo.end())
    {
        saveDev.insert("allSpray", dlDevInfo["allSpray"].toObject());
    }
    /*

    saveDev.insert("allSpray", dlDevInfo[""]);
    saveDev.insert("UpdateTime", dlDevInfo[""]);
    */
    //saveDev.insert("", dlDevInfo[""]);

    return saveDev;
#else
    QJsonObject saveDev;

    saveDev.insert("HYID", SN);
    saveDev.insert("Address", dlDevInfo["Address"].toString());
    saveDev.insert("MapCoordinates", dlDevInfo["Location"].toString());
    saveDev.insert("WorkWifiSsid", dlDevInfo["SSID"].toString());
    saveDev.insert("WorkWifiPassword", dlDevInfo["WiFiPassword"].toString());
    saveDev.insert("TaskPlanLastUpdateTime", dlDevInfo["TaskPlanLastUpdateTime"].toString());
    saveDev.insert("Organization", dlDevInfo["Organization"].toString());

    if (dlDevInfo.find("allSpray") != dlDevInfo.end())
    {
        saveDev.insert("allSpray", dlDevInfo["allSpray"].toObject());
    }
    /*
    saveDev.insert("allSpray", dlDevInfo[""]);
    saveDev.insert("UpdateTime", dlDevInfo[""]);
    */
    //saveDev.insert("", dlDevInfo[""]);

    return saveDev;
#endif
}

void MainCtrlInfo::reset()
{
    connectionMode = HOST_CONNECTION_MODE_UNKNOW;
    onlineStatus = HOST_ONLINE_STATUS_UNKNOW;
    workStatus = HOST_WORK_STATUS_UNKNOW;
}
