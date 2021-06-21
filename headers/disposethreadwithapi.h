#ifndef DISPOSETHREADWITHAPI_H
#define DISPOSETHREADWITHAPI_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QtConcurrent>
#include <QJsonObject>
#include <QEventLoop>
#include <QMetaType>
#include <QFuture>
#include <QThread>
#include <QDebug>
#include "accesscontrol.h"

/**************************************************
*   文件名: disposethreadwithapi.h and disposethreadwithapi.cpp
*   依赖文件:
*   工程引用: network concurrent
*   文件用途: 动态生成线程处理通用性API(目前支持get/post/deleteResource)
*   文件作用：提高系统性能, 利用多线程来处理多种多个API请求
*
*   //构造函数 APIDisposeThread(QString runMode, QString UrlType, , QList<QNetworkCookie> allcookies, QJsonObject TemperatureJsonObj = QJsonObject(), QString newUrl = "");
*   //参数说明
*   {
*      runMode(选择连接方式get/post_0->无返回内容/post_1->有返回内容)   //runMode = "get"
*
*      UrlType(既是URL的工作方式, 也是保存线程容器QHash对应线程的键值)(API 尾缀)    //UrlType = "settings"//API: http://192.168.100.64/api/settings
*
*      allcookies(外部传来的cookie, 一般由accesscontrol中的API_login获得)
*
*      TemperatureJsonObj/QString FilePath/QIODevice *data(API为post需要传递的参数, 对应三种重载形式)   //TemperatureJsonObj 默认是（）
*
*      newUrl(不使用默认的URL时, 填入这个<当实际使用的Url非标准API时引用>)  //摄像机Url：http://192.168.100.64/webcam/?action=snapshot
*   }
*   使用举例:
*      APIDisposeThread *newThread = new APIDisposeThread("get", "connection", allcookies, TemperatureJsonObj);
*      //开启线程   最后一个参数是API属于队列或者不属于队列的判断 默认为false(不属于队列)
*      //如果不属于队列, 相同类型API的新线程会先停止旧线程再执行, 如果属于队列, 那么新线程会排在旧线程的后面
*      APIDisposeThread::runNewThread(newThread, tempUrl, false);
*
*      //接收线程返回的信息
*      connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
*          statusCode = _statusCode;    //状态码
*          QObj = _QObj;                //反馈的QObject信息
*          requestState = _requestState;//对应网络请求是否成功
*      });
*
*      //当需要结束进程的时候使用以下静态函数
*      //stopThread(QString threadType = NULL)   不加入参数默认停止所有线程, 加入参数停止某一类型的线程 例如加入"job"关闭所有job类型的API线程
*      APIDisposeThread::stopThread();
*
*      最后修改日期: 2021 / 5 / 27
**************************************************/

extern AccessControl *globalAccess;
class APIDisposeThread : public QThread
{
    Q_OBJECT
public:
    explicit APIDisposeThread(QObject *parent = 0);
    //通过生成对象时候将各个参数加入构造函数, 从而保存于私有成员中                                        TemperatureJsonObj/FilePath/data post三种重载形式
    explicit APIDisposeThread(QString runMode, QString UrlType, QList<QNetworkCookie> allcookies, QJsonObject TemperatureJsonObj = QJsonObject(), QString newUrl = "");
//    explicit APIDisposeThread(QString runMode, QString UrlType, QList<QNetworkCookie> allcookies, QString FilePath, QString newUrl = "");
    explicit APIDisposeThread(QString runMode, QString UrlType, QList<QNetworkCookie> allcookies, QIODevice *data, QString newUrl = "");
    explicit APIDisposeThread(QString runMode, QString UrlType, QList<QNetworkCookie> allcookies,QString folderUploadPath, QString uploadFilePath, QString newUrl = "");
    explicit APIDisposeThread(QString runMode, QString UrlType, QString folderCreatePath, QString createNewFolder, QList<QNetworkCookie> allcookies, QString newUrl = "");
    ~APIDisposeThread();
    //用来停止线程
    void stop();
public:
    static QHash<APIDisposeThread *, QString> threadKey;    /* 用以保存非队列的 API的线程/区别名(UrlType) */
    static QHash<QString, QMap<int, APIDisposeThread *>> queueThreadKey;    /* 用以保存队列API的 区别名(UrlType)/序号/线程 */
    //API所使用的Url配置
//    QString MainIP = "127.0.0.1";//Set: IP:Port
////    QString MainIP = "192.168.0.26";//Set: IP:Port
//    QString MainAddress()
//    {
//        return "http://"+MainIP+"/";//Get: MainUrl
//    }
//    QString MainUrl()
//    {
//        return MainAddress() + "api/";//Get: ApiUrl
//    }
protected:
    //新线程的真正执行函数
    void run();
private:
    volatile bool stopped;  /* 用以停止线程, 为true时停止线程 */
    int threadCount = 0;    /* 测试代码段 */
    int choosePostMode = 0; /* post三种重载选择 */
    QNetworkReply *tempReply; /* 临时接收应答变量 */
    QString _runMode;       /* 运行模式 */
    QString _UrlType;       /* URL类别 */
    QString _FilePath;      /* 文件路径 */
    QIODevice *_data;
    QJsonObject _TemperatureJsonObj;
    QString _newUrl;        /* 不使用默认URL值时使用 */
    bool requestState = false;  /* 线程应答情况, 无错误为true */
    QList<QNetworkCookie> _allcookies; /* 接收外部传来的cookie */
    QString _folderPath;    /* 要创建文件夹的路径 */
    QString _newFolder;     /* 新的文件夹 */
    QString _folderUploadPath; /* 上传目文件夹的路径 */
    QString _uploadFilePath; /* 要上传的文件所在路径 */
public:
    //新线程相关操作处理函数
    static void runNewThread(APIDisposeThread *newThread, QString UrlType, bool queueOrNo = false);

    //外部调用停止线程函数, 从而生成并发线程来执行停止线程的操作
    static void stopThread(QString threadType = NULL);

    //队列和非队列的API分开使用线程, 加速
    //停止所有线程函数
    void stopAllThread();
    void stopAllQueueThread();

    //停止某一API类型的线程函数
    void stopOneTypeThread(QString UrlType);
    void stopOneTypeQueueThread(QString UrlType);
private:
    //队列线程函数
    void runNewQueueThread();
    static void queueThread();

    //runMode{get / post(post_0->无返回内容, post_1->有返回内容) / deleterResource}
    //post三种重载形式
    void api_post_jsonObject(QString runMode, QString UrlType, QJsonObject TemperatureJsonObj);
    void api_post_filePath(QString UrlType, QString folderPath, QString FilePath);
    void api_post_createfilePath(QString UrlType, QString folderPath, QString newFolder);
    void api_post_device(QString UrlType, QIODevice *data);
    //del
    void api_delResource(QUrl Url);

    //get
    void api_get(QString UrlType);
    void api_getFile(QString UrlType);
signals:
    //将分析后的值传递出去
    void sentRequest(int, QJsonObject, bool);

    //sent getFile
    void sentByte(int, QByteArray, bool);
};
#endif // MAINWINDOW_H
