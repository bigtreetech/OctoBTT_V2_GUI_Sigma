#ifndef CONNECTIONDETECTION_H
#define CONNECTIONDETECTION_H

#include <QObject>
#include "accesscontrol.h"
#include "disposethreadwithapi.h"

/**************************************************
*   文件名: connectiondetection.h and connectiondetection.cpp
*   依赖文件: accesscontrol.h and accesscontrol.cpp disposethreadwithapi.h and disposethreadwithapi.cpp
*   工程引用:
*   文件用途: 检测并反馈机器连接状态, 提供选择波特率以及选择设备的接口
*   文件作用：使得用户可以了解机器连接状态, 并可以设置新连接
*
*   使用举例:
*      ConnectionDetection* myconnect = new ConnectionDetection(this);
*      //获取connection信息
*      myconnect->getConnectionInformation();
*
*      //设置新的连接
*      myconnect->postConnectionInformation(port, baudrate);
*
*      //外部接收connection反馈信息封包结构体
*      connect(myconnect, &ConnectionDetection::ConnectionInfoSignal, this, [=](ConnectionInfo tempStruct){
*
*      });
*
*      最后修改日期: 2021 / 5 / 27
**************************************************/

extern AccessControl *globalAccess;

//保存connection获取的信息
struct ConnectionInfo
{
    double baudrate;
    QString port;
    QStringList ports;
    QStringList baudrates;
};
Q_DECLARE_METATYPE(ConnectionInfo);

class ConnectionDetection : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionDetection(QObject *parent = nullptr);

    //获取connection中的port[当前使用的设备]/baudrate[当前使用的波特率]/ports[检测到的设备]/baudrates[可供使用的波特率]
    void getConnectionInformation();

    //设置新的连接[需要用户选择新的设备以及选择新的波特率]
    void postConnectionInformation(QString port, QString baudrate);

public:
    ConnectionInfo  connectionInfoStruct;
signals:
    //connection信息抛出信号
    void ConnectionInfoSignal(ConnectionInfo tempStruct);
};

#endif // CONNECTIONDETECTION_H
