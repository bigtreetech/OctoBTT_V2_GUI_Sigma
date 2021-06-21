#ifndef USERLOGINMODULE_H
#define USERLOGINMODULE_H

#include <QFile>
#include <QObject>
#include <QJsonParseError>
#include "accesscontrol.h"
#include "disposethreadwithapi.h"
#include "websocketmodule.h"

/**************************************************
*   文件名: userloginmodule.h and userloginmodule.cpp
*   依赖文件:accesscontrol.h and accesscontrol.cpp disposethreadwithapi.h and disposethreadwithapi.cpp
*   工程引用:
*   文件用途: 验证账号的初始化, 完成登录操作, 并保存用户的登录信息
*   文件作用：使得操作OCTOBTT需要权限认证
*
*   使用举例:
*      userlogin = new UserLoginModule(this);
*      //根据下面的函数userLoginBTT()返回的值来执行操作
*      connect(userlogin, &UserLoginModule::userLoginCase, this, [=](int loginState){
*        //该octoprint已完成初始化且需要登录
*        if(loginState == 0x000)
*        {
*           //登录按键按下
*           ui->userLabel->setText("Please log in");
*           //用户输入账号和密码
*           globalAccess->userinfo->uid = ui->userIdLineEdit->text();
*           globalAccess->userinfo->pwd = ui->userPasswordLineEdit->text();
*
*           QEventLoop loop_1;
*           QObject::connect(globalAccess,&AccessControl::CommunicationStateChange,&loop_1,&QEventLoop::quit);
*           QObject::connect(globalAccess,&AccessControl::CommunicationStateChange,this,[=](bool _return){
*               qDebug() << "loop is good" << __LINE__ << __FILE__;
*               //返回值正确, 则登录成功
*               if(_return)
*               {
*                   ui->userLabel->setText("~~~! Welcome !~~~");
*                   //更新配置文件
*                   userlogin->profileUpdate(ui->autologinCheckBox->isChecked(), ui->rememberCheckBox->isChecked());
*               }
*               //返回值错误, 则账号或者密码错误
*               else
*               {
*                    ui->userLabel->setText("Incorrect account or password");
*               }
*           });
*
*           globalAccess->acLogin();
*           loop_1.exec();
*       }
*       //该octoprint为0.17版本, 且在octoprint网页上完成了免注册, 故可以直接使用OCTOBTT
*       else if(loginState == 0x100)
*       {
*           ui->userLabel->setText("Welcome");
*       }
*       //该octoprint未完成初始化, 需要注册
*       else if(loginState == 0x110)
*       {
*            //按键按下
*            //获取UI上面的注册信息, 进行注册并且登录
*            userlogin->userWizard(ui->userIdLineEdit->text().trimmed(), ui->userPasswordLineEdit->text(), ui->userPasswordLineEdit->text());
*            //更新配置文件
*            userlogin->profileUpdate(ui->autologinCheckBox->isChecked(), ui->rememberCheckBox->isChecked());
*       }
*       //使用的IP地址错误
*       else if(loginState == 0xfff)
*       {
*           ui->userLabel->setText("IP address error");
*           ui->loginPushButton->setEnabled(false);
*           ui->resetPushButton->setEnabled(true);
*       }
*      });
*      userlogin->userLoginBTT();   //检测是否具有配置文件config.json 检测octoprint是否初始化
*
*
*      最后修改日期: 2021 / 4 / 23
**************************************************/

extern AccessControl *globalAccess;
extern WebSocketModule *myWebsoket;

class UserLoginModule : public QObject
{
    Q_OBJECT

    //注册属性，使之可以在QML中访问--具体语法:Q_PROPERTY
//    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(int year READ getYear WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(bool autoLogin MEMBER _autoLogin NOTIFY autoLoginChange);
    Q_PROPERTY(bool rememberMeBool MEMBER _rememberMeBool NOTIFY rememberMeBoolChange);
public:
    explicit UserLoginModule(QObject *parent = nullptr);

private:
    //加密
    QString plaintextEncrypted(QString pwd);

    //解密
    QString cipherDecryption(QString pwd);

    //网络探测
    void checkNetwork(QString tempIp = globalAccess->property("mainIP").toString().contains(":") ? globalAccess->property("mainIP").toString().split(":")[0] : globalAccess->property("mainIP").toString(), QString tempPort = globalAccess->property("mainIP").toString().contains(":") ? globalAccess->property("mainIP").toString().split(":")[1] : "80");
    void checkWithLogin();
    void checkWithLogout();


    //提取用户配置
    void extractingUserConfiguration();

    //判断登录状态
//    void determineLoginStatus(QString tempIp = "", QString tempPort = "");
    void determineLoginStatus(QString tempIp = globalAccess->property("mainIP").toString().contains(":") ? globalAccess->property("mainIP").toString().split(":")[0] : globalAccess->property("mainIP").toString(), QString tempPort = globalAccess->property("mainIP").toString().contains(":") ? globalAccess->property("mainIP").toString().split(":")[1] : "80");

    //    更新配置文件
    //    void profileUpdate(bool tempAutoLogin = false, bool tempRememberMe = false, QString tempUid = "", QString tempPwd = "", QString tempIp = "127.0.0.1", QString tempPort = "81");
    void profileUpdate(bool tempAutoLogin, bool tempRememberMe, QString tempUid = globalAccess->userinfo->property("uid").toString(), QString tempPwd = globalAccess->userinfo->property("pwd").toString(), QString tempIp = (globalAccess->property("mainIP").toString().contains(":") ? globalAccess->property("mainIP").toString().split(":")[0] : globalAccess->property("mainIP").toString()), QString tempPort = (globalAccess->property("mainIP").toString().contains(":") ? globalAccess->property("mainIP").toString().split(":")[1] : "80"));

    //用户初始化
    void userWizard(QString user = globalAccess->userinfo->property("uid").toString(), QString pass1 = globalAccess->userinfo->property("pwd").toString(), QString pass2 = globalAccess->userinfo->property("pwd").toString(), bool ac = true);

    //登录流程
    void userLoginBTT();
private:
    bool _autoLogin = true;
//    QString _rememberMeString = "remember me";    /* remember me or false */
    QJsonValue _rememberMeString_(bool remember)
    {
        return (remember ? QJsonValue("remember me") : QJsonValue(false));
    }
    bool _rememberMeBool = true;
    QString _port = "80";

private:
    QString configPath = QCoreApplication::applicationDirPath() + "/userConfig.json";

signals:
    void autoLoginChange();
    void rememberMeBoolChange();
    //传递出用户信息
    void userInformation(QString _uid, QString _pwd, QString _ip, QString _port, bool _autoLogin, bool rememberMe);

    Q_INVOKABLE void sendDetermineLoginStatus();
    Q_INVOKABLE void sendUserLogin();
    Q_INVOKABLE void sendUserWizard();
    Q_INVOKABLE void sendUserLogout();
    Q_INVOKABLE void initialize();
    //传递操作信息(loginState)
    //0x000:需要登陆
    //0x001:登陆失败,需要登陆
    //0x002:退出成功
    //0x100:免注册登录直接使用
    //0x110:需要注册
    //0x220:注册成功
    //0x120:注册失败
    //0x111:登陆成功
    //0xfff:IP地址错误
    void userLoginCase(int loginState);
    //true:IP地址可用(实时)
    //false:IP地址不可用(实时)
    void ipCase(bool online);

    //登录失败
//    void loginBTTfail(bool _return);
};

#endif // USERLOGINMODULE_H
