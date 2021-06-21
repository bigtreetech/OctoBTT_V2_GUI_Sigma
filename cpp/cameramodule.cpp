#include "../headers/cameramodule.h"

CameraModule::CameraModule(QString _address, QString _path)
{
    //开启定时器
    frameTimer = new QTimer(this);
    frameTimer->setSingleShot(true);

    //设置帧数
    CameraModule::setFrame();

    //获取Url
    path = _path;
    address = _address;

    //允许线程以及相机摄像开启
    stoppedRequest = false;
    stoppedThread = false;
}

CameraModule::~CameraModule()
{
    this->terminate();
    stopCameraThread();
    frameTimer->disconnect(SIGNAL(timeout()));
    frameTimer->stop();

    qDebug()<<__FILE__<<__LINE__<<"destroyed_Thread";

    this->destroyed();
    this->deleteLater();
}

void CameraModule::run()
{
    /* 创建一个用于应用程序发送网络请求和接收网络应答的"管理员" */
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    /* 创建一个要发送的请求实例 */
    QNetworkRequest *request = new QNetworkRequest(QUrl(address + path));
    QEventLoop loop;
    QEventLoop loopframeTimer;
    connect(frameTimer,&QTimer::timeout,&loopframeTimer,&QEventLoop::quit);
    connect(this, &CameraModule::loopQiut, &loop, &QEventLoop::quit);

    connect(this, &CameraModule::finished, &loop, &QEventLoop::quit);

    connect(this, &CameraModule::finished, this, [=]()
    {
        manager->destroyed();
        manager->deleteLater();
        delete request;

    });

    request->setUrl(QUrl(address + path));
    while(!stoppedThread)
    {
        if(!stoppedRequest && !frameTimer->isActive())
        {
            connect(manager, &QNetworkAccessManager::finished, this, &CameraModule::mjpeg_streamer_reply);
            connect(manager, &QNetworkAccessManager::finished, manager, [=](){
                manager->disconnect(SIGNAL(finished(QNetworkReply*)));
            });
            request->setUrl(QUrl(address + path));//重置地址
            manager->get(*request);
            //阻塞线程,直到请求结束
            loop.exec();
        }
        else if(!stoppedRequest && !frameTimer->isActive())
            loopframeTimer.exec();
        else
            QThread::sleep(time/100);
//        manager->disconnect(SIGNAL(finished(QNetworkReply*)));
    }

//    if(stoppedThread)
//    {
        qDebug()<<__FILE__<<__LINE__<<"Exit_Thread";
//        return;
//    }
}

void CameraModule::mjpeg_streamer_reply(QNetworkReply *reply)
{
//    qDebug() << __FILE__ << __LINE__ << "reciver";
    if(reply->error() == QNetworkReply::NoError)
    {
        /* 读出全部数据 */
        QByteArray byteArr;
        byteArr = reply->readAll();

        //将图片信息抛出
        QPixmap outPixmap;
        if(outPixmap.loadFromData(byteArr))
        {
            emit sentQPixmap(outPixmap);
        }
        QImage outimage;
        if(outimage.loadFromData(byteArr))
        {
            viewProvider->updateView(outimage);
            emit sentQImage(outimage);
        }
    }

    emit loopQiut();
    frameTimer->start(time);

    reply->destroyed();
    reply->deleteLater();
}

void CameraModule::openCamera(/*CameraModule *thread*/)
{
    //保存用以释放
//    _thread = this;
    if(!this->isRunning())
    {
        this->start();
    }

    stoppedThread = false;
    stoppedRequest = false;
}

void CameraModule::stopCameraThread()
{
    stoppedRequest = true;
    stoppedThread = true;
}

void CameraModule::closeCamera()
{
    stoppedRequest = true;
    stoppedThread = false;
}

void CameraModule::setFrame(double frame)
{
    time = 1000 / frame;
}

double CameraModule::getFrame()
{
    return (1000 / time);
}
