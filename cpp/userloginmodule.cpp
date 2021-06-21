#include "../headers/userloginmodule.h"

UserLoginModule::UserLoginModule(QObject *parent) : QObject(parent)
{
    connect(this,&UserLoginModule::sendUserLogin,this,[=](){
        checkWithLogin();
    });
    connect(this,&UserLoginModule::sendUserWizard,this,[=](){
        userWizard();
    });
    connect(this,&UserLoginModule::sendDetermineLoginStatus,this,[=](){
        checkNetwork();
    });
    connect(this,&UserLoginModule::sendUserLogout,this,[=](){
        checkWithLogout();
    });
//    connect(globalAccess->userinfo,&UserInfo::loginChange,this,[=](){
//        if(!globalAccess->userinfo->property("login").toBool())
//            emit userLoginCase(0x002);
//    });
    //changeprofile
    connect(globalAccess,&AccessControl::userinfoChange,this,[=](){
        profileUpdate(this->property("autoLogin").toBool(), this->property("rememberMeBool").toBool());
    });
    connect(globalAccess,&AccessControl::mainIPChange,this,[=](){
        profileUpdate(this->property("autoLogin").toBool(), this->property("rememberMeBool").toBool());
    });
    connect(this,&UserLoginModule::autoLoginChange,this,[=](){
        if(this->property("autoLogin").toBool())
        {
             this->setProperty("rememberMeBool",true);
        }
        profileUpdate(this->property("autoLogin").toBool(), this->property("rememberMeBool").toBool());
    });
    connect(this,&UserLoginModule::rememberMeBoolChange,this,[=](){
        profileUpdate(this->property("autoLogin").toBool(), this->property("rememberMeBool").toBool());
    });
    connect(this,&UserLoginModule::initialize,this,[=](){
        userLoginBTT();
    });
    connect(myWebsoket,&WebSocketModule::invalidSession,this,[=](){
        if(globalAccess->userinfo->property("admin").toBool())//清空检测位置
        {
            globalAccess->userinfo->API_Key = QByteArray();
            globalAccess->userinfo->session = QByteArray();
            globalAccess->userinfo->cookies = QList<QNetworkCookie>();
            globalAccess->userinfo->setProperty("onlineUid","");
            globalAccess->userinfo->setProperty("admin",false);
            globalAccess->userinfo->setProperty("login",false);
            checkNetwork();
        }
    });
    connect(myWebsoket,&WebSocketModule::checkIPResult,this,[=](bool result){
        emit ipCase(result);
        if(globalAccess->userinfo->property("admin").toBool() && !result)//清空检测位置
        {
            globalAccess->userinfo->API_Key = QByteArray();
            globalAccess->userinfo->session = QByteArray();
            globalAccess->userinfo->cookies = QList<QNetworkCookie>();
            globalAccess->userinfo->setProperty("onlineUid","");
            globalAccess->userinfo->setProperty("admin",false);
            globalAccess->userinfo->setProperty("login",false);
            emit userLoginCase(0xfff);//释放网络异常信号
        }
    });
}

void UserLoginModule::userLoginBTT()
{
    //判断userConfig.json文件在不在
    //有-提取各项信息
    qDebug() << __FILE__ << __LINE__ << configPath;
    if(QFile(configPath).exists())
        UserLoginModule::extractingUserConfiguration();

    //然后开始关于初始化的检测
    UserLoginModule::checkNetwork();
}

void UserLoginModule::profileUpdate(bool tempAutoLogin, bool tempRememberMe, QString tempUid, QString tempPwd, QString tempIp, QString tempPort)
{
    //如果userConfig.json存在就删除
    if(QFile(configPath).exists())
    {
        QFile file(configPath);
        file.remove();
    }
    //创建用户配置信息文件
    QFile file(configPath);
    //设置权限
    QFile::setPermissions(configPath, QFileDevice::ReadOwner|QFileDevice::WriteOwner);

    file.open(QFile::ReadWrite);
    if(file.isOpen())
    {
        //总Josn文件
        QJsonObject userJosn;
        userJosn.insert("autoLogin", tempAutoLogin);
        userJosn.insert("rememberMe",_rememberMeString_(tempRememberMe));
        userJosn.insert("pwd", tempRememberMe ? plaintextEncrypted(tempPwd) :  QJsonValue());

        userJosn.insert("uid", tempUid);
        userJosn.insert("ip", tempIp);
        userJosn.insert("prot", tempPort);

        //转换成文档格式
        QJsonDocument userDocument;
        userDocument.setObject(userJosn);
        QByteArray userByetArray = userDocument.toJson(QJsonDocument::Compact);

        //写入配置Jons文档中
        file.write(userByetArray);
    }
    file.close();
}

void UserLoginModule::extractingUserConfiguration()
{
    QFile file(configPath);
    //设置权限
    QFile::setPermissions(configPath, QFileDevice::ReadOwner|QFileDevice::WriteOwner);

    file.open(QFile::ReadWrite);
    if(file.isOpen())
    {
        QByteArray userByetArray = file.readAll();
        file.close();
        QJsonParseError jsonError;
        QJsonDocument userDocument = QJsonDocument::fromJson(userByetArray, &jsonError);

        //如果解析没有发生错误
        if (!userDocument.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            if(userDocument.isObject())
            {
                QJsonObject userJsonObject = userDocument.object();
                //解析用户信息json包
                if(userJsonObject.contains("rememberMe"))
                {
                    QJsonValue rememberMeValue = userJsonObject.value("rememberMe");
                    qDebug() << __FILE__ << __LINE__ << !rememberMeValue.isBool();
                    this->setProperty("rememberMeBool", !rememberMeValue.isBool());

//                    if(rememberMeValue.isBool())
//                        _rememberMeBool = rememberMeValue.toBool();
//                    if(rememberMeValue.isString())
//                        _rememberMeString = rememberMeValue.toString();
                }
                if(userJsonObject.contains("autoLogin"))
                {
                    QJsonValue autoLoginValue = userJsonObject.value("autoLogin");
                    if(autoLoginValue.isBool())
                        this->setProperty("autoLogin", autoLoginValue.toBool());
                }
                if(userJsonObject.contains("uid"))
                {
                    QJsonValue uidValue = userJsonObject.value("uid");
                    if(uidValue.isString())
                        globalAccess->userinfo->setProperty("uid", uidValue.toString());
                }
                if(userJsonObject.contains("pwd"))
                {
                    QJsonValue pwdValue = userJsonObject.value("pwd");
                    if(pwdValue.isString())
                        globalAccess->userinfo->setProperty("pwd", cipherDecryption(pwdValue.toString()));
                }
                if(userJsonObject.contains("prot"))
                {
                    QJsonValue portValue = userJsonObject.value("prot");
                    if(portValue.isString())
                        _port = portValue.toString();
                }
                if(userJsonObject.contains("ip"))
                {
                    QJsonValue ipValue = userJsonObject.value("ip");
                    if(ipValue.isString())
                    {
                        globalAccess->setProperty("mainIP",ipValue.toString() + ":" + _port);
//                        if(_port.toInt() != 80)
//                            globalAccess->setProperty("mainIP",ipValue.toString() + ":" + _port);
//                        else
//                            globalAccess->setProperty("mainIP",ipValue.toString());

                    }
                }
            }
        }
    }
}
void UserLoginModule::checkWithLogout()
{
    Terminal* pinger = new Terminal();
    QObject::connect(pinger,&Terminal::CMD_Reply,this,[=](QStringList value)
    {
        qDebug() << __FILE__ << __LINE__ << value;
        qDebug() << __FILE__ << __LINE__ << "IP Check: " +  QString(value.count() > 0 && value.last().contains(QRegExp(".*time=.*ms")));
        if(value.count() > 0 && value.last().contains(QRegExp(".*time=.*ms")))
        {
            emit ipCase(true);//释放网络通路信号
            //IP有效，执行鉴权探测
            connect(globalAccess,&AccessControl::CommunicationStateChange,this,[=]()
            {
                globalAccess->disconnect(SIGNAL(CommunicationStateChange(bool,bool)));
                emit userLoginCase(0x002);
            });
            globalAccess->acLogout();
        }
        else
        {
            emit userLoginCase(0xfff);
        }
    });
    pinger->SendCMD("ping "+(globalAccess->property("mainIP").toString().contains(":") ? globalAccess->property("mainIP").toString().split(":")[0] : globalAccess->property("mainIP").toString()),2000);
}
void UserLoginModule::checkWithLogin()
{
    Terminal* pinger = new Terminal();
    QObject::connect(pinger,&Terminal::CMD_Reply,this,[=](QStringList value)
    {
        qDebug() << __FILE__ << __LINE__ << value;
        qDebug() << __FILE__ << __LINE__ << "IP Check: " +  QString(value.count() > 0 && value.last().contains(QRegExp(".*time=.*ms")));
        if(value.count() > 0 && value.last().contains(QRegExp(".*time=.*ms")))
        {
            emit ipCase(true);//释放网络通路信号
            //IP有效，执行鉴权探测
            connect(globalAccess,&AccessControl::CommunicationStateChange,this,[=]()
            {
                globalAccess->disconnect(SIGNAL(CommunicationStateChange(bool,bool)));
                connect(globalAccess,&AccessControl::CommunicationStateChange,this,[=]()
                {
                    globalAccess->disconnect(SIGNAL(CommunicationStateChange(bool,bool)));
                    connect(globalAccess,&AccessControl::CommunicationStateChange,this,[=](bool replySuccess,bool replyNoError)
                    {
                        globalAccess->disconnect(SIGNAL(CommunicationStateChange(bool,bool)));
                        if(replySuccess && replyNoError)
                        {
                            emit userLoginCase(0x111);
                        }
                        else
                            emit userLoginCase(0x001);
                    });
                    globalAccess->acLogin();
                });
                globalAccess->acLogout();
            });
            globalAccess->acLogout();
        }
        else
        {
            emit userLoginCase(0xfff);
        }
    });
    pinger->SendCMD("ping "+(globalAccess->property("mainIP").toString().contains(":") ? globalAccess->property("mainIP").toString().split(":")[0] : globalAccess->property("mainIP").toString()),2000);
}
void UserLoginModule::checkNetwork(QString tempIp, QString tempPort)
{
    Terminal* pinger = new Terminal();
    QObject::connect(pinger,&Terminal::CMD_Reply,this,[=](QStringList value)
    {
        qDebug() << __FILE__ << __LINE__ << value;
        qDebug() << __FILE__ << __LINE__ << "IP Check: " +  QString(value.count() > 0 && value.last().contains(QRegExp(".*time=.*ms")));
        if(value.count() > 0 && value.last().contains(QRegExp(".*time=.*ms")))
        {
            emit ipCase(true);//释放网络通路信号
            //IP有效，执行鉴权探测
            connect(globalAccess,&AccessControl::CommunicationStateChange,this,[=]()
            {
                globalAccess->disconnect(SIGNAL(CommunicationStateChange(bool,bool)));
                determineLoginStatus(tempIp, tempPort);
            });
            globalAccess->acLogout();
        }
        else
        {
            emit userLoginCase(0xfff);
        }
    });
    pinger->SendCMD("ping "+tempIp,2000);
}

void UserLoginModule::determineLoginStatus(QString tempIp, QString tempPort)
{
    if(tempIp != "")
    {
        if(tempPort.toInt() != 80)
            globalAccess->setProperty("mainIP",tempIp + ":" + tempPort);
        else
            globalAccess->setProperty("mainIP",tempIp);
    }

    QString tempUid = globalAccess->userinfo->property("uid").toString();
    QString tempPwd = globalAccess->userinfo->property("pwd").toString();

    globalAccess->userinfo->setProperty("uid","");
    QJsonObject tempObject = QJsonObject();

    QEventLoop loop;
    qDebug() << __FILE__ << __LINE__ << QObject::connect(globalAccess, &AccessControl::CommunicationStateChange, &loop, [=](bool replySuccess,bool replyNoError)
    {
        Q_UNUSED(replySuccess);
        qDebug() << __FILE__ << __LINE__  << replyNoError ;
        if(!replyNoError)
        {
            emit userLoginCase(0xfff);///Address error
        }
        else
        {
            APIDisposeThread *newThread = new APIDisposeThread("get", "settings", globalAccess->userinfo->cookies, tempObject, globalAccess->MainUrl() + "settings");
            //接收线程返回的信息
            connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
                Q_UNUSED(_requestState);
                //已有注册用户, 需要登录
                if(_statusCode == 403)
                    emit userLoginCase(0x000);
                //无注册用户, 检测是否需要初始化
                else if(_statusCode == 200)
                {
                    //判断Get-settings中的api：key是不是空的, 为空需要注册, 否则可以直接操作
                    if(_QObj.contains("api"))
                    {
                        QJsonValue apiValue = _QObj.value("api");
                        if(apiValue.isObject())
                        {
                            QJsonObject keyObjcet = apiValue.toObject();
                            if(keyObjcet.contains("key"))
                            {
                                QJsonValue keyValue = keyObjcet.value("key");
                                if(keyValue.isString())
                                {
//                                    profileUpdate(_autoLogin, _rememberMeBool);
                                    globalAccess->userinfo->setProperty("admin",true);
//                                    globalAccess->userinfo->setProperty("login",true);
                                    emit userLoginCase(0x100);
                                }
                                else
                                    emit userLoginCase(0x110);
                            }
                        }
                    }
                }
            });
            APIDisposeThread::runNewThread(newThread, "settings", false);
        }
//        globalAccess->disconnect(SIGNAL(connectionTimeoutSignal(bool)));
        globalAccess->userinfo->setProperty("pwd" , tempPwd);
        globalAccess->userinfo->setProperty("uid" , tempUid);
    });
    QObject::connect(globalAccess, &AccessControl::CommunicationStateChange, &loop, &QEventLoop::quit);

    globalAccess->acLogin();
    loop.exec();

    globalAccess->disconnect(SIGNAL(CommunicationStateChange(bool,bool)));
}

void UserLoginModule::userWizard(QString user, QString pass1, QString pass2, bool ac)
{
    //注册账号
    QJsonObject aclObject;
    aclObject.insert("ac", ac);
    aclObject.insert("user", user);
    aclObject.insert("pass1", pass1);
    aclObject.insert("pass2", pass2);

    QJsonObject tempObject = QJsonObject();

    APIDisposeThread *newThread = new APIDisposeThread("post_0", "plugin/corewizard/acl", globalAccess->userinfo->cookies, aclObject, globalAccess->MainAddress()+"plugin/corewizard/acl");
    APIDisposeThread::runNewThread(newThread, "plugin/corewizard/acl", false);

    //接收线程返回的信息
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](/*int _statusCode, QJsonObject _QObj, bool _requestState*/)
    {
        //以上面注册的账号登录
        globalAccess->userinfo->setProperty( "uid" , user);
        globalAccess->userinfo->setProperty( "pwd" , pass1);

        QEventLoop loop_1;
        QObject::connect(globalAccess,&AccessControl::CommunicationStateChange,&loop_1,&QEventLoop::quit);

        globalAccess->acLogin();
        loop_1.exec();

        /* 设置 */
        //QJsonObject({"plugins":{"tracking":{"enabled":true}}})
        QJsonObject object_00;
        object_00.insert("enabled", true);

        QJsonObject object_01;
        object_01.insert("tracking", QJsonValue(object_00));

        QJsonObject setting_0_Object;
        setting_0_Object.insert("plugins", object_01);

        APIDisposeThread *newThread_0 = new APIDisposeThread("post_0", "settings", globalAccess->userinfo->cookies, setting_0_Object, globalAccess->MainUrl() + "settings");
        APIDisposeThread::runNewThread(newThread_0, "settings", true);

        //QJsonObject({"server":{"onlineCheck":{"enabled":true,"host":"8.8.8.8","interval":15,"port":"53"}}})
        QJsonObject object_10;
        object_10.insert("enabled", true);
        object_10.insert("interval", 15);
        object_10.insert("host", "8.8.8.8");
        object_10.insert("port", "53");

        QJsonObject object_11;
        object_11.insert("onlineCheck", QJsonValue(object_10));

        QJsonObject setting_1_Object;
        setting_1_Object.insert("server", object_11);

        APIDisposeThread *newThread_1 = new APIDisposeThread("post_0", "settings", globalAccess->userinfo->cookies, setting_1_Object, globalAccess->MainUrl() + "settings");
        APIDisposeThread::runNewThread(newThread_1, "settings", true);

        //QJsonObject({"server":{"pluginBlacklist":{"enabled":true}}})
        QJsonObject object_20;
        object_20.insert("enabled", true);

        QJsonObject object_21;
        object_21.insert("pluginBlacklist", QJsonValue(object_20));

        QJsonObject setting_2_Object;
        setting_2_Object.insert("server", object_21);

        APIDisposeThread *newThread_2 = new APIDisposeThread("post_0", "settings", globalAccess->userinfo->cookies, setting_2_Object, globalAccess->MainUrl() + "settings");
        APIDisposeThread::runNewThread(newThread_2, "settings", true);

        //QJsonObject({"temperature":[{"bed":60,"chamber":0,"extruder":210,"name":"ABS"},{"bed":100,"chamber":0,"extruder":180,"name":"PLA"}]})
        QJsonObject object_300;
        object_300.insert("name", "ABS");
        object_300.insert("extruder", 210);
        object_300.insert("bed", 100);
        object_300.insert("chamber", 0);

        QJsonObject object_301;
        object_301.insert("name", "PLA");
        object_301.insert("extruder", 180);
        object_301.insert("bed", 60);
        object_301.insert("chamber", 0);

        QJsonArray object_31;
        object_31.push_back(object_300);
        object_31.push_back(object_301);

        QJsonObject setting_3_Object;
        setting_3_Object.insert("temperature", object_31);
        /* 设置 */

        /* 完成初始化 */
        //QJsonObject({"handled":["corewizard","tracking"]})
        QJsonArray wizardArray;
        wizardArray.insert(0,"corewizard");
        wizardArray.insert(1,"tracking");

        QJsonObject wizardObjcet;
        wizardObjcet.insert("handled", wizardArray);

        APIDisposeThread *newThread_3 = new APIDisposeThread("post_0", "settings", globalAccess->userinfo->cookies, setting_3_Object, globalAccess->MainUrl() + "settings");
        APIDisposeThread::runNewThread(newThread_3, "settings", true);

        connect(newThread_3, &APIDisposeThread::sentRequest, this, [=](/*int _statusCode, QJsonObject _QObj, bool _requestState*/){
            APIDisposeThread *newThread_4 = new APIDisposeThread("post_0", "setup/wizard", globalAccess->userinfo->cookies, wizardObjcet, globalAccess->MainUrl() + "setup/wizard");
            connect(newThread_4, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState)
            {
                Q_UNUSED(_QObj);
                Q_UNUSED(_statusCode);
                if(_requestState)
                    emit userLoginCase(0x220);
                else
                    emit userLoginCase(0x120);
            });
            APIDisposeThread::runNewThread(newThread_4, "setup/wizard", false);
        });
    });
}

QString UserLoginModule::plaintextEncrypted(QString pwd)
{
    return pwd;
}

QString UserLoginModule::cipherDecryption(QString pwd)
{
    return pwd;
}
