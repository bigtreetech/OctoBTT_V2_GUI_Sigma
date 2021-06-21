#include "../headers/accesscontrol.h"

AccessControl::AccessControl()
{
    connect(userinfo,&UserInfo::change,this,[=](){emit userinfoChange();});

    GetACRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    loginTimer = new QTimer(this);
    loginTimer->setSingleShot(true);
    loginTimer->setInterval (2000);
    //连接超时信号
    connect(loginTimer, &QTimer::timeout, this, [=](){
        qDebug() << __FILE__ << __LINE__ << ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
        emit CommunicationStateChange(false,false);
//        loginTimer->disconnect(SIGNAL(timeout()));
    });
}

AccessControl::~AccessControl()
{
    userinfo->~UserInfo();
}

//login
void AccessControl::acLogin()
{
    GetACRequest.setUrl(QUrl(MainUrl() + "login"));//http://IP/api/login
    QJsonObject TemperatureJsonObj;

    if(userinfo->property("uid").toString() != "" && userinfo->property("uid").toString() != "guests")
    {
        TemperatureJsonObj.insert("user", userinfo->property("uid").toString());
        TemperatureJsonObj.insert("pass", userinfo->property("pwd").toString());
    }

    TemperatureJsonObj.insert("remember", "remember me");
    TemperatureJsonObj.insert("passive", true);//被动模式登录，如果免登录依旧能够获取session///需要允许被动登录才可以获得免登录方式
    QJsonDocument _SenderJson;
    _SenderJson.setObject(TemperatureJsonObj);

    ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    connect(ACManager, &QNetworkAccessManager::finished, this, &AccessControl::LoginReply);
    //连接超时信号
//    qDebug() << connect(loginTimer, &QTimer::timeout, this, [=](){
//        qDebug() << ACManager->disconnect(SIGNAL(finished(QNetworkReply*))) << __FILE__ << __LINE__ ;
//        emit CommunicationStateChange(false,false);
////        loginTimer->disconnect(SIGNAL(timeout()));
//    }) << __FILE__ << __LINE__ ;
//    loginTimer->start(2000);
    loginTimer->start ();

    ACManager->post(GetACRequest, _SenderJson.toJson());
}

void AccessControl::LoginReply(QNetworkReply *reply)
{
    loginTimer->stop ();
//    qDebug() << loginTimer->disconnect(SIGNAL(timeout())) << __FILE__ << __LINE__ ;
    bool _return = false;
    if(reply->error() == QNetworkReply::NoError)
    {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //无论状态码是什么, 都可以获取cookies
        userinfo->cookies = ACManager->cookieJar()->cookiesForUrl(QUrl(MainUrl() + "login"));
        if(statusCode == 200)//Have receive
        {
            if(userinfo->property("uid").toString() == "" || userinfo->property("uid").toString() == "guests")
            {
                userinfo->setProperty("admin",true);
                userinfo->setProperty("login",false);
                userinfo->setProperty("onlineUid",userinfo->property("uid").toString());
                userinfo->API_Key = QByteArray();
                userinfo->session = QByteArray();
//                userinfo = new UserInfo(userinfo->property("uid").toString(),"");
                userinfo->cookies = ACManager->cookieJar()->cookiesForUrl(QUrl(MainUrl() + "login"));
                _return = false;
            }
            else
            {
                //创建接收对象
                QByteArray replyArray = reply->readAll();
                QJsonDocument loadDoc(QJsonDocument::fromJson(replyArray));
                QJsonObject QObj = loadDoc.object();
                //获取登录反馈内容
                QList<QJsonValue> result = SearchJsonValue(LoginMsg , QObj);//{{"active"},{"admin"},{"apikey"},{"name"},{"session"}};
                userinfo->setProperty("login" , result[0].toBool());
                userinfo->setProperty("admin" , result[1].toBool());
                userinfo->API_Key = result[2].toString().toUtf8();//user api key
                userinfo->setProperty("onlineUid" , result[3].toString());
                userinfo->session = result[4].toString().toUtf8();
                _return = true;
            }

        }
    }
    ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    emit CommunicationStateChange(_return,reply->error() == QNetworkReply::NoError);
}

//logout
void AccessControl::acLogout()
{
    if(userinfo->property("uid").toString() != "" && userinfo->property("uid").toString() != "guests")
    {
        GetACRequest.setUrl(QUrl(MainUrl() + "logout"));//http://IP/api/logout
        ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
        connect(ACManager,&QNetworkAccessManager::finished,this,&AccessControl::LogoutReply);

        ACManager->post(GetACRequest,QByteArray());
    }
    else
    {
        emit CommunicationStateChange(false,true);
    }
}

void AccessControl::LogoutReply(QNetworkReply *reply)
{
    loginTimer->disconnect(SIGNAL(timeout()));
    bool _return=false;
    if(reply->error() == QNetworkReply::NoError)
    {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(statusCode == 200)//Have receive
        {
            //重置登录参数
//            userinfo = new UserInfo();
//            userinfo->setProperty("uid","");
//            userinfo->setProperty("pwd","");
            userinfo->setProperty("admin",false);
            userinfo->setProperty("login",false);
            userinfo->setProperty("onlineUid","");
            userinfo->API_Key = QByteArray();
            userinfo->session = QByteArray();
            userinfo->cookies = QList<QNetworkCookie>();

            _return = true;
        }
    }
    ACManager->disconnect(SIGNAL(finished(QNetworkReply*)));
    emit CommunicationStateChange(_return,reply->error() == QNetworkReply::NoError);
}


//Function
//Find_List_Name : Retrieved List Sequence --> [Grandma,Mother,Me]
//_QObj:Retrieved Retrieved Json Objecet
QJsonValue AccessControl::SearchJsonValue(QList<QString> Find_List_Name , QJsonObject _QObj)
{
    QJsonValue returnValue;
    QList<QString> SearchLink ;
    SearchLink << Find_List_Name;
    if(SearchLink.count() > 1)
    {
        QString MotherKey = SearchLink[0];
        SearchLink.removeFirst();
        if(MotherKey.indexOf("*") >= 0)//wildcard
        {
            QJsonArray ArrayValue;
            for(int i = 0 ; i <= 9 ; i++)
            {
                QString TempMotherKey = MotherKey;
                if(_QObj.find(TempMotherKey.replace("*",QString::number(i))).value().isUndefined())continue;
                //if(QSysInfo::productType() != "raspbian"){qDebug()<<_QObj.find(TempMotherKey.replace("*",QString::number(i))).value().isUndefined();};
                QJsonValue data = SearchJsonValue(SearchLink , _QObj.constFind(TempMotherKey.replace("*",QString::number(i))).value().toObject());
                ArrayValue.append(data);
            }
            returnValue = ArrayValue;
        }
        else
            returnValue = SearchJsonValue(SearchLink , _QObj.constFind(MotherKey).value().toObject());
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
QList<QJsonValue> AccessControl::SearchJsonValue(QList<QList<QString>> Find_List_Names , QJsonObject _QObj)
{
    QList<QJsonValue> _returnValue;
    foreach(QList<QString> item , Find_List_Names)
    {
        _returnValue.append(SearchJsonValue(item,_QObj));
    }
    return _returnValue;
}
