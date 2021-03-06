#include "../headers/disposethreadwithapi.h"

QHash<APIDisposeThread *, QString> APIDisposeThread::threadKey;

QHash<QString, QMap<int, APIDisposeThread *>> APIDisposeThread::queueThreadKey;

APIDisposeThread::APIDisposeThread(QObject *parent) :
    QThread(parent)
{
    stopped = false;
}

APIDisposeThread::APIDisposeThread(QString runMode, QString UrlType, QList<QNetworkCookie> allcookies, QJsonObject TemperatureJsonObj, QString newUrl,QJsonObject Params)
{
    _runMode =  runMode;
    _UrlType =  UrlType;
    _TemperatureJsonObj =  TemperatureJsonObj;
    _newUrl = newUrl;
    if(Params.count() > 0)
    {
        QStringList ParamList;
        foreach(QString key , Params.keys())
        {
            QString Paramvalue;
            if(Params.value(key).isBool())
                Paramvalue = Params.value(key).toBool() ? "true" : "false";
            else if(Params.value(key).isNull())
                Paramvalue = "null";
            else if(Params.value(key).isDouble())
                Paramvalue = QString::number(Params.value(key).toDouble(),'f');
            else if(Params.value(key).isString())
                Paramvalue = Params.value(key).toString();
            else
                Paramvalue = Params.value(key).toString();
            ParamList.append(QString("%1=%2").arg(key).arg(Paramvalue));
        }
        _newUrl = QString("%1?%2").arg(_newUrl).arg(ParamList.join("&"));
    }
    _allcookies = allcookies;
    stopped = false;
    choosePostMode = 0;
}

//APIDisposeThread::APIDisposeThread(QString runMode, QString UrlType, QList<QNetworkCookie> allcookies, QString FilePath, QString newUrl)
//{
//    _runMode =  runMode;
//    _UrlType =  UrlType;
//    _FilePath =  FilePath;
//    _newUrl = newUrl;
//    _allcookies = allcookies;
//    stopped = false;
//    choosePostMode = 1;
//}

APIDisposeThread::APIDisposeThread(QString runMode, QString UrlType, QList<QNetworkCookie> allcookies, QIODevice *data, QString newUrl,QJsonObject Params)
{
    _runMode =  runMode;
    _UrlType =  UrlType;
    _data =  data;
    _newUrl = newUrl;
    if(Params.count() > 0)
    {
        QStringList ParamList;
        foreach(QString key , Params.keys())
        {
            QString Paramvalue;
            if(Params.value(key).isBool())
                Paramvalue = Params.value(key).toBool() ? "true" : "false";
            else if(Params.value(key).isNull())
                Paramvalue = "null";
            else if(Params.value(key).isDouble())
                Paramvalue = QString::number(Params.value(key).toDouble(),'f');
            else if(Params.value(key).isString())
                Paramvalue = Params.value(key).toString();
            else
                Paramvalue = Params.value(key).toString();
            ParamList.append(QString("%1=%2").arg(key).arg(Paramvalue));
        }
        _newUrl = QString("%1?%2").arg(_newUrl).arg(ParamList.join("&"));
    }
    _allcookies = allcookies;
    stopped = false;
    choosePostMode = 2;
}

APIDisposeThread::APIDisposeThread(QString runMode, QString UrlType, QString folderPath, QString newFolder, QList<QNetworkCookie> allcookies, QString newUrl,QJsonObject Params)
{
    _runMode =  runMode;
    _UrlType =  UrlType;
    _newUrl = newUrl;
    if(Params.count() > 0)
    {
        QStringList ParamList;
        foreach(QString key , Params.keys())
        {
            QString Paramvalue;
            if(Params.value(key).isBool())
                Paramvalue = Params.value(key).toBool() ? "true" : "false";
            else if(Params.value(key).isNull())
                Paramvalue = "null";
            else if(Params.value(key).isDouble())
                Paramvalue = QString::number(Params.value(key).toDouble(),'f');
            else if(Params.value(key).isString())
                Paramvalue = Params.value(key).toString();
            else
                Paramvalue = Params.value(key).toString();
            ParamList.append(QString("%1=%2").arg(key).arg(Paramvalue));
        }
        _newUrl = QString("%1?%2").arg(_newUrl).arg(ParamList.join("&"));
    }
    _allcookies = allcookies;
    stopped = false;
    _folderPath = folderPath;
    _newFolder = newFolder;
}

APIDisposeThread::APIDisposeThread(QString runMode, QString UrlType, QList<QNetworkCookie> allcookies, QString folderUploadPath, QString uploadFilePath, QString newUrl,QJsonObject Params)
{
    _runMode =  runMode;
    _UrlType =  UrlType;
    _newUrl = newUrl;
    if(Params.count() > 0)
    {
        QStringList ParamList;
        foreach(QString key , Params.keys())
        {
            QString Paramvalue;
            if(Params.value(key).isBool())
                Paramvalue = Params.value(key).toBool() ? "true" : "false";
            else if(Params.value(key).isNull())
                Paramvalue = "null";
            else if(Params.value(key).isDouble())
                Paramvalue = QString::number(Params.value(key).toDouble(),'f');
            else if(Params.value(key).isString())
                Paramvalue = Params.value(key).toString();
            else
                Paramvalue = Params.value(key).toString();
            ParamList.append(QString("%1=%2").arg(key).arg(Paramvalue));
        }
        _newUrl = QString("%1?%2").arg(_newUrl).arg(ParamList.join("&"));
    }
    _allcookies = allcookies;
    stopped = false;
    _folderUploadPath = folderUploadPath;
    _uploadFilePath = uploadFilePath;
    choosePostMode = 1;
}

APIDisposeThread::~APIDisposeThread()
{
    if(isRunning()) stop();
}

void APIDisposeThread::stop()
{
    stopped = true;
}

//????????????????????????
void APIDisposeThread::run()
{
    //??????????????????
    if(stopped)
    {
        stopped = false;
        return;
    }

    if(_runMode == QString("post_0") || _runMode == QString("post_1"))
    {
        if(choosePostMode == 0)
            api_post_jsonObject(_runMode, _UrlType, _TemperatureJsonObj);
        else if(choosePostMode == 1)
        {
            api_post_filePath(_UrlType, _folderUploadPath, _uploadFilePath);
        }
        else
        {
            api_post_device(_UrlType, _data);
        }
    }
    else if(_runMode == QString("post_2"))
        api_post_createfilePath(_UrlType, _folderPath, _newFolder);
    else if(_runMode == QString("get"))
        api_get(_UrlType);
    else if(_runMode == QString("get_1"))
        api_getFile(_UrlType);
    else if(_runMode == QString("deleteResource"))
        api_delResource(QUrl(_newUrl));

    //????????????
    //    while(!stopped)
    //    {
    //        //        qDebug() << QThread::currentThreadId() << ": " << QString("in APIDisposeThread: %1").arg(threadCount);
    //        msleep(10);
    //        threadCount++;
    //        if(threadCount == 10)
    //        {
//    qDebug() << QThread::currentThreadId() << ": is over!" << __LINE__;
    //            return;
    //        }
    //    }
    //??????, ??????????????????????????????network?????????????????????
    msleep(10);
}

void APIDisposeThread::stopAllThread()
{
    //????????????saverThread???????????????
    if(APIDisposeThread::threadKey.count() != 0)
    {
        foreach(APIDisposeThread * oneThread, APIDisposeThread::threadKey.keys())
        {
            if(oneThread->isRunning())
            {
                oneThread->stop();
            }
            APIDisposeThread::threadKey.remove(oneThread);
        }
        APIDisposeThread::threadKey.clear();
    }
}

void APIDisposeThread::stopAllQueueThread()
{
    if(APIDisposeThread::queueThreadKey.count() != 0)
    {
        foreach(QString tempUrlType, APIDisposeThread::queueThreadKey.keys())
        {
            if(APIDisposeThread::queueThreadKey[tempUrlType].count() == 0)
            {
                return;
            }
            APIDisposeThread *tmep = APIDisposeThread::queueThreadKey[tempUrlType].begin().value();
            APIDisposeThread::queueThreadKey[tempUrlType].clear();
            //?????????????????????????????????
            if(tmep->isRunning())
                tmep->stop();

            //??????????????????????????????UrlType??????
            APIDisposeThread::queueThreadKey.remove(tempUrlType);
        }
        APIDisposeThread::queueThreadKey.clear();
    }
}

void APIDisposeThread::stopOneTypeThread(QString UrlType)
{
    if(APIDisposeThread::threadKey.count() != 0)
    {
        //????????????saverThread??????UrlType?????????
        foreach(APIDisposeThread * oneThread, APIDisposeThread::threadKey.keys())
        {
            if(APIDisposeThread::threadKey.value(oneThread) == UrlType)
                if(oneThread->isRunning())
                    oneThread->stop();

            APIDisposeThread::threadKey.remove(oneThread);
        }
    }
}

void APIDisposeThread::stopOneTypeQueueThread(QString UrlType)
{
    if(APIDisposeThread::queueThreadKey.count() != 0)
    {
        foreach(QString tempUrlType, APIDisposeThread::queueThreadKey.keys())
        {
            //???????????????????????????????????????UrlType?????????QMap?????????????????????
            if(tempUrlType == UrlType)
            {
                APIDisposeThread *tmep = APIDisposeThread::queueThreadKey[tempUrlType].begin().value();
                //?????????????????????????????????
                APIDisposeThread::queueThreadKey[tempUrlType].clear();
                if(tmep->isRunning())
                    tmep->stop();

                //??????????????????????????????UrlType??????
                APIDisposeThread::queueThreadKey.remove(tempUrlType);
            }
        }
    }
}

void APIDisposeThread::queueThread()
{
    //????????????????????????????????????????????????API?????????, ?????????????????????
    QFuture<void> queueThread;
    APIDisposeThread queueRunThread;
    //?????????????????????, ????????????????????????????????????????????????
    queueThread = QtConcurrent::run(&queueRunThread, &APIDisposeThread::runNewQueueThread);
}

void APIDisposeThread::runNewQueueThread()
{
    if(APIDisposeThread::queueThreadKey.count() != 0)
    {
        foreach(QString tempUrlType, APIDisposeThread::queueThreadKey.keys())
        {
            //????????????????????????????????????????????????????????????????????????, ?????????????????????
            if(APIDisposeThread::queueThreadKey[tempUrlType].count() != 0)
            {
                if(!APIDisposeThread::queueThreadKey.value(tempUrlType).begin().value()->isRunning())
                {
                    APIDisposeThread *tempThread = APIDisposeThread::queueThreadKey.value(tempUrlType).begin().value();

                    connect(tempThread, &QThread::finished, tempThread, &QObject::deleteLater);

                    //???????????????, ?????????????????????????????????
                    connect(tempThread, &QThread::finished, tempThread, [=](){
                        if(APIDisposeThread::queueThreadKey[tempUrlType].count() != 0)
                        {
                            APIDisposeThread::queueThreadKey[tempUrlType].remove(APIDisposeThread::queueThreadKey.value(tempUrlType).firstKey());
                            queueThread();
                        }
                    });
                    tempThread->start();
                }
            }
        }
    }
}

void APIDisposeThread::runNewThread(APIDisposeThread *newThread, QString UrlType, bool queueOrNo)
{
    bool sameThreadFlag = false;
    //??????API(?????????????????????????????????????????????)
    if(queueOrNo == true)
    {
        //==========================================================================??????????????????
        //?????????????????????????????????????????????????????????????????????UrlType, ???????????????????????????????????????
        if(APIDisposeThread::queueThreadKey.value(UrlType).count() == 0)
        {
            QMap<int, APIDisposeThread *> tempMap;
            tempMap.insert(0, newThread);
            APIDisposeThread::queueThreadKey.insert(UrlType, tempMap);
            queueThread();
        }
        //??????????????????????????????????????????
        else
        {
            foreach(QString tempUrlType, APIDisposeThread::queueThreadKey.keys())
            {
                //????????????API???UrlType????????????????????????, ?????????????????????
                if(tempUrlType == UrlType)
                {
                    //???????????????????????????, ???????????????0?????????
                    if(APIDisposeThread::queueThreadKey.value(tempUrlType).count() == 0)
                    {
                        APIDisposeThread::queueThreadKey[tempUrlType].insert(0, newThread);
                    }
                    //????????????????????????
                    else
                    {
                        //????????????????????????????????????????????? + 1
                        APIDisposeThread::queueThreadKey[tempUrlType].insert(APIDisposeThread::queueThreadKey.value(tempUrlType).lastKey() + 1, newThread);
                    }
                }
            }
        }
    }
    //?????????API(????????????????????????????????????????????????)
    else
    {
        foreach(APIDisposeThread *oneThread, APIDisposeThread::threadKey.keys())
        {
            if(APIDisposeThread::threadKey.value(oneThread) == UrlType)
            {
                //??????????????????
                if(oneThread->isRunning())
                {
                    sameThreadFlag = true;
                    connect(oneThread, &QThread::finished, oneThread, [=](){
                        newThread->start();
                        APIDisposeThread::threadKey.insert(newThread, UrlType);
                    });
                    oneThread->stop();
                }
            }
        }

        //???????????????,????????????
        connect(newThread, &QThread::finished, newThread, [=](){
            //???????????????, ?????????????????????
            APIDisposeThread::threadKey.remove(newThread);
            newThread->deleteLater();
        });

        //????????????
        if(!sameThreadFlag)
        {
            //???????????????
            APIDisposeThread::threadKey.insert(newThread, UrlType);
            newThread->start();
        }
    }
}

void APIDisposeThread::stopThread(QString threadType)
{
    //????????????????????????????????????API?????????, ?????????????????????
    QFuture<void> clearThread;
    QFuture<void> clearQueueThread;
    APIDisposeThread stopNoQueueThread;
    APIDisposeThread stopQueueThread;
    //??????????????????????????????API?????????????????????
    if(threadType == NULL)
    {
        //?????????????????????, ????????????????????????????????????????????????
        clearThread = QtConcurrent::run(&stopNoQueueThread, &APIDisposeThread::stopAllThread);
        clearQueueThread = QtConcurrent::run(&stopQueueThread, &APIDisposeThread::stopAllQueueThread);
    }
    //???????????????????????????API??????????????????????????????
    else{
        //?????????????????????, ????????????????????????????????????????????????
        clearThread = QtConcurrent::run(&stopNoQueueThread, &APIDisposeThread::stopOneTypeThread, threadType);
        clearQueueThread = QtConcurrent::run(&stopQueueThread, &APIDisposeThread::stopOneTypeQueueThread, threadType);
    }
}
//??????post??????
void APIDisposeThread::api_post_jsonObject(QString runMode, QString UrlType, QJsonObject TemperatureJsonObj)
{
    //????????????
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    QNetworkRequest _Request;

    QVariant var;

    var.setValue(_allcookies);

    if(_newUrl != "")
        _Request.setUrl(QUrl(_newUrl) );
    else
        _Request.setUrl(QUrl(globalAccess->MainUrl() + UrlType));

    _Request.setHeader(QNetworkRequest::CookieHeader, var);
    _Request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonDocument _SenderJson;
    _SenderJson.setObject(TemperatureJsonObj);
    QJsonObject QObj;

    //???????????????
    if(runMode == QString("post_0"))
    {
        //?????????????????????????????????
        connect(networkAccessManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply)
        {
            tempReply = reply;
            int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if(tempReply->error() == QNetworkReply::NoError)
            {
                requestState = true;
            }
            QJsonDocument loadDoc(QJsonDocument::fromJson(tempReply->readAll()));
            QJsonObject QObj;
            if(loadDoc.isArray())
                QObj.insert("array",loadDoc.array());
            else
                QObj = loadDoc.object();
            emit sentRequest(statusCode, QObj, requestState);
            networkAccessManager->deleteLater();
            reply->deleteLater();
            networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
        });
        networkAccessManager->post(_Request, _SenderJson.toJson());
    }
    //???????????????
    if(runMode == QString("post_1"))
    {
        //?????????????????????????????????
        connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
        {
            tempReply = reply;
            //????????????????????????
            int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if(tempReply->error() == QNetworkReply::NoError)
            {
                QByteArray replyArray = tempReply->readAll();
                QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
                QJsonObject QObj;
                if(loadDoc.isArray())
                    QObj.insert("array",loadDoc.array());
                else
                    QObj = loadDoc.object();
                requestState = true;
                //???????????????
                emit sentRequest(statusCode, QObj, requestState);
            }
            //???????????????????????????
            networkAccessManager->deleteLater();
            reply->deleteLater();
            networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
        });
        //??????post??????
        networkAccessManager->post(_Request, _SenderJson.toJson());
    }
    //????????????,?????????????????? (QNetworkAccessManager?????????????????????, ?????????????????????????????????)
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void APIDisposeThread::api_post_filePath(QString UrlType, QString folderPath, QString FilePath)
{
    //????????????
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    QNetworkRequest _Request;

    QVariant var;
    var.setValue(_allcookies);

    if(_newUrl != "")
        _Request.setUrl(QUrl(_newUrl));
    else
        _Request.setUrl(QUrl(globalAccess->MainUrl() + UrlType));

    _Request.setHeader(QNetworkRequest::CookieHeader,var);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QFile *file = new QFile(FilePath);     //???????????????????????????????????????new
    file->open(QIODevice::ReadOnly);

    QHttpPart contentPart;
    contentPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"file\"; filename=\"%1\"").arg(QFileInfo(FilePath).fileName())));
    contentPart.setBodyDevice(file);
    file->setParent(multiPart);

    if(folderPath != "")
    {
        QHttpPart contentPart_1;
        contentPart_1.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"path\"")));
        contentPart_1.setBody(folderPath.toUtf8());
        multiPart->append(contentPart_1);
    }
    multiPart->append(contentPart);

    QJsonObject QObj;

    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(tempReply->error() == QNetworkReply::NoError)
        {
            QByteArray replyArray = tempReply->readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
            QJsonObject QObj;
            if(loadDoc.isArray())
                QObj.insert("array",loadDoc.array());
            else
                QObj = loadDoc.object();
            requestState = true;
            emit sentRequest(statusCode, QObj, requestState);
        }

        //???????????????????????????
        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });

    networkAccessManager->post(_Request, multiPart);

    //????????????,?????????????????? (QNetworkAccessManager?????????????????????, ?????????????????????????????????)
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    multiPart->deleteLater();
    file->deleteLater();
}

void APIDisposeThread::api_post_createfilePath(QString UrlType, QString folderPath, QString newFolder)
{
    //????????????
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    QNetworkRequest _Request;
    QVariant var;
    var.setValue(_allcookies);

    if(_newUrl != "")
        _Request.setUrl(QUrl(_newUrl));
    else
        _Request.setUrl(QUrl(globalAccess->MainUrl() + UrlType));

    _Request.setHeader(QNetworkRequest::CookieHeader,var);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart contentPart;
    contentPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"foldername\"")));//Content-Type=\"multipart/form-data\";
    contentPart.setBody(newFolder.toUtf8());

    if(folderPath == "")
    {
        QHttpPart contentPart_1;
        contentPart_1.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"path\"")));//Content-Type=\"multipart/form-data\";
        contentPart_1.setBody(folderPath.toUtf8());
        multiPart->append(contentPart_1);
    }

    multiPart->append(contentPart);

    QJsonObject QObj;
    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(tempReply->error() == QNetworkReply::NoError)
        {
            QByteArray replyArray = tempReply->readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
            QJsonObject QObj;
            if(loadDoc.isArray())
                QObj.insert("array",loadDoc.array());
            else
                QObj = loadDoc.object();
            requestState = true;
            emit sentRequest(statusCode, QObj, requestState);
        }

        //???????????????????????????
        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });

    networkAccessManager->post(_Request, multiPart);

    //????????????,?????????????????? (QNetworkAccessManager?????????????????????, ?????????????????????????????????)
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    multiPart->deleteLater();
}

void APIDisposeThread::api_post_device(QString UrlType, QIODevice *data)
{
    //????????????
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    QNetworkRequest _Request;

    QVariant var;
    var.setValue(_allcookies);

    if(_newUrl != "")
        _Request.setUrl(QUrl(_newUrl));
    else
        _Request.setUrl(QUrl(globalAccess->MainUrl() + UrlType));

    QJsonObject QObj;

    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(tempReply->error() == QNetworkReply::NoError)
        {
            QByteArray replyArray = tempReply->readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
            QJsonObject QObj;
            if(loadDoc.isArray())
                QObj.insert("array",loadDoc.array());
            else
                QObj = loadDoc.object();
            requestState = true;
        }
        emit sentRequest(statusCode, QObj, requestState);
        //???????????????????????????
        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });

    networkAccessManager->post(_Request, data);

    //????????????,?????????????????? (QNetworkAccessManager?????????????????????, ?????????????????????????????????)
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void APIDisposeThread::api_delResource(QUrl Url)
{
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    //??????
    QNetworkRequest _Request;
    _Request.setUrl(Url);

    QVariant var;
    var.setValue(_allcookies);
    _Request.setHeader(QNetworkRequest::CookieHeader, var);

    //?????????????????????????????????
    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QJsonObject QObj;
        if(tempReply->error() ==  QNetworkReply::NoError)
        {
            //?????????????????????
            QByteArray replyArray = tempReply->readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
            if(loadDoc.isArray())
                QObj.insert("array",loadDoc.array());
            else
                QObj = loadDoc.object();
            requestState = true;
        }
        emit sentRequest(statusCode, QObj, requestState);

        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });
    //??????get??????
    networkAccessManager->deleteResource(_Request);

    //????????????,??????????????????
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

//??????get??????
void APIDisposeThread::api_get(QString UrlType)
{
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    //??????
    QNetworkRequest _Request;
    if(_newUrl != "")
        _Request.setUrl(QUrl(_newUrl));
    else
        _Request.setUrl(QUrl(globalAccess->MainUrl() + UrlType));

    QVariant var;
    var.setValue(_allcookies);
    _Request.setHeader(QNetworkRequest::CookieHeader, var);

    //?????????????????????????????????
    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QJsonObject QObj;
        if(tempReply->error() ==  QNetworkReply::NoError)
        {
            //?????????????????????
            QByteArray replyArray = tempReply->readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
            if(loadDoc.isArray())
                QObj.insert("array",loadDoc.array());
            else
                QObj = loadDoc.object();
            requestState = true;
        }
        emit sentRequest(statusCode, QObj, requestState);

        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });
    //??????get??????
    networkAccessManager->get(_Request);

    //????????????,??????????????????
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void APIDisposeThread::api_getFile(QString UrlType)
{
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    //??????
    QNetworkRequest _Request;
    if(_newUrl != "")
        _Request.setUrl(QUrl(_newUrl));
    else
        _Request.setUrl(QUrl(globalAccess->MainUrl() + UrlType));

    QVariant var;
    var.setValue(_allcookies);
    _Request.setHeader(QNetworkRequest::CookieHeader, var);

    //?????????????????????????????????
    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(tempReply->error() ==  QNetworkReply::NoError)
        {
            //?????????????????????
            QByteArray replyArray = tempReply->readAll();
            requestState = true;
            emit sentByte(statusCode, replyArray, requestState);
        }

        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });
    //??????get??????
    networkAccessManager->get(_Request);

    //????????????,??????????????????
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}
