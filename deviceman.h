#ifndef DEVICEMAN_H
#define DEVICEMAN_H

#include <QObject>
#include <QJsonObject>

#define PLAN_COUNT          (3)
#define TIMING_COUNT        (3)
#define MAX_SPRAYHEAD_COUNT (16)
#define INVALID_SPRAY_ID    (0)

enum HOST_CONNECTION_MODE_  //主机上网连接方式
{
    HOST_CONNECTION_MODE_UNKNOW,
    HOST_CONNECTION_MODE_WIFI,
    HOST_CONNECTION_MODE_NB,

    CONNECTION_MODE_MAX
};

enum HOST_ONLINE_STATUS_    //在线状态
{
    HOST_ONLINE_STATUS_UNKNOW,
    HOST_ONLINE_STATUS_OFFLINE,
    HOST_ONLINE_STATUS_ONLINE,

    HOST_ONLINE_STATUS_MAX
};

enum HOST_WORK_STATUS_  //停机 复机
{
    HOST_WORK_STATUS_UNKNOW,
    HOST_WORK_STATUS_HOLD,
    HOST_WORK_STATUS_WORKING,

    HOST_WORK_STATUS_MAX
};

struct WorkingPeriod
{
    QString startTime;
    QString endTime;
    QString sprayTime;
    QString intervalTime;
};

enum PLAN_DEFINES_
{
    PLAN_SPRAYMODE_VARSPEED = 0,
    PLAN_SPRAYMODE_FIXEDSPEED = 1,

    PLAN_WORKMODE_TIMLING = 0,  //
    PLAN_WORKMODE_CYCLING = 1,

    PLAN_WORKDAY_WORKINGDAY = 0,
    PLAN_WORKDAY_WEEKDAY = 1,

    PLAN_DEFINES_EDN
};

struct PlanInfo
{
    bool isValidPlan;
    int ID;
    int sprayMod;   //Spraying mode, 0 for variable fan speed, 1 for fixed fan speed
    int workDay;    //Day, 0 for working day, 1 for week d
    int workMode;   //Working mode, 0 for no cycling, 1 for cycling

    WorkingPeriod periods[TIMING_COUNT];
    int sprays[MAX_SPRAYHEAD_COUNT] = {0};
//    QString startTime1;
//    QString endTime1;
//    QString sprayTime1;
//    QString intervalTime1;

//    QString startTime2;
//    QString endTime2;
//    QString sprayTime2;
//    QString intervalTime2;

//    QString startTime3;
//    QString endTime3;
//    QString sprayTime3;
//    QString intervalTime3;
    PlanInfo()
    {
        reset();
    }

    void reset()
    {
        isValidPlan = false;
        ID = 0;
        sprayMod = 0;
        workDay = 0;
        workMode = 0;

        memset(sprays, INVALID_SPRAY_ID, sizeof (sprays));
        for (int i = 0; i< TIMING_COUNT; i++)
        {
            periods[i].startTime = "";
            periods[i].endTime = "";
            periods[i].sprayTime = "";
            periods[i].intervalTime = "";
        }
    }

    QJsonObject toJson();
    PlanInfo fromJson(QJsonObject plan);
};

bool parseSprayPlan(QJsonObject& plan, PlanInfo& planInfo);

class PeriodTime
{
public:
    static QString format(int value);
    static QString format(int val1, int val2);
};

class Sprayer
{
public:
    QString SN;
    static bool isValidID(int id);

private:

};

class MainCtrlInfo
{
public:
    int connectionMode = HOST_CONNECTION_MODE_UNKNOW;
    int onlineStatus = HOST_ONLINE_STATUS_UNKNOW;
    int workStatus = HOST_WORK_STATUS_UNKNOW;

public:
    void reset();
};

class MainController
{
public:
    QString SN;

    PlanInfo plans[PLAN_COUNT];

    MainController(QString sn);
    bool fromDownloadJson(QJsonObject dlDevInfo);
    QJsonObject toSaveJson();

    QJsonObject dlJson2SaveJson(QJsonObject dlDevInfo);

private:

};

class DeviceMan
{
public:
    DeviceMan();
};

#endif // DEVICEMAN_H
