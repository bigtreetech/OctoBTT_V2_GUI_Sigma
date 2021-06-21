#ifndef WLANCONFIG_H
#define WLANCONFIG_H

#include <QDebug>
#include <QObject>
#include "terminal.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

/**************************************************
*   文件名: wlanconfig.h and wlanconfig.cpp
*   依赖文件: terminal.h and terminal.cpp
*   文件用途: wifi模块的相关操作
*
*   使用举例:
*      需要实例化WlanConfig对象
*      通过以下信号调用对应的方法
*      //获取dev列表 此信号中调用的方法还会抛出信号 devInfoSignal(QString devInfoJson) [抛出dev列表]
*      Q_INVOKABLE void scanDevListSignal();
*      wlanInfo (最外层名字 devInfo 参考)
*      {
*           devInfo:[
*                      {
*                           "dev": "wlan0",
*                           "devState": "open",
*                           "power": 20
*                      },
*                      {
*                           "dev": "wlan1",
*                           "devState": "open",    //open,close
*                           "power": 20
*                      }
*                   ]
*      }
*      ！注意获取wifi列表前(即读取wifiInfo)必须先调用信号getWlanNameSignal传递wlanName！
*      //获取wifi列表 此信号中调用的方法还会抛出新哈奥 wifiInfoSingal(QString wifiInfoJson) [抛出wifi列表]
*      Q_INVOKABLE void getWiFilistSignal(QString _wlanName);
*      wifiInfo (最外层名字 wifiList 参考)
*      {
*           wifiList:[
*                       {
*                           "WAP_ID": 0, //有记录则有数字
*                           "signalLevel": 1,
*                           "wifi": "biqu-m",
*                           "wifiState": ""
*                       },
*                       {
*                           "WAP_ID": "",
*                           "signalLevel": 1,   //0,1,2 差->良好
*                           "wifi": "biqu-m",
*                           "wifiState": ""
*                       }
*                    ]
*      }
*      //传递当前选择wlanName
*      Q_INVOKABLE void getWlanNameSignal(QString);
*      //连接wifi
*      Q_INVOKABLE void connectWifiSignal(QString _wlanName, QString wifiName, QString _password);
*      //遗忘wifi
*      Q_INVOKABLE void forgetWiFiSignal(QString _wlanName, QString wifiName);
*      //关闭当前dev
*      Q_INVOKABLE void closeDevSignal(QString _wlanName);
*      //开启关闭的dev
*      Q_INVOKABLE void openDevSignal(QString _wlanName);
*      //获取network信息 此信号中调用的方法还会抛出信号 networkInfoSingal(QString networkInfoJson) [抛出网络信息]
*      Q_INVOKABLE void GetNetworkInfoSignal();
*      networkInfo 最外层名字 networkInfo 参考
*      [
*           "eth0:",
*           "IPv4:192.168.0.42",
*           "IPv4 Netmask:255.255.255.0",
*           "IPv6:fe80::27a1:9b8e:576c:9e71",
*           "MAC:dc:a6:32:84:a8:2b",
*           "lo:",
*           "IPv4:127.0.0.1",
*           "IPv4 Netmask:255.0.0.0",
*           "IPv6:::1",
*           "wlan0:",
*           "MAC:dc:a6:32:84:a8:2c",
*           "wlan1:",
*           "MAC:e8:4e:06:34:f1:92"
*      ]
*
*      最后修改日期: 2021 / 6 / 17
**************************************************/

struct WlanStructInfo:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString devName MEMBER devName NOTIFY devNameChange);
    Q_PROPERTY(QString connectWifiName MEMBER connectWifiName NOTIFY connectWifiNameChange);
    Q_PROPERTY(int power MEMBER power NOTIFY powerChange);
public:
    QHash<QString, int> scanWifiList;     /* 保存扫描到的wifi名称以及网络状态 [0] 极差 [1]差 [2]良好 */
    QMap<int, QHash<QString, QString>> saveWPA_IDAndWifiList;   /* 保存WPA_ID wifi名 wifi使用状态 [CONNECTING](正在连接) [CURRENT](已连接) [DISABLED](未连接) [TAMP-DISABLED](连接失败) */

private:
    QString devName;   /* dev名称 */
    QString connectWifiName;    /* 已经连接的wifi名称 */
    int power;  /* dBm大小 如果为0 代表dev没有开启 */
public:
    WlanStructInfo()
    {
        connect(this,&WlanStructInfo::devNameChange,this,[=](){emit change();});
        connect(this,&WlanStructInfo::connectWifiNameChange,this,[=](){emit change();});
        connect(this,&WlanStructInfo::powerChange,this,[=](){emit change();});
    }
    ~WlanStructInfo()
    {

    }
signals:
    void devNameChange();
    void connectWifiNameChange();
    void powerChange();
    void change();
};

class WlanConfig : public QObject
{
    Q_OBJECT
public:
    explicit WlanConfig(QObject *parent = nullptr);
public:
    QHash<QString, WlanStructInfo*> wlanStructHash; /* wlanName 对应的结构体 */
    QStringList devNameList;              /* 保存扫描到的dev */
signals:
    void devInfoSignal(QString devInfoJson);
    void wifiInfoSingal(QString wifiInfoJson);
    void networkInfoSingal(QString networkInfoJson);
signals:
    //获取dev列表
    Q_INVOKABLE void scanDevListSignal();
    //获取wifi列表
    Q_INVOKABLE void getWiFilistSignal(QString _wlanName);
    //传递当前选择wlanName
    Q_INVOKABLE void getWlanNameSignal(QString _wlanName);
    //连接wifi
    Q_INVOKABLE void connectWifiSignal(QString _wlanName, QString wifiName, QString _password);
    //遗忘wifi
    Q_INVOKABLE void forgetWiFiSignal(QString _wlanName, QString wifiName);
    //关闭当前dev
    Q_INVOKABLE void closeDevSignal(QString _wlanName);
    //开启关闭的dev
    Q_INVOKABLE void openDevSignal(QString _wlanName);
    //获取networkInfo
    Q_INVOKABLE void GetNetworkInfoSignal();
private : signals:
    void scanWPAListOver();
    void scanWPAWifiOver();
    void wifiPasswordOver();
    void createWPAOver();
    void ipInfoOver(QString ipInfo = "");
private:
    //获得wlanName
    void getWlanName(QString _wlanName);
    //获取所有dev
    void scanDevList();
    //获取当前dev对应的wifi列表
    void getWiFilist(QString _wlanName);
    //连接wifi
    void connectWifi(QString _wlanName, QString wifiName, QString _password);
    //遗忘wifi
    void forgetWiFi(QString _wlanName, QString wifiName);
    //关闭当前dev
    void closeDev(QString _wlanName);
    //开启关闭的dev
    void openDev(QString _wlanName);
    //获取network信息
    void GetNetworkInfo(QString _wlanName = "");

    //判断能否连接wifi
    void ScanWPA();
    //获取能连接的wifi的信息
    void ScanWPA_Ressult();
    //获取dev的信息
    void ScanWPAList();
    //创建新的WPA_ID
    void CreateNetwork();
    void SetSSID(QString wifiName);
    void SetPassword(QString wifiName);
    //连接指定WAP_ID网络
    void ConnectNetwork();
    //删除指定WPA_ID网络
    void removeNetwork(int removeWAP_ID);
    //选择已经存储的WPA_ID网络
    void selectWifi(int WPA_ID);
    void scanDevList(QStringList Responses);
    void ScanWPA(QStringList Responses);
    void ScanWPA_Ressult(QStringList Responses);
    void ScanWPAList(QStringList Responses);
    void CreateNetwork(QStringList Responses);
    void SetSSID(QStringList Responses);
    void SetPassword(QStringList Responses);
    void GetNetworkInfo(QString wlanName, QStringList Responses);
    void removeNetwork(QStringList Responses, int removeWAP_ID);
private:
    int newWPA_ID;
    QString wlanName;
    QString wifiName;
    QString password;

    QJsonArray wlanInfoArray;
    QJsonArray wifiInfoArray;
    QJsonArray networkInfoArray;
};

#endif // WLANCONFIG_H
