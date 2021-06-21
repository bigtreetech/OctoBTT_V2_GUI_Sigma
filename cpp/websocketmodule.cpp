#include "../headers/websocketmodule.h"

WebSocketModule::WebSocketModule(QObject *parent) : QObject(parent)
{
    textReceivedTimer = new QTimer(this);
    textReceivedTimer->setSingleShot(true);

//    _ptimer = new QTimer();
    checkNetwork_Timer->setInterval(2000);
    checkNetwork_Timer->setSingleShot(true);

    ping_pongTimer = new QTimer(this);
    ping_pongTimer->setInterval(3000);
    ping_pongTimer->setSingleShot(true);


    connect(_pdataRecvWS, SIGNAL(disconnected()), this, SLOT(onWSDisconnected()), Qt::AutoConnection);
    connect(_pdataRecvWS, SIGNAL(textMessageReceived(QString)), this, SLOT(onWSTextReceived(QString)), Qt::AutoConnection);
    connect(_pdataRecvWS, SIGNAL(connected()), this, SLOT(onWSConnected()), Qt::AutoConnection);
    connect(_ptimer, SIGNAL(timeout()), this, SLOT(onWSReconnect()), Qt::AutoConnection);
    connect(globalAccess, SIGNAL(mainIPChange()), this, SLOT(onWSReconnect()), Qt::AutoConnection);
}

WebSocketModule::~WebSocketModule()
{
    disconnect(_ptimer, SIGNAL(timeout()), this, SLOT(onWSReconnect()));
    disconnect(_pdataRecvWS, SIGNAL(disconnected()), this, SLOT(onWSDisconnected()));
    checkNetwork_Timer->disconnect(SIGNAL(timeout()));
    disconnect(globalAccess, SIGNAL(mainIPChange()), this, SLOT(onWSReconnect()));
    _pdataRecvWS->abort();
}

void WebSocketModule::webSocketBegin()
{
    disconnect(_pdataRecvWS, SIGNAL(disconnected()), this, SLOT(onWSDisconnected()));
    disconnect(_ptimer, SIGNAL(timeout()), this, SLOT(onWSReconnect()));
    _pdataRecvWS->abort();
    connect(_pdataRecvWS, SIGNAL(disconnected()), this, SLOT(onWSDisconnected()), Qt::AutoConnection);
    connect(_ptimer, SIGNAL(timeout()), this, SLOT(onWSReconnect()), Qt::AutoConnection);
    connect(_pdataRecvWS, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slot_error(QAbstractSocket::SocketError)));
    connect(_pdataRecvWS, &QWebSocket::pong,this,[=](quint64 elapsedTime,const QByteArray &payload)
    {
        ping_pongTimer->stop();
        Q_UNUSED(payload)
        Q_UNUSED(elapsedTime)
//        qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy-HH-mm hh:MM:ss:zzz") << "pong: " << elapsedTime;
        if(!ping_pongTimer->isActive())
        {
            ping_pongTimer->setInterval(3000);
            disconnect(ping_pongTimer, &QTimer::timeout, this, &WebSocketModule::slot_pongTimeout);
            connect(ping_pongTimer, &QTimer::timeout, this, &WebSocketModule::slot_pingTimeout);
            ping_pongTimer->start();
        }
    });
//    qDebug() << __FILE__ << __LINE__ <<"ws://" + globalAccess->property("mainIP").toString() + "/sockjs/websocket";
    checkIP();
//    _pdataRecvWS->open(QUrl("ws://" + globalAccess->property("mainIP").toString() + "/sockjs/websocket"));
}

void WebSocketModule::webSocketMsgAnalysis(QString msg)
{
    QJsonDocument loadDoc(QJsonDocument::fromJson(msg.toUtf8()));
    QJsonObject QObj = loadDoc.object();

    /// 信号处理单元
    if(MessageParser::searchJsonKey({"event"},QObj))
    {
        emit wsEvent(msg);
        if(MessageParser::searchJsonKey({"event","payload","printables","type"},QObj))
        {
            if(MessageParser::searchJsonValue({"event","payload","printables","type"},QObj).toString() == "printables")
                emit refreshFileInfoEvent(msg);
        }
        if(MessageParser::searchJsonKey({"event","payload","state_string"},QObj))
        {
            QString var = MessageParser::searchJsonValue({"event","payload","state_string"},QObj).toString();
            if(var == "Error: Heating failed, system stopped! Heater_ID: bed - Printer halted. kill() called!")
                connectionStateInfoStruct.setProperty("state" , "Error: Print terminated");
            else if(var == "Printing")
                connectionStateInfoStruct.setProperty("state" , "Printing : " + printFileInfoStruct.property("fileName").toString());
            else
                connectionStateInfoStruct.setProperty("state" , var);
        }
        if(MessageParser::searchJsonKey({"event","type"},QObj))
        {
            if(MessageParser::searchJsonValue({"event","type"},QObj).toString() == "SettingsUpdated")
                emit settingsUpdatedEvent(msg);
        }
    }
    /// current 信息采集单元
    if(MessageParser::searchJsonKey({"current"},QObj))
    {
        emit wsCurrentInfo(msg);
        // state sequence
        QList<QString> state_sequence_List = {"cancelling","pausing","operational","paused","printing","resuming","sdReady","error","ready","finishing","closedOrError"};
        foreach(QString var,state_sequence_List)
        {
            connectionStateInfoStruct.setProperty(var.toLatin1().data() , MessageParser::searchJsonValue({"current","state","flags",var},QObj).toBool());
        }
        // file system
        printFileInfoStruct.setProperty("fileName" , MessageParser::searchJsonValue({"current","job","file","display"},QObj).isNull() ? "" : MessageParser::searchJsonValue({"current","job","file","display"},QObj).toString());
        printFileInfoStruct.setProperty("filePath" , MessageParser::searchJsonValue({"current","job","file","path"},QObj).isNull() ? "" : MessageParser::searchJsonValue({"current","job","file","path"},QObj).toString());
        // progress
        printFileInfoStruct.setProperty("fileCompletion" ,  MessageParser::searchJsonValue({"current","progress","completion"},QObj).isNull() ? "" : QString("%1%").arg(QString::number(MessageParser::searchJsonValue({"current","progress","completion"},QObj).toDouble(), 'd', 2)));

        if(MessageParser::searchJsonKey({"current","progress","printTime"},QObj))
        {
            QJsonValue printTimeValue = MessageParser::searchJsonValue({"current","progress","printTime"},QObj);
            if(!printTimeValue.isNull())
            {
                int second = printTimeValue.toInt();
                QString tempDay = second / (3600 * 24) > 0 ? QString::number(second / (3600 * 24)) + tr("day") + " " : "";
                second = second % (3600 * 24);
                QString tempHour = second / 3600 > 0 ? QString("1%").arg(QString::number(second / 3600), 2, QChar('0')) + ":" : "";
                second = second % 3600;
                QString tempMinute = !tempHour.isEmpty() && second / 60 > 0 ? QString("1%").arg(QString::number(second / 60), 2, QChar('0'))  + ":" : "";
                QString tempSecond = QString::number(second % 60) + (tempHour.isEmpty() && tempMinute.isEmpty() ? tr("seconds") : "");
                printFileInfoStruct.setProperty("filePrintTime" , QString("%1%2%3%4").
                                                arg(tempDay).
                                                arg(tempHour).
                                                arg(tempMinute).
                                                arg(tempSecond));
            }
            else
                printFileInfoStruct.setProperty("filePrintTime" , "");
        }

        if(MessageParser::searchJsonKey({"current","temps"},QObj))
        {
        }
    }

    if(QObj.contains("current"))
    {
        QJsonValue currentValue = QObj.value("current");
        if(currentValue.isObject())
        {
            QJsonObject currentObject = currentValue.toObject();
            if(currentObject.contains("temps"))
            {
                QJsonValue tempsValue = currentObject.value("temps");
                if(tempsValue.isArray())
                {
                    QJsonArray tempsArray = tempsValue.toArray();
                    QJsonValue temp0Value = tempsArray.at(0);
                    QJsonObject temp0Objcet = temp0Value.toObject();
                    if(temp0Objcet.contains("bed"))
                    {
                        if(temp0Objcet.contains("bed"))
                        {
                            QJsonValue bedValue = temp0Objcet.value("bed");
                            if(bedValue.isObject())
                            {
                                QJsonObject bedObject = bedValue.toObject();
                                if(bedObject.contains("actual"))
                                {
                                    machineParameterStruct.setProperty("bedActualTemperature" , QString("%1").arg(bedObject.value("actual").toDouble()));
                                }
                                if(bedObject.contains("target"))
                                {
                                    machineParameterStruct.setProperty("bedTargetTemperature" , QString("%1").arg(bedObject.value("target").toDouble()));
                                }
                            }
                        }
                        if(temp0Objcet.contains("tool0"))
                        {
                            QJsonValue bedValue = temp0Objcet.value("tool0");
                            if(bedValue.isObject())
                            {
                                QJsonObject bedObject = bedValue.toObject();
                                if(bedObject.contains("actual"))
                                {
                                    machineParameterStruct.setProperty("tool0ActualTemperature" , QString("%1").arg(bedObject.value("actual").toDouble()));
                                }
                                if(bedObject.contains("target"))
                                {
                                    machineParameterStruct.setProperty("tool0TargetTemperature" , QString("%1").arg(bedObject.value("target").toDouble()));
                                }
                            }
                        }
                    }
                }
            }
            if(currentObject.contains("currentZ"))
            {
                QJsonValue currentZValue = currentObject.value("currentZ");
                machineParameterStruct.setProperty("currentZ" , QString("%1").arg(currentZValue.toDouble()));
            }

        }
    }
    emit printFileInfo(&printFileInfoStruct);
    emit machineParameterInfo(&machineParameterStruct);
    emit connectionStateInfo(&connectionStateInfoStruct);
}

QString WebSocketModule::historyLog(QString msg)
{
    historyLog_stringlist.append(msg);
    if(historyLog_stringlist.count() > historyLog_maxcount)//限制数据长度
        historyLog_stringlist.removeFirst();
    return historyLog_stringlist.join("\n");
}

//断开连接会触发这个槽函数
void WebSocketModule::onWSDisconnected()
{
    //    _ptimer->start();
    //    ping_pongTimer->stop();
    //    onWSReconnect();//20210530:断开连接触发自动重新连接
}
//连接成功会触发这个槽函数
void WebSocketModule::onWSConnected()
{
//    qDebug() << __FILE__ << __LINE__ << "onWSConnected";
    //    _ptimer->stop();
}
//收到服务发来的消息会触发这个槽函数
void WebSocketModule::onWSTextReceived(QString msg)
{
    ping_pongTimer->setInterval(3000);
    disconnect(ping_pongTimer, &QTimer::timeout, this, &WebSocketModule::slot_pingTimeout);
    if(!disconnect(ping_pongTimer, &QTimer::timeout, this, &WebSocketModule::slot_pingTimeout))
    {
        connect(ping_pongTimer, &QTimer::timeout, this, &WebSocketModule::slot_pingTimeout);
    }

    disconnect(ping_pongTimer, &QTimer::timeout, this, &WebSocketModule::slot_pongTimeout);

    emit sentMsgtoOut(msg);
    WebSocketModule::webSocketMsgAnalysis(msg);
    QJsonDocument loadDoc(QJsonDocument::fromJson(msg.toUtf8()));
    QJsonObject QObj = loadDoc.object();
    QJsonObject QObj_1 = loadDoc.object();
    if(!textReceivedTimer->isActive() && QObj.contains("connected"))
    {
        QJsonValue value  = QObj.value("connected");
        if(value.isObject())
        {
            QJsonObject QObj = value.toObject();
            if(QObj.contains("permissions"))
            {
                sendPermissionsMsg();
//                sendTextMessage(QString("{\"auth\":\"%1:%2\"}").arg(globalAccess->userinfo->property("onlineUid").toString(), QString(globalAccess->userinfo->session)));
//                disconnect(this, SIGNAL(sentMsg(QString)), this ,SLOT(slot_sendTextMessage(QString)));
            }
        }
    }

    ping_pongTimer->start();

    if(!msg.trimmed().startsWith("{\"current\":") || msg.trimmed().length() == 0)
        return;
    QList<QString> SearchLink ;
    SearchLink << "current" << "logs";
    foreach(QJsonValue _value, SearchWSJsonValue(SearchLink , QObj_1).toArray())
    {
        emit sentSearchLinkMsg(_value.toString());
    }
}

void WebSocketModule::sendPermissionsMsg()
{
//    qDebug() << __FILE__ << __LINE__ << QString("{\"auth\":\"%1:%2\"}").arg(globalAccess->userinfo->property("onlineUid").toString(), QString(globalAccess->userinfo->session));
    sendTextMessage(QString("{\"auth\":\"%1:%2\"}").arg(globalAccess->userinfo->property("onlineUid").toString(), QString(globalAccess->userinfo->session)));
}

void WebSocketModule::onWSReconnect()
{
//    qDebug() << __FILE__ << __LINE__ <<"ws://" + globalAccess->property("mainIP").toString() + "/sockjs/websocket";
    if(_pdataRecvWS->state() != QAbstractSocket::UnconnectedState)
        _pdataRecvWS->abort();
    _pdataRecvWS->open(QUrl("ws://" + globalAccess->property("mainIP").toString() + "/sockjs/websocket"));
}

void WebSocketModule::slot_pingTimeout()
{
    ping_pongTimer->setInterval(5000);
    disconnect(ping_pongTimer, &QTimer::timeout, this, &WebSocketModule::slot_pingTimeout);
    connect(ping_pongTimer, &QTimer::timeout, this, &WebSocketModule::slot_pongTimeout);
//    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy-HH-mm hh:MM:ss:zzz") << "ping";
    ping_pongTimer->start();
    sendPermissionsMsg();
    if(globalAccess->userinfo->property("admin").toBool() == false)
        emit checkIPResult(true);
    _pdataRecvWS->ping();
}

void WebSocketModule::slot_pongTimeout()
{
    emit _pdataRecvWS->error(QAbstractSocket::SocketError(QAbstractSocket::RemoteHostClosedError));
}

void WebSocketModule::slot_error(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    ping_pongTimer->stop();
//    qDebug() << __FILE__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy-HH-mm hh:MM:ss:zzz") << int(error) << _pdataRecvWS->errorString();
    emit signal_error(_pdataRecvWS->errorString());
    _pdataRecvWS->close();
    checkIP();

}

void WebSocketModule::checkIP()
{
    //切断历史槽信号
    checkNetwork_Timer->disconnect(SIGNAL(timeout()));
    //触发IP检测机制
    Terminal* pinger = new Terminal();
    QObject::connect(pinger,&Terminal::CMD_Reply,this,[=](QStringList value)
    {
//        qDebug() << __FILE__ << __LINE__ << value;
//        qDebug() << __FILE__ << __LINE__ << "IP Check: " +  QString(value.count() > 0 &&  value.last().contains(QRegExp(".*time=.*ms")));
        if(value.count() > 0 && value.last().contains(QRegExp(".*time=.*ms")))
        {
            if(globalAccess->userinfo->property("admin").toBool())
                checkCookies();//IP有效，检测对话ID是否有效
            else
            {
                emit checkIPResult(true);
                connect(checkNetwork_Timer, SIGNAL(timeout()), this, SLOT(checkIP()));//接入重复检测IP信号
                checkNetwork_Timer->start();
            }
        }
        else
        {
            emit checkIPResult(false);
            connect(checkNetwork_Timer, SIGNAL(timeout()), this, SLOT(checkIP()));//接入重复检测IP信号
            checkNetwork_Timer->start();
        }
    });
    pinger->SendCMD("ping "+globalAccess->property("mainIP").toString().split(":")[0],2000);
}

void WebSocketModule::checkCookies()
{
    checkNetwork_Timer->disconnect(SIGNAL(timeout()));
    APIDisposeThread *newThread = new APIDisposeThread("get", "webJob", globalAccess->userinfo->cookies, QJsonObject(), globalAccess->MainUrl() + "job");
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        Q_UNUSED(_QObj)
//        qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState;
        if(_statusCode == 200 && _requestState)
        {
//            qDebug() << __FILE__ << __LINE__ << "Cookies有效";
            onWSReconnect();
        }
        else
        {
//            qDebug() << __FILE__ << __LINE__ << "Cookies失效";
            if(!globalAccess->userinfo->property("admin").toBool())
                emit invalidSession();
            connect(checkNetwork_Timer, SIGNAL(timeout()), this, SLOT(checkIP()));//接入重复检测对话ID信号
            checkNetwork_Timer->start();
        }
    });
    APIDisposeThread::runNewThread(newThread, "webJob", false);
}

void WebSocketModule::sendTextMessage(const QString &message)
{
    _pdataRecvWS->sendTextMessage(message);
    textReceivedTimer->start(5000);
}

QJsonValue WebSocketModule::SearchWSJsonValue(QList<QString> Find_List_Name, QJsonObject _QObj)
{
    QJsonValue returnValue;
    QList<QString> SearchLink ;
    SearchLink << Find_List_Name;
    if(SearchLink.count() > 1)
    {
        QString MotherKey = SearchLink[0]; //current
        SearchLink.removeFirst();   //移除current
        //找current中是否有*
        if(MotherKey.indexOf("*") >= 0)//wildcard
        {
            QJsonArray ArrayValue;
            for(int i = 0 ; i <= 9 ; i++)
            {
                QString TempMotherKey = MotherKey;
                if(_QObj.find(TempMotherKey.replace("*",QString::number(i))).value().isUndefined())continue;
                //if(QSysInfo::productType() != "raspbian"){qDebug()<<_QObj.find(TempMotherKey.replace("*",QString::number(i))).value().isUndefined();};
                QJsonValue data = SearchWSJsonValue(SearchLink , _QObj.constFind(TempMotherKey.replace("*",QString::number(i))).value().toObject());
                ArrayValue.append(data);
            }
            returnValue = ArrayValue;
        }
        else
            returnValue = SearchWSJsonValue(SearchLink, _QObj.constFind(MotherKey).value().toObject());
    }
    else
    {
        if(SearchLink[0].indexOf("*") >= 0)//wildcard
        {

        }
        else
            returnValue = _QObj.constFind(SearchLink[0]).value();
    }
    return returnValue;
}

QList<QJsonValue> WebSocketModule::SearchWSJsonValue(QList<QList<QString> > Find_List_Names, QJsonObject _QObj)
{
    QList<QJsonValue> _returnValue ;
    foreach(QList<QString> item , Find_List_Names)
    {
        _returnValue.append(SearchWSJsonValue(item,_QObj));
    }
    return _returnValue;
}
