#ifndef CAMERAMODULE_H
#define CAMERAMODULE_H

#include <QNetworkReply>
#include <QtConcurrent>
#include <QFuture>
#include <QObject>
#include <QTimer>
#include <QBitmap>
#include "viewprovider.h"

/**************************************************
*   文件名: cameramodule.h and cameramodule.cpp
*   依赖文件:
*   工程引用: network
*   文件用途: 用以用户更好地使用摄像头
*   文件作用：用线程控制相机的运作, 以提高性能
*
*   //构造函数CameraModule(QString _address, QString _path = "webcam/?action=snapshot")
*   参数说明
*   {
*       _address IP地址
*       _path    请求路径 默认为webcam/?action=snapshot
*   }
*   使用举例:
*      CameraModule *_camera = new CameraModule(globalAccess->MainAddress());
*      //连接传出图片信息的槽
*      connect(_camera, &CameraModule::sentImage, this, &MainApi::imageShow);
*
*      //开启相机
*      _camera->openCamera(_camera);
*      if(flag)
*      {
*          connect(_camera, &CameraModule::sentImage, this, &MainApi::imageShow);
*      }
*
*      //暂停相机
*      _camera->closeCamera();
*      disconnect(_camera, &CameraModule::sentImage, this, &MainApi::imageShow);
*      flag = true;
*
*      //获取相机帧数
*      connect(_camera, &CameraModule::sentFarme, this, &MainApi::farmeShow);
*
*      //调整相机帧数 参数为想要设置的帧数, 默认为30帧
*      _camera->setFrame(30);
*
*      最后修改日期: 2021 / 3 / 26
**************************************************/
extern ViewProvider *viewProvider;
class CameraModule : public  QThread
{
    Q_OBJECT
    
    Q_PROPERTY(double frame READ getFrame WRITE setFrame NOTIFY frame_Change);
    Q_PROPERTY(QString address MEMBER address NOTIFY address_Change);
    Q_PROPERTY(QString path MEMBER path NOTIFY path_Change);
    
public:
    explicit CameraModule(QString _address = "http://127.0.0.1/", QString _path = "webcam/?action=snapshot");
    ~CameraModule();
public:
    void stop();                            /* 用来停止线程 */
    Q_INVOKABLE void openCamera(/*CameraModule *thread*/);  /* 外部调用开启相机线程 */
    void stopCameraThread();                /* 停止线程 */
    Q_INVOKABLE void closeCamera();                     /* 暂停相机 */
    void setFrame(double frame = 45);          /* 设置帧数 */
    double getFrame();                      /* 获取帧数 */
protected:
    //新线程的真正执行函数
    void run();
signals:
    void sentQPixmap(QPixmap image);  /* 发送图片信息 */
    void sentQImage(QImage image);  /* 发送图片信息 */
//    void sentFarme(int frame);      /* 发送相机帧数 */
    void loopQiut();    /* 阻塞loop退出 */
    void frame_Change();
    void address_Change();
    void path_Change();
private slots:
     void mjpeg_streamer_reply(QNetworkReply *reply);   /* 相机API请求处理函数 */
private:
     volatile bool stoppedRequest;  /* 暂停相机 */
     volatile bool stoppedThread;   /* 用以停止线程, 为true时停止线程 */
     int time;                      /* 帧数定时器的时间 */
//     CameraModule *_thread;         /* 用以保存 */
     QTimer *frameTimer;            /* 帧数定时器 */
public:
     QString address = "http://127.0.0.1/";
     QString path = "webcam/?action=snapshot";
     QNetworkReply* reply;
private:

};

#endif // CAMERAMODULE_H
