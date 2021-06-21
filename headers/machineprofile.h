#ifndef MACHINEPROFILE_H
#define MACHINEPROFILE_H

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include "accesscontrol.h"
#include "disposethreadwithapi.h"

/**************************************************
*   文件名: machineprofile.h and machineprofile.cpp
*   依赖文件: accesscontrol.h and accesscontrol.cpp disposethreadwithapi.h disposethreadwithapi.cpp
*   工程引用: network concurrent
*   文件作用：使得用户可以在OCTOBTT上可以保存并修改参数配置信息(只支持用户使用一个profile)
*   文件用途: 使得用户减少在octoprint上的操作
*
*   使用举例:
*       //外部连接信号machineProfileInfo接收API返回的机器默认参数信息
*       connect(myMachineProfile, &MachineProfile::machineProfileInfo, this, [=](QVariant tempVar){
*         MechanicalParameter tempMec = tempVar.value<MechanicalParameter>();
*       });
*       myMachineProfile = new MachineProfile(this);
*
*      //获取机器默认参数信息
*      myMachineProfile->getMachineProfileInfo();
*
*      //设置新的机器参数信息
*      tempPostMec 为结构体 MechanicalParameter
*      myMachineProfile->postMachineProfile(tempPostMec);
*
*      最后修改日期: 2021 / 4 / 27
**************************************************/

extern AccessControl *globalAccess;

//储存机器配置信息
struct MechanicalParameter
{

    bool inverted_e;
    double speed_e;

    bool inverted_z;
    double speed_z;

    bool inverted_y;
    double speed_y;

    bool inverted_x;
    double speed_x;

    QString model;

    double width;
    double depth;
    double height;

    QString origin;

    double nozzleDiameter;
};
Q_DECLARE_METATYPE(MechanicalParameter);

class MachineProfile : public QObject
{
    Q_OBJECT
public:
    explicit MachineProfile(QObject *parent = nullptr);

    //获取机器参数文件
    void getMachineProfileInfo();

    //建立新的机器参数文件
    void postMachineProfile(MechanicalParameter profileInfo);

public:
    MechanicalParameter MechanicalParameter_default;
signals:
    //抛出用户信息
    void machineProfileInfo(QVariant  profileInfo);
    void machineProfile_Json(QString  json);
private:
    QJsonObject QObj;
};

#endif // MACHINEPROFILE_H
