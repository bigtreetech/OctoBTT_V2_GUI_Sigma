#include "../headers/connectiondetection.h"

ConnectionDetection::ConnectionDetection(QObject *parent) : QObject(parent)
{

}

void ConnectionDetection::getConnectionInformation()
{
    APIDisposeThread *newThread = new APIDisposeThread("get", "connection", globalAccess->userinfo->cookies, QJsonObject(), globalAccess->MainUrl() + "connection");
    APIDisposeThread::runNewThread(newThread, "connection", false);

    //接收线程返回的信息
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        Q_UNUSED(_statusCode)
        Q_UNUSED(_requestState)
        if(_QObj.contains("current"))
        {
            QJsonValue currentValue = _QObj.value("current");
            if(currentValue.isObject())
            {
                QJsonObject currentObject = currentValue.toObject();
                if(currentObject.contains("baudrate"))
                {
                    QJsonValue baudrateValue = currentObject.value("baudrate");
                    if(baudrateValue.isDouble())
                    {
                        connectionInfoStruct.baudrate = baudrateValue.toDouble();
                    }
                }
                if(currentObject.contains("port"))
                {
                    QJsonValue portValue = currentObject.value("port");
                    if(portValue.isString())
                    {
                        connectionInfoStruct.port = portValue.toString();
                    }
                }
            }
        }
        if(_QObj.contains("options"))
        {
            QJsonValue optionsValue = _QObj.value("options");
            if(optionsValue.isObject())
            {
                QJsonObject optionsObject = optionsValue.toObject();
                if(optionsObject.contains("ports"))
                {
                    connectionInfoStruct.ports.clear();
                    for(int i = 0; i < optionsObject.value("ports").toArray().count(); i++)
                    {
                        connectionInfoStruct.ports.append(optionsObject.value("ports").toArray().at(i).toString());
                    }
                }
                if(optionsObject.contains("baudrates"))
                {
                    connectionInfoStruct.baudrates.clear();
                    for(int i = 0; i < optionsObject.value("baudrates").toArray().count(); i++)
                    {
                        connectionInfoStruct.baudrates.append(QString::number(optionsObject.value("baudrates").toArray().at(i).toDouble()));
                    }
                }
            }
        }
        emit ConnectionInfoSignal(connectionInfoStruct);
    });
}

void ConnectionDetection::postConnectionInformation(QString port, QString baudrate)
{
    double baudrateToDouble = baudrate.toDouble();
    QJsonObject connectionObjcet;
    connectionObjcet.insert("command", "connect");
    connectionObjcet.insert("port", port);
    connectionObjcet.insert("baudrate", baudrateToDouble);
    connectionObjcet.insert("printerProfile", "_default");
    connectionObjcet.insert("save", true);
    connectionObjcet.insert("autoconnect", true);

    QString tempType = "connection";

    APIDisposeThread *newThread = new APIDisposeThread("post_0", tempType, globalAccess->userinfo->cookies, connectionObjcet, globalAccess->MainUrl()+tempType);
    APIDisposeThread::runNewThread(newThread, tempType, false);
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState << _QObj;
    });
}
