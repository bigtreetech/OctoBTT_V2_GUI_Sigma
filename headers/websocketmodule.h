#ifndef WEBSOCKETMODULE_H
#define WEBSOCKETMODULE_H

#include "accesscontrol.h"
#include <QWebSocket>
#include <QObject>
#include "disposethreadwithapi.h"
#include "terminal.h"
#include "messageParser.h"

/**************************************************
*   文件名: websocketmodule.h and websocketmodule.cpp
*   依赖文件: accesscontrol.h and accesscontrol.cpp
*   工程引用: websocket
*   文件用途: 处理OCTOBTT中的websocket信息的接收以及回复密钥
*
*   使用举例:
*       需要引用全局的accesscontrol的全局globalAccess
*       WebSocketModule *myWebsoket = new WebSocketModule;
*       myWebsoket->webSocketBegin();
*
*       //打印接收的消息
*       connect(myWebsoket, &WebSocketModule::sentMsgtoOut, this, [=](QString msg){
*           qDebug() << msg << "=====================================" << __LINE__ <<  __FILE__;
*       });
*
*       connect(myWebsoket, &WebSocketModule::sentSearchLinkMsg, this, [=](QString msg){
*           qDebug() << msg << "=====================================" << __LINE__ <<  __FILE__;
*           ui->Receiver->append(msg);
*      });
*      //[文件名][文件路径][打印时长][打印进度]
*      connect(myWebsoket, &WebSocketModule::printFileInfo, this, [=](PrintFile tempStruct){
*           qDebug() << __LINE__ << __FILE__ << tempStruct.fileCompletion << tempStruct.filePrintTime << tempStruct.fileName << tempStruct.filePath;
*      });
*      //conntion状态
*      connect(myWebsoket, &WebSocketModule::connectionStateInfo, this, [=](ConnectionStateInfo tempStruct){
*           qDebug() << __LINE__ << __FILE__ << tempStruct.state;
*      });
*      //温度以及z轴
*      connect(myWebsoket, &WebSocketModule::machineParameterInfo, this, [=](MachineParameter tempStruct){
*           qDebug() << __LINE__ << __FILE__ << tempStruct.bedActualTemperature << tempStruct.bedTargetTemperature << tempStruct.currentZ << tempStruct.tool0ActualTemperature << tempStruct.tool0TargetTemperature;
*      });
*
*      在方法webSocketMsgAnalysis中
*      if(typeValue.toString() == "printables")
*      {
*           emit refreshFileInfoSignal(); //这个信号用来通知functionmodule重新获取最新的文件夹结构
*      }
*
*      最后修改日期: 2021 / 5 / 27
**************************************************/

extern AccessControl *globalAccess;

//struct MachineParameter: public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(QString bedActualTemperature MEMBER bedActualTemperature NOTIFY bedActualTemperature_Change);
//    Q_PROPERTY(QString bedTargetTemperature MEMBER bedTargetTemperature NOTIFY bedTargetTemperature_Change);
//    Q_PROPERTY(QString tool0ActualTemperature MEMBER tool0ActualTemperature NOTIFY tool0ActualTemperature_Change);
//    Q_PROPERTY(QString tool0TargetTemperature MEMBER tool0TargetTemperature NOTIFY tool0TargetTemperature_Change);
//    Q_PROPERTY(QString currentZ MEMBER currentZ NOTIFY currentZ_Change);
//private:
//    QString bedActualTemperature = "0.0";   //热床实时温度
//    QString bedTargetTemperature = "0.0";   //热床目标温度
//    QString tool0ActualTemperature = "0.0"; //喷头实时温度
//    QString tool0TargetTemperature = "0.0"; //喷头目标温度
//    QString currentZ = "0";               //Z轴高度
//signals:
//    void bedActualTemperature_Change();
//    void bedTargetTemperature_Change();
//    void tool0ActualTemperature_Change();
//    void tool0TargetTemperature_Change();
//    void currentZ_Change();
//};

//struct PrintFile: public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(QString fileName MEMBER fileName NOTIFY fileName_Change);
//    Q_PROPERTY(QString filePath MEMBER filePath NOTIFY filePath_Change);
//    Q_PROPERTY(QString filePrintTime MEMBER filePrintTime NOTIFY filePrintTime_Change);
//    Q_PROPERTY(QString fileCompletion MEMBER fileCompletion NOTIFY fileCompletion_Change);
//private:
//    QString fileName;               //文件名
//    QString filePath;               //文件路径
//    QString filePrintTime;          //打印时长
//    QString fileCompletion;         //打印进度
//signals:
//    void fileName_Change();
//    void filePath_Change();
//    void filePrintTime_Change();
//    void fileCompletion_Change();
//};

//struct ConnectionStateInfo: public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(QString state MEMBER state NOTIFY state_Change);
//    Q_PROPERTY(bool cancelling MEMBER cancelling NOTIFY cancelling_Change);
//    Q_PROPERTY(bool pausing MEMBER pausing NOTIFY pausing_Change);
//    Q_PROPERTY(bool operational MEMBER operational NOTIFY operational_Change);

//    Q_PROPERTY(bool paused MEMBER paused NOTIFY paused_Change);
//    Q_PROPERTY(bool printing MEMBER printing NOTIFY printing_Change);
//    Q_PROPERTY(bool resuming MEMBER resuming NOTIFY resuming_Change);
//    Q_PROPERTY(bool sdReady MEMBER sdReady NOTIFY sdReady_Change);

//    Q_PROPERTY(bool error MEMBER error NOTIFY error_Change);
//    Q_PROPERTY(bool ready MEMBER ready NOTIFY ready_Change);
//    Q_PROPERTY(bool finishing MEMBER finishing NOTIFY finishing_Change);
//    Q_PROPERTY(bool closedOrError MEMBER closedOrError NOTIFY closedOrError_Change);

//private:
//    QString state;      //connection状态
//    bool cancelling;
//    bool pausing;
//    bool operational;

//    bool paused;
//    bool printing;
//    bool resuming;
//    bool sdReady;

//    bool error;
//    bool ready;
//    bool finishing;
//    bool closedOrError;

//signals:
//    void state_Change();
//    void cancelling_Change();
//    void pausing_Change();
//    void operational_Change();

//    void paused_Change();
//    void printing_Change();
//    void resuming_Change();
//    void sdReady_Change();

//    void error_Change();
//    void ready_Change();
//    void finishing_Change();
//    void closedOrError_Change();
//};

//Q_DECLARE_METATYPE(ConnectionStateInfo);
//Q_DECLARE_METATYPE(MachineParameter);
//Q_DECLARE_METATYPE(PrintFile);


class WebSocketModule : public QObject
{
    Q_OBJECT

//    Q_PROPERTY(int historyLog_maxcount MEMBER historyLog_maxcount NOTIFY historyLog_maxcount_Change);
public:
    explicit WebSocketModule(QObject *parent = nullptr);
    ~WebSocketModule();
    Q_INVOKABLE void webSocketBegin();

    //webSoket_Msg解析
    Q_INVOKABLE void webSocketMsgAnalysis(QString msg);
    //webSoket 日志重组
    Q_INVOKABLE QString historyLog(QString msg);
signals:
//    void sentMsgtoOut(QString msg);//完整消息输出
//    void sentSearchLinkMsg(QString msg);//单次消息日志输出
    void signal_error(QString error);
    //传递打印文件信息
//    void printFileInfo(PrintFile* tempStruct);
    //传递机器参数
//    void machineParameterInfo(MachineParameter* tempStruct);
    //传递连接状态信息
//    void connectionStateInfo(ConnectionStateInfo* tempStruct);
    //RefreshFileInfo刷新文件信息
    void refreshFileInfoEvent(QString json);
    void settingsUpdatedEvent(QString json);
    void wsEvent(QString json);
    void wsCurrentInfo(QString json);
//    void historyLog_maxcount_Change();
    void invalidSession();
    void checkIPResult(bool result);
private slots:
    void onWSDisconnected();
    void checkCookies();
    void checkIP();
    void onWSConnected();
    void onWSTextReceived(QString msg);
    void onWSReconnect();
    void slot_pingTimeout();
    void slot_pongTimeout();
    void slot_error(QAbstractSocket::SocketError error);
private:
    QTimer* _ptimer = new QTimer();
    QTimer* textReceivedTimer;
    QTimer* ping_pongTimer;
    QTimer* checkNetwork_Timer = new QTimer();
//    QTimer* checkCookies_Timeout = new QTimer();
    QStringList historyLog_stringlist;
    int historyLog_maxcount = 1000;//最大历史数据长度
private:
    QJsonValue SearchWSJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj);
    QList<QJsonValue> SearchWSJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj);
    //文本信息发送
    void sendTextMessage(const QString &message);
    void sendPermissionsMsg();
public:
    QWebSocket* _pdataRecvWS = new QWebSocket();
//    ConnectionStateInfo  connectionStateInfoStruct;
//    PrintFile printFileInfoStruct;
//    MachineParameter machineParameterStruct;
};

#endif // WebSocketModule_H
