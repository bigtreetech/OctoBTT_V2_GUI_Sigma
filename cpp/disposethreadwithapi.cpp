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

//执行任务的新线程
void APIDisposeThread::run()
{
    //用来中断线程
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

    //测试代码
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
    //必要, 防止线程释放过快导致network的线程没有释放
    msleep(10);
}

void APIDisposeThread::stopAllThread()
{
    //删除容器saverThread保存的线程
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
            //关闭第一个键对应的线程
            if(tmep->isRunning())
                tmep->stop();

            //从一级容器中删除这个UrlType类别
            APIDisposeThread::queueThreadKey.remove(tempUrlType);
        }
        APIDisposeThread::queueThreadKey.clear();
    }
}

void APIDisposeThread::stopOneTypeThread(QString UrlType)
{
    if(APIDisposeThread::threadKey.count() != 0)
    {
        //删除容器saverThread相应UrlType的线程
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
            //就遍历二级容器并且删除这个UrlType类型下QMap里面所有的元素
            if(tempUrlType == UrlType)
            {
                APIDisposeThread *tmep = APIDisposeThread::queueThreadKey[tempUrlType].begin().value();
                //关闭第一个键对应的线程
                APIDisposeThread::queueThreadKey[tempUrlType].clear();
                if(tmep->isRunning())
                    tmep->stop();

                //从一级容器中删除这个UrlType类别
                APIDisposeThread::queueThreadKey.remove(tempUrlType);
            }
        }
    }
}

void APIDisposeThread::queueThread()
{
    //创建队列函数处理线程用来处理队列API的线程, 避免主线程卡顿
    QFuture<void> queueThread;
    APIDisposeThread queueRunThread;
    //该线程自带销毁, 不需要添加额外的代码进行内存释放
    queueThread = QtConcurrent::run(&queueRunThread, &APIDisposeThread::runNewQueueThread);
}

void APIDisposeThread::runNewQueueThread()
{
    if(APIDisposeThread::queueThreadKey.count() != 0)
    {
        foreach(QString tempUrlType, APIDisposeThread::queueThreadKey.keys())
        {
            //如果每个类别对应的二级容器首个键值的线程没有运行, 就运行这个线程
            if(APIDisposeThread::queueThreadKey[tempUrlType].count() != 0)
            {
                if(!APIDisposeThread::queueThreadKey.value(tempUrlType).begin().value()->isRunning())
                {
                    APIDisposeThread *tempThread = APIDisposeThread::queueThreadKey.value(tempUrlType).begin().value();

                    connect(tempThread, &QThread::finished, tempThread, &QObject::deleteLater);

                    //线程完成后, 从容器中删除第一个元素
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
    //队列API(新的请求不会结束旧的请求的线程)
    if(queueOrNo == true)
    {
        //==========================================================================存在潜在问题
        //如果一级容器是空的就直接在一级容器里面添加这个UrlType, 并生成二级容器中第一个元素
        if(APIDisposeThread::queueThreadKey.value(UrlType).count() == 0)
        {
            QMap<int, APIDisposeThread *> tempMap;
            tempMap.insert(0, newThread);
            APIDisposeThread::queueThreadKey.insert(UrlType, tempMap);
            queueThread();
        }
        //一级容器不为空就遍历二级容器
        else
        {
            foreach(QString tempUrlType, APIDisposeThread::queueThreadKey.keys())
            {
                //如果目标API的UrlType保存在一级容器里, 就遍历二级容器
                if(tempUrlType == UrlType)
                {
                    //如果二级容器是空的, 就直接创建0号序列
                    if(APIDisposeThread::queueThreadKey.value(tempUrlType).count() == 0)
                    {
                        APIDisposeThread::queueThreadKey[tempUrlType].insert(0, newThread);
                    }
                    //否则就将加入队列
                    else
                    {
                        //键的大小为容器最后一个键的大小 + 1
                        APIDisposeThread::queueThreadKey[tempUrlType].insert(APIDisposeThread::queueThreadKey.value(tempUrlType).lastKey() + 1, newThread);
                    }
                }
            }
        }
    }
    //非队列API(新的请求会立即结束旧的请求的线程)
    else
    {
        foreach(APIDisposeThread *oneThread, APIDisposeThread::threadKey.keys())
        {
            if(APIDisposeThread::threadKey.value(oneThread) == UrlType)
            {
                //停止这个线程
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

        //线程结束时,清理内存
        connect(newThread, &QThread::finished, newThread, [=](){
            //线程完成时, 从容器里面删除
            APIDisposeThread::threadKey.remove(newThread);
            newThread->deleteLater();
        });

        //运行线程
        if(!sameThreadFlag)
        {
            //保存新线程
            APIDisposeThread::threadKey.insert(newThread, UrlType);
            newThread->start();
        }
    }
}

void APIDisposeThread::stopThread(QString threadType)
{
    //创建清理线程用来清理处理API的线程, 避免主线程卡顿
    QFuture<void> clearThread;
    QFuture<void> clearQueueThread;
    APIDisposeThread stopNoQueueThread;
    APIDisposeThread stopQueueThread;
    //如果不指定某一类别的API就结束所有线程
    if(threadType == NULL)
    {
        //该线程自带销毁, 不需要添加额外的代码进行内存释放
        clearThread = QtConcurrent::run(&stopNoQueueThread, &APIDisposeThread::stopAllThread);
        clearQueueThread = QtConcurrent::run(&stopQueueThread, &APIDisposeThread::stopAllQueueThread);
    }
    //如果指定某一类别的API就结束指定类别的线程
    else{
        //该线程自带销毁, 不需要添加额外的代码进行内存释放
        clearThread = QtConcurrent::run(&stopNoQueueThread, &APIDisposeThread::stopOneTypeThread, threadType);
        clearQueueThread = QtConcurrent::run(&stopQueueThread, &APIDisposeThread::stopOneTypeQueueThread, threadType);
    }
}
//用以post请求
void APIDisposeThread::api_post_jsonObject(QString runMode, QString UrlType, QJsonObject TemperatureJsonObj)
{
    //创建连接
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

    //无返还内容
    if(runMode == QString("post_0"))
    {
        //如果连接成功接收返回值
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
    //有返还内容
    if(runMode == QString("post_1"))
    {
        //如果连接成功接收返回值
        connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
        {
            tempReply = reply;
            //将保存的数据返回
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
                //将参数传出
                emit sentRequest(statusCode, QObj, requestState);
            }
            //结束时中断信号连接
            networkAccessManager->deleteLater();
            reply->deleteLater();
            networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
        });
        //发出post请求
        networkAccessManager->post(_Request, _SenderJson.toJson());
    }
    //阻塞线程,直到请求结束 (QNetworkAccessManager本身会生成线程, 用线程处理必须加入循环)
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void APIDisposeThread::api_post_filePath(QString UrlType, QString folderPath, QString FilePath)
{
    //创建连接
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
    QFile *file = new QFile(FilePath);     //由于是异步，所以记得一定要new
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

        //结束时中断信号连接
        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });

    networkAccessManager->post(_Request, multiPart);

    //阻塞线程,直到请求结束 (QNetworkAccessManager本身会生成线程, 用线程处理必须加入循环)
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    multiPart->deleteLater();
    file->deleteLater();
}

void APIDisposeThread::api_post_createfilePath(QString UrlType, QString folderPath, QString newFolder)
{
    //创建连接
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

        //结束时中断信号连接
        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });

    networkAccessManager->post(_Request, multiPart);

    //阻塞线程,直到请求结束 (QNetworkAccessManager本身会生成线程, 用线程处理必须加入循环)
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
    multiPart->deleteLater();
}

void APIDisposeThread::api_post_device(QString UrlType, QIODevice *data)
{
    //创建连接
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
        //结束时中断信号连接
        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });

    networkAccessManager->post(_Request, data);

    //阻塞线程,直到请求结束 (QNetworkAccessManager本身会生成线程, 用线程处理必须加入循环)
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void APIDisposeThread::api_delResource(QUrl Url)
{
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    //连接
    QNetworkRequest _Request;
    _Request.setUrl(Url);

    QVariant var;
    var.setValue(_allcookies);
    _Request.setHeader(QNetworkRequest::CookieHeader, var);

    //如果连接成功接收返回值
    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QJsonObject QObj;
        if(tempReply->error() ==  QNetworkReply::NoError)
        {
            //返还的数据处理
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
    //发出get请求
    networkAccessManager->deleteResource(_Request);

    //阻塞线程,直到请求结束
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

//用以get请求
void APIDisposeThread::api_get(QString UrlType)
{
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    //连接
    QNetworkRequest _Request;
    if(_newUrl != "")
        _Request.setUrl(QUrl(_newUrl));
    else
        _Request.setUrl(QUrl(globalAccess->MainUrl() + UrlType));

    QVariant var;
    var.setValue(_allcookies);
    _Request.setHeader(QNetworkRequest::CookieHeader, var);

    //如果连接成功接收返回值
    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QJsonObject QObj;
        if(tempReply->error() ==  QNetworkReply::NoError)
        {
            //返还的数据处理
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
    //发出get请求
    networkAccessManager->get(_Request);

    //阻塞线程,直到请求结束
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void APIDisposeThread::api_getFile(QString UrlType)
{
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();
    //连接
    QNetworkRequest _Request;
    if(_newUrl != "")
        _Request.setUrl(QUrl(_newUrl));
    else
        _Request.setUrl(QUrl(globalAccess->MainUrl() + UrlType));

    QVariant var;
    var.setValue(_allcookies);
    _Request.setHeader(QNetworkRequest::CookieHeader, var);

    //如果连接成功接收返回值
    connect(networkAccessManager, &QNetworkAccessManager::finished,this, [=](QNetworkReply *reply)
    {
        tempReply = reply;
        int statusCode = tempReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(tempReply->error() ==  QNetworkReply::NoError)
        {
            //返还的数据处理
            QByteArray replyArray = tempReply->readAll();
            requestState = true;
            emit sentByte(statusCode, replyArray, requestState);
        }

        networkAccessManager->deleteLater();
        reply->deleteLater();
        networkAccessManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    });
    //发出get请求
    networkAccessManager->get(_Request);

    //阻塞线程,直到请求结束
    QEventLoop loop;
    connect(networkAccessManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();
}
