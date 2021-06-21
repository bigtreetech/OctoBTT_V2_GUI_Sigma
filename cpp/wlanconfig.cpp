#include "../headers/wlanconfig.h"

WlanConfig::WlanConfig(QObject *parent) : QObject(parent)
{
    connect(this, &WlanConfig::getWlanNameSignal, this, [=](QString _wlanName){
        getWlanName(_wlanName);
    });
    connect(this, &WlanConfig::scanDevListSignal, this, [=](){
        scanDevList();
    });
    connect(this, &WlanConfig::getWiFilistSignal, this, [=](QString _wlanName){
        getWiFilist(_wlanName);
    });
    connect(this, &WlanConfig::connectWifiSignal, this, [=](QString _wlanName, QString wifiName, QString _password){
        connectWifi(_wlanName, wifiName, _password);
    });
    connect(this, &WlanConfig::forgetWiFiSignal, this, [=](QString _wlanName, QString wifiName){
        forgetWiFi(_wlanName, wifiName);
    });
    connect(this, &WlanConfig::closeDevSignal, this, [=](QString _wlanName){
        closeDev(_wlanName);
    });
    connect(this, &WlanConfig::openDevSignal, this, [=](QString _wlanName){
        openDev(_wlanName);
    });
    connect(this, &WlanConfig::GetNetworkInfoSignal, this, [=](){
        GetNetworkInfo();
    });
}

void WlanConfig::scanDevList()
{
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
    {
        scanDevList(CommandLine);
    });
    tempTerminal->SendCMD("sudo iwconfig | grep -E \"wlan[0-9]|ESSID|Tx-Power=\"");
}

void WlanConfig::getWlanName(QString _wlanName)
{
    wlanName = _wlanName;
}

void WlanConfig::ScanWPAList()
{
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
    {
        ScanWPAList(CommandLine);
    });
    tempTerminal->SendCMD("sudo wpa_cli -i " + wlanName + " list_network");
}

void WlanConfig::CreateNetwork()
{
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
    {
        CreateNetwork(CommandLine);
    });
    tempTerminal->SendCMD("wpa_cli -i " + wlanName + " add_network");
}

void WlanConfig::SetSSID(QString wifiName)
{
    CreateNetwork();
    connect(this, &WlanConfig::createWPAOver, this, [=](){
        this->disconnect(SIGNAL(createWPAOver()));
        int tempWAP_ID = newWPA_ID;

        Terminal *tempTerminal = new Terminal();
        tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));

        QString setSSIDcmd = QString("wpa_cli -i %1 set_network %2 ssid '\"%3\"'").arg(wlanName, QString::number(tempWAP_ID), wifiName);
        QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
        {
            SetSSID(CommandLine);
        });
        tempTerminal->SendCMD(setSSIDcmd);
    });
}

void WlanConfig::SetPassword(QString wifiName)
{
    Q_UNUSED(wifiName)
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QString setPSKcmd = QString("wpa_cli -i %1 set_network %2 psk '\"%3\"'").arg(wlanName).arg(newWPA_ID).arg(password);
    QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
    {
        SetPassword(CommandLine);
    });
    tempTerminal->SendCMD(setPSKcmd);
}

void WlanConfig::ConnectNetwork()
{
    Terminal *tempTerminal = new Terminal();
    QString connectCmd = QString("wpa_cli -i %1 enable_network %2 &&  wpa_cli -i %1 select_network %2 && wpa_cli -i %1 save_config").arg(wlanName).arg(newWPA_ID);
    tempTerminal->SendCMD(connectCmd);
    getWiFilist(wlanName);
    emit wifiPasswordOver();
}

void WlanConfig::GetNetworkInfo(QString _wlanName)
{
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QString getNetworkInfoCmd = "ifconfig | grep -E \"flags|inet|ether\"";
    QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
    {
        GetNetworkInfo(_wlanName, CommandLine);
    });
    tempTerminal->SendCMD(getNetworkInfoCmd);
}

void WlanConfig::removeNetwork(int tempWAP_ID)
{
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QString removeCmd = QString("wpa_cli -i %1 remove_network %2").arg(wlanName).arg(tempWAP_ID);
    QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
    {
        removeNetwork(CommandLine, tempWAP_ID);
    });
    tempTerminal->SendCMD(removeCmd);
}

void WlanConfig::selectWifi(int WPA_ID)
{
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QString selectCmd = QString("wpa_cli -i %1 select_network %2").arg(wlanName).arg(WPA_ID);
    tempTerminal->SendCMD(selectCmd);
}

void WlanConfig::closeDev(QString _wlanName)
{
    getWlanName(_wlanName);
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QString closeDevCmd = QString("sudo ifconfig %1 down").arg(wlanName);
    tempTerminal->SendCMD(closeDevCmd);
}

void WlanConfig::openDev(QString _wlanName)
{
    getWlanName(_wlanName);
    Terminal *tempTerminal = new Terminal();
    QString openDevCmd = QString("sudo ifconfig %1 up").arg(wlanName);
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    tempTerminal->SendCMD(openDevCmd);
}

void WlanConfig::getWiFilist(QString _wlanName)
{
    getWlanName(_wlanName);
    wlanStructHash[wlanName]->scanWifiList.clear();
    //获取list得到的列表
    ScanWPAList();
    connect(this, &WlanConfig::scanWPAListOver, this, [=](){
        this->disconnect(SIGNAL(scanWPAListOver()));
        //获取scan得到的列表
        ScanWPA();
        connect(this, &WlanConfig::scanWPAWifiOver, this, [=](){
            this->disconnect(SIGNAL(scanWPAWifiOver()));
            if(wlanStructHash[wlanName]->saveWPA_IDAndWifiList.isEmpty())
            {
                qDebug() << __LINE__ << __FILE__ << "error! 容器是空的";
                return;
            }

            GetNetworkInfo(wlanName);
            connect(this, &WlanConfig::ipInfoOver, this, [=](QString ipInfo){
                if(ipInfo == "")
                {
                    foreach(int tempWAP_ID, wlanStructHash[wlanName]->saveWPA_IDAndWifiList.keys())
                    {
                        foreach(QString tempWiFiName, wlanStructHash[wlanName]->saveWPA_IDAndWifiList[tempWAP_ID].keys())
                        {
                            //如果ip是空的, 但是显示的连接是CURRENT, 将其改正为[CONNECTING]
                            if(wlanStructHash[wlanName]->saveWPA_IDAndWifiList[tempWAP_ID][tempWiFiName] == "[CURRENT]")
                            {
                                wlanStructHash[wlanName]->saveWPA_IDAndWifiList[tempWAP_ID].remove(tempWiFiName);
                                wlanStructHash[wlanName]->saveWPA_IDAndWifiList[tempWAP_ID].insert(tempWiFiName, "[CONNECTING]");
                            }
                        }
                    }
                }
                foreach(int tempWAP_ID, wlanStructHash[wlanName]->saveWPA_IDAndWifiList.keys())
                {
                    QJsonObject wifiObject;
                    wifiObject.insert("WAP_ID", tempWAP_ID);
                    foreach(QString tempWiFiName, wlanStructHash[wlanName]->saveWPA_IDAndWifiList[tempWAP_ID].keys())
                    {
                        //判断list中是否有和scan相同的wifi, 有就添加这个wifi的强度和状态
                        if(wlanStructHash[wlanName]->scanWifiList.contains(tempWiFiName))
                        {
                            wifiObject.insert("wifi", tempWiFiName);
                            wifiObject.insert("signalLevel", wlanStructHash[wlanName]->scanWifiList[tempWiFiName]);
                            wifiObject.insert("wifiState", wlanStructHash[wlanName]->saveWPA_IDAndWifiList[tempWAP_ID][tempWiFiName]);
                            wifiInfoArray.append(wifiObject);
                        }
                    }
                }
                foreach(QString tempWifiName, wlanStructHash[wlanName]->scanWifiList.keys())
                {
                    QJsonObject wifiObject;
                    wifiObject.insert("WAP_ID", "");
                    wifiObject.insert("wifi", tempWifiName);
                    wifiObject.insert("signalLevel", wlanStructHash[wlanName]->scanWifiList[tempWifiName]);
                    wifiObject.insert("wifiState", "");
                    wifiInfoArray.append(wifiObject);
                }
                QJsonObject wifiListInfoObject;
                wifiListInfoObject.insert("wifiList", QJsonValue(wifiInfoArray));
                qDebug() << __LINE__ << __FILE__ << wifiListInfoObject;
                return QString(QJsonDocument(wifiListInfoObject).toJson());
            });
        });
    });
}

void WlanConfig::connectWifi(QString _wlanName, QString wifiName, QString _password)
{
    password = _password;
    getWlanName(_wlanName);
    bool flag = true;
    foreach(int tempWAP_ID, wlanStructHash[wlanName]->saveWPA_IDAndWifiList.keys())
    {
        if(wlanStructHash[wlanName]->saveWPA_IDAndWifiList[tempWAP_ID].contains(wifiName))
        {
            selectWifi(tempWAP_ID);
            flag = false;
        }
    }
    //设置要连接wifi名和密码
    if(flag)
    {
        SetSSID(wifiName);
    }
}

void WlanConfig::forgetWiFi(QString _wlanName, QString wifiName)
{
    getWlanName(_wlanName);
    foreach(int tempWAP_ID, wlanStructHash[wlanName]->saveWPA_IDAndWifiList.keys())
    {
        if(wlanStructHash[wlanName]->saveWPA_IDAndWifiList[tempWAP_ID].contains(wifiName))
        {
            removeNetwork(tempWAP_ID);
        }
    }
}

void WlanConfig::ScanWPA()
{
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
    {
        ScanWPA(CommandLine);
    });
    tempTerminal->SendCMD("sudo wpa_cli -i " + wlanName + " scan");
}

void WlanConfig::ScanWPA_Ressult()
{
    Terminal *tempTerminal = new Terminal();
    tempTerminal->disconnect(SIGNAL(CMD_Reply(QStringList)));
    QObject::connect(tempTerminal, &Terminal::CMD_Reply, this, [=](QStringList CommandLine)
    {
        ScanWPA_Ressult(CommandLine);
    });
    tempTerminal->SendCMD("wpa_cli -i " + wlanName + " scan_results");
}

void WlanConfig::scanDevList(QStringList Responses)
{
    wlanStructHash.clear();
    QString tempConnectWifiName;
    QString tempDevName;
    //截取连续两段都是wlan开头
    for(int i = 0; i < Responses.length() - 1; i++)
    {
        if(Responses[i].toLower().startsWith("wlan") && Responses[i + 1].toLower().startsWith("wlan"))
        {
            QStringList Analytical = Responses[i].split(" ", QString::SkipEmptyParts);
            if(Analytical.length() > 0)
            {
                if(Analytical[0].toLower().startsWith("wlan"))
                {
                    tempDevName = Analytical[0];
                    if(Analytical[3].toLower() == "essid:off/any")
                    {
                        tempConnectWifiName = "notConnect";
                    }
                    else
                    {
                        foreach(QString item ,Analytical)
                        {
                            if(item.toLower().startsWith("essid:"))
                            {
                                tempConnectWifiName = item.section(':', 0);
                                break;
                            }
                        }
                    }
                }
                WlanStructInfo *tempWlanStruct = new WlanStructInfo();
                tempWlanStruct->setProperty("connectWifiName", tempConnectWifiName);
                tempWlanStruct->setProperty("devName", tempDevName);
                tempWlanStruct->setProperty("power", 0);
                wlanStructHash.insert(tempDevName, tempWlanStruct);
                Responses.removeAt(i);
            }
        }
    }

    if(Responses[Responses.length() - 1].toLower().startsWith("wlan"))
    {
        QStringList Analytical = Responses[Responses.length() - 1].split(" ", QString::SkipEmptyParts);
        if(Analytical.length() > 0)
        {
            if(Analytical[0].toLower().startsWith("wlan"))
            {
                tempDevName = Analytical[0];
                if(Analytical[3].toLower() == "essid:off/any")
                {
                    tempConnectWifiName = "notConnect";
                }
                else
                {
                    foreach(QString item ,Analytical)
                    {
                        if(item.toLower().startsWith("essid:"))
                        {
                            tempConnectWifiName = item.section(':', 0);
                            break;
                        }
                    }
                }
            }
            WlanStructInfo *tempWlanStruct = new WlanStructInfo();
            tempWlanStruct->setProperty("connectWifiName", tempConnectWifiName);
            tempWlanStruct->setProperty("devName", tempDevName);
            tempWlanStruct->setProperty("power", 0);
            wlanStructHash.insert(tempDevName, tempWlanStruct);
            Responses.removeAt(Responses.length() - 1);
        }
    }
    //截取最后一段是wlan开头
    foreach(QString item, Responses)
    {
        QStringList Analytical = item.split(" ", QString::SkipEmptyParts);
        if(Analytical.length() > 0)
        {
            if(Analytical[0].toLower().startsWith("wlan"))
            {
                tempDevName = Analytical[0];
                if(Analytical[3].toLower() == "essid:off/any")
                {
                    tempConnectWifiName = "notConnect";
                }
                else
                {
                    foreach(QString item ,Analytical)
                    {
                        if(item.toLower().startsWith("essid:"))
                        {
                            tempConnectWifiName = item.section(':', 0);
                            break;
                        }
                    }
                }
            }
            else if(!Analytical[0].toLower().startsWith("eth0") && !Analytical[0].toLower().startsWith("lo"))
            {
                WlanStructInfo *tempWlanStruct = new WlanStructInfo();
                tempWlanStruct->setProperty("connectWifiName", tempConnectWifiName);
                tempWlanStruct->setProperty("devName", tempDevName);
                //功率数据可在此处提取
                foreach(QString item ,Analytical)
                {
                    if(item.toLower().startsWith("tx-power"))
                    {
                        tempWlanStruct->setProperty("power",item.split('=')[1].toInt());
                        break;
                    }
                }
                wlanStructHash.insert(tempDevName, tempWlanStruct);
            }
        }
    }
    foreach(QString tempWlanName, wlanStructHash.keys())
    {
        QJsonObject wlanObject;
        wlanObject.insert("dev", tempWlanName);
        if(wlanStructHash[tempWlanName]->property("power") == 0)
        {
            wlanObject.insert("devState", "close");
        }
        else {
            wlanObject.insert("devState", "open");
        }
        wlanObject.insert("power", wlanStructHash[tempWlanName]->property("power").toDouble());

        wlanInfoArray.append(wlanObject);
    }
    QJsonObject wlanInfoObject;
    wlanInfoObject.insert("devInfo", QJsonValue(wlanInfoArray));
    qDebug() << __LINE__ << __FILE__ << wlanInfoObject;
    emit devInfoSignal(QString(QJsonDocument(wlanInfoObject).toJson()));
}

void WlanConfig::ScanWPA(QStringList Responses)
{
    if(!wlanStructHash.contains(wlanName))
        return;
    if(Responses[0].toLower().trimmed() == "ok")
        ScanWPA_Ressult();
    else if(wlanStructHash[wlanName]->property("connectWifiName").toString() == "notConnect"){
        scanDevList();
    }
}

void WlanConfig::ScanWPA_Ressult(QStringList Responses)
{
    wlanStructHash[wlanName]->scanWifiList.clear();
    foreach(QString item , Responses)
    {
        QStringList ModuleStr = item.replace("\t"," ").split(" ",QString::SkipEmptyParts);
        if(!ModuleStr[0].toLower().startsWith("bssid") && ModuleStr.length() > 4)
        {
            if(ModuleStr[4] != "")
            {
                int tempInt;
                if(ModuleStr[2].toInt() <= -100)
                    tempInt = 0;
                else if(ModuleStr[2].toInt() >= -50)
                    tempInt = 2;
                else
                    tempInt = 1;

                wlanStructHash[wlanName]->scanWifiList.insert(ModuleStr[4], tempInt);
            }
        }
    }
    emit scanWPAWifiOver();
}

void WlanConfig::ScanWPAList(QStringList Responses)
{
    if(!wlanStructHash.contains(wlanName))
        return;

    foreach(QString item, Responses)
    {
        //获取WLAN network ID
        QStringList tempList = item.replace("\t", " ").split(" ", QString::SkipEmptyParts);
        if(tempList.length() > 3)
        {
            QHash<QString, QString> tempHash;
            tempHash.insert(tempList[1], tempList[3]);
            wlanStructHash[wlanName]->saveWPA_IDAndWifiList.insert(tempList[0].toInt(), tempHash);
        }
    }
    emit scanWPAListOver();
}

void WlanConfig::CreateNetwork(QStringList Responses)
{
    QHash<QString, QString> tempHash;
    tempHash.insert("", "");
    newWPA_ID = Responses[0].toInt();
    wlanStructHash[wlanName]->saveWPA_IDAndWifiList.insert(newWPA_ID, tempHash);

    emit createWPAOver();
}

void WlanConfig::SetSSID(QStringList Responses)
{
    if(Responses[0].toLower().trimmed() == "ok")
    {
        //设置密码
        SetPassword(wifiName);
    }
}

void WlanConfig::SetPassword(QStringList Responses)
{
    if(Responses[0].toLower().trimmed() == "ok")
    {
        ConnectNetwork();
    }
}

void WlanConfig::GetNetworkInfo(QString _wlanName, QStringList Responses)
{
    if(_wlanName != "")
    {
        bool tempFlag = false;
        foreach(QString item ,Responses)
        {
            if(tempFlag == true)
            {
                if(item.trimmed().toLower().startsWith("inet") && !item.trimmed().toLower().startsWith("inet6"))
                {
                    QStringList tempList = item.split(" ", QString::SkipEmptyParts);
                    emit ipInfoOver(tempList[1]);
                }
                else
                {
                    emit ipInfoOver();
                }
                tempFlag = false;
            }

            if(item.toLower().startsWith(_wlanName))
            {
                tempFlag = true;
            }
        }
    }
    else
    {
        foreach(QString item ,Responses)
        {
            QString replaceitem = item.replace("\t"," ");
            QStringList explore = replaceitem.split(" ",QString::SkipEmptyParts);
            if(explore[0].contains(QRegExp("(eth[0-9]+[0-9]*)|(lo)|(wlan[0-9]+[0-9]*)")))
            {
                networkInfoArray.append(explore[0]);
            }
            else if(explore[0].contains("inet6"))//IPv6
            {
                networkInfoArray.append(QString("IPv6:%1").arg(explore[1]));
            }
            else if(explore[0].contains("inet"))//IP
            {
                networkInfoArray.append(QString("IPv4:%1").arg(explore[1]));
                networkInfoArray.append(QString("IPv4 Netmask:%1").arg(explore[3]));
            }
            else if(explore[0].contains("ether"))//MAC
            {
                networkInfoArray.append(QString("MAC:%1").arg(explore[1]));
            }
        }
        qDebug() << __LINE__ << __FILE__ << networkInfoArray;
        QJsonObject networkInfoObject;
        networkInfoObject.insert("networkInfo", QJsonValue(networkInfoArray));
        emit networkInfoSingal(QString(QJsonDocument(networkInfoObject).toJson()));
    }
}

void WlanConfig::removeNetwork(QStringList Responses, int removeWAP_ID)
{
    if(Responses[0].toLower().trimmed() == "ok")
    {
        wlanStructHash[wlanName]->saveWPA_IDAndWifiList.remove(removeWAP_ID);
    }
}

