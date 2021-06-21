#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonArray>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>
Q_DECLARE_METATYPE(QList<QNetworkCookie>);

struct UserInfo: public QObject{
    Q_OBJECT
//    Q_PROPERTY(QString &uid MEMBER uid)
    Q_PROPERTY(QString uid MEMBER uid NOTIFY uidChange);
    Q_PROPERTY(QString onlineUid MEMBER onlineUid NOTIFY onlineUidChange);
    Q_PROPERTY(QString pwd MEMBER pwd NOTIFY pwdChange);
    Q_PROPERTY(bool admin MEMBER admin NOTIFY adminChange);
    Q_PROPERTY(bool login MEMBER login NOTIFY loginChange);
public:
    QList<QNetworkCookie> cookies;//对话发起cookies
    QByteArray API_Key;//{"apikey"}//如果没设置就不存在，引用全局Key,此key为用户key
    QByteArray session;//{"session"}//每次登录成功会获得新的对话ID，不区分超级账户和用户
private:
    QString uid;//用户设置登录账号
    QString onlineUid;//当前登录账号
    QString pwd;//账户密码
    bool admin;//{"admin"}//判断权限是否充足，仅仅高级账户才能运行UI运作
    bool login;//{"active"}//登录失败403拒绝访问，布尔值归位默认false账户///为websocket提供接入ID
private:
    QString _test;
public:
    UserInfo(QString _uid = NULL , QString _pwd = NULL)
    {
        if(_uid != NULL && _pwd != NULL)
        {
            this->setProperty("uid",_uid);
            this->setProperty("pwd",_pwd);
//            uid = _uid;
//            pwd = _pwd;
        }
        this->setProperty("admin",false);
        this->setProperty("login",false);
//        admin = false;
//        login = false;
        API_Key = "";
        session = "";

        connect(this,&UserInfo::uidChange,this,[=](){emit change();});
        connect(this,&UserInfo::onlineUidChange,this,[=](){emit change();});
        connect(this,&UserInfo::pwdChange,this,[=](){emit change();});
        connect(this,&UserInfo::adminChange,this,[=](){emit change();});
        connect(this,&UserInfo::loginChange,this,[=](){emit change();});
    };
    ~UserInfo()
    {

    };
signals:
    void uidChange();
    void onlineUidChange();
    void pwdChange();
    void adminChange();
    void loginChange();

    void change();
};
//Q_DECLARE_METATYPE(UserInfo);

class AccessControl: public QObject
{
    Q_OBJECT
    Q_PROPERTY(UserInfo* userinfo MEMBER userinfo NOTIFY userinfoChange);
    Q_PROPERTY(QString mainIP MEMBER MainIP NOTIFY mainIPChange);
public:
    AccessControl();
    ~AccessControl();
    QNetworkAccessManager *ACManager = new QNetworkAccessManager();
    QNetworkRequest GetACRequest;

    void acLogin();
    void acLogout();
    UserInfo *userinfo = new UserInfo();

//    QString MainIP = "192.168.0.26";//Set: IP:Port
    Q_INVOKABLE QString MainAddress()
    {
        return "http://"+MainIP+"/";//Get: MainUrl
    }
    Q_INVOKABLE QString MainUrl()
    {
        return MainAddress() + "api/";//Get: ApiUrl
    }
signals:
    void CommunicationStateChange(bool replySuccess,bool replyNoError);
//    void connectionTimeoutSignal();
    void userinfoChange();
    void mainIPChange();
private:
//    QWidget *FUI;
    QString MainIP = "127.0.0.1";//Set: IP:Port

    QJsonValue SearchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj);
    QList<QJsonValue> SearchJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj);

    const QList<QList<QString>> LoginMsg = {{"active"},{"admin"},{"apikey"},{"name"},{"session"}};

    QTimer *loginTimer;
//    bool connectionTimeoutFlag = true;
//    QString *userinfo_uid = &userinfo->uid;
private slots:
    void LoginReply(QNetworkReply *reply);
    void LogoutReply(QNetworkReply *reply);

};

#endif // ACCESSCONTROL_H
