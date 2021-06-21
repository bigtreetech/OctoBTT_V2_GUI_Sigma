#include "../headers/functionmodule.h"

FunctionModule::FunctionModule(QObject *parent) : QObject(parent)
{
    connect(this, &FunctionModule::getFileInfoSignal, this, [=](QStringList tempList){
        FunctionModule::getFileInfo(tempList);
    });
    connect(this, &FunctionModule::postSelectTheFileSignal, this, [=](FileInfo* tempStruct){
        if(tempStruct->property("fileResource").toString().startsWith("http://"))
            FunctionModule::postSelectTheFile(tempStruct);
    });
    connect(this, &FunctionModule::postPrintFileCommandSignal, this, [=](FileInfo* tempFileStruct,QString tempString){
        FunctionModule::postPrintFileCommand(tempFileStruct, tempString);
    });
    connect(this, &FunctionModule::postUploadFileToOctoprintSignal, this, [=](FolderInfo* tempFolderStruct, FileInfo* tempFileStruct){
        FunctionModule::postUploadFileToOctoprint(tempFolderStruct->property("folderPath").toString(), tempFileStruct->property("filePath").toString());
    });
    connect(this, &FunctionModule::downloadFileSignal, this, [=](FileInfo* tempFileStruct, FolderInfo* tempFolderStruct){
        FunctionModule::downloadFile(tempFileStruct, tempFolderStruct->property("folderPath").toString());
    });
    connect(this, &FunctionModule::delFileSignal, this, [=](FileInfo* tempFileStruct){
        if(tempFileStruct->property("fileResource").toString().startsWith("http://"))
        {
            FunctionModule::delFile(tempFileStruct->property("fileResource").toString());
        }
        else
        {
            FunctionModule::delFile(tempFileStruct->property("filePath").toString());
        }
    });
    connect(this, &FunctionModule::delFolderSignal, this, [=](FolderInfo* tempFolderStruct){
        if(tempFolderStruct->property("folderResource").toString().startsWith("http://"))
        {
            FunctionModule::delFile(tempFolderStruct->property("folderResource").toString());
        }
        else
        {
            FunctionModule::delFile(tempFolderStruct->property("folderPath").toString());
        }
    });
    connect(this, &FunctionModule::createFolderSignal, this, [=](FolderInfo* tempFolderStruct, QString folderName){
        FunctionModule::createFolder(tempFolderStruct, folderName);
    });
    connect(this, &FunctionModule::sentGcodeCommandSignal, this, [=](QString tempString){
        FunctionModule::sentGcodeCommand(tempString);
    });
}

void FunctionModule::getFileInfo(QStringList usbStringList)
{
    Q_UNUSED(usbStringList)
    APIDisposeThread *newThread = new APIDisposeThread("get", "files", globalAccess->userinfo->cookies, QJsonObject(), globalAccess->MainUrl() + "files" + "?recursive=true");
    APIDisposeThread::runNewThread(newThread, "files", false);
    //获取所有文件的信息
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        Q_UNUSED(_statusCode)
        Q_UNUSED(_requestState)
        FolderInfo* tempFolderStruct = new FolderInfo();
        FolderInfo* tempLocalFolderStruct = new FolderInfo();
        FolderInfo* tempSDcardFolderStruct = new FolderInfo();
        tempFolderStruct->setProperty("folderName" , "files");
        tempFolderStruct->setProperty("folderPath", "");
        tempLocalFolderStruct->setProperty("folderName" , "Local");
        tempLocalFolderStruct->setProperty("folderPath" , "");
        tempSDcardFolderStruct->setProperty("folderName" , "Board");
        tempSDcardFolderStruct->setProperty("folderPath" , "");
        if(_QObj.contains("files"))
        {
            QJsonValue filesValue = _QObj.value("files");
            if(filesValue.isArray())
            {
                QJsonArray fileArray = filesValue.toArray();
                for(int i = 0; i < fileArray.count(); i++)
                {
                    bool childrenFlag = false;
                    //true为local false为sdcard
                    bool SDcardOrLocal = true;
                    QString tempFileDisplay;
                    QJsonValue tempValue = fileArray.at(i);
                    if(tempValue.isObject())
                    {
                        QJsonObject tempObject = tempValue.toObject();
                        if(tempObject.contains("origin"))
                        {
                            QJsonValue originValue = tempObject.value("origin");
                            if(originValue.isString())
                            {
                                if(originValue.toString() == "local")
                                {
                                    SDcardOrLocal = true;
                                }
                                else if(originValue.toString() == "sdcard")
                                {
                                    SDcardOrLocal = false;
                                }
                            }
                        }

                        if(SDcardOrLocal)
                        {
                            if(tempObject.contains("children"))
                            {
                                childrenFlag = true;
                                FolderInfo* tempChildrenFolderStruct = getChildrenFileInfo(tempObject);
                                tempLocalFolderStruct->nextNode.append(tempChildrenFolderStruct);
                            }
                        }
                        else
                        {
                            if(tempObject.contains("children"))
                            {
                                childrenFlag = true;
                                FolderInfo* tempChildrenFolderStruct = getChildrenFileInfo(tempObject);
                                tempSDcardFolderStruct->nextNode.append(tempChildrenFolderStruct);
                            }
                        }
                        if(!childrenFlag)
                        {
                            FileInfo* tempFileStruct = new FileInfo();
                            if(tempObject.contains("display"))
                            {
                                QJsonValue displayValue = tempObject.value("display");
                                if(displayValue.isString())
                                {
                                    tempFileStruct->setProperty("fileDisplayName" , displayValue.toString());
                                    tempFileDisplay = displayValue.toString();
                                }
                            }
                            if(tempObject.contains("gcodeAnalysis"))
                            {
                                QJsonValue gcodeAnalysisValue = tempObject.value("gcodeAnalysis");
                                if(gcodeAnalysisValue.isObject())
                                {
                                    QJsonObject gcodeAnalysisObject = gcodeAnalysisValue.toObject();
                                    if(gcodeAnalysisObject.contains("dimensions"))
                                    {
                                        QJsonValue dimensionsValue = gcodeAnalysisObject.value("dimensions");
                                        if(dimensionsValue.isObject())
                                        {
                                            QJsonObject dimensionsObject = dimensionsValue.toObject();
                                            if(dimensionsObject.contains("depth"))
                                            {
                                                QJsonValue depthValue = dimensionsObject.value("depth");
                                                if(depthValue.isDouble())
                                                {
                                                    tempFileStruct->setProperty("modelDepth" , QString("%1").arg(QString::number(depthValue.toDouble(), 'd', 1)));
                                                }
                                            }
                                            if(dimensionsObject.contains("height"))
                                            {
                                                QJsonValue heightValue = dimensionsObject.value("height");
                                                if(heightValue.isDouble())
                                                {
                                                    tempFileStruct->setProperty("modelHeight" , QString("%1").arg(QString::number(heightValue.toDouble(), 'd', 1)));
                                                }
                                            }
                                            if(dimensionsObject.contains("width"))
                                            {
                                                QJsonValue widthValue = dimensionsObject.value("width");
                                                if(widthValue.isDouble())
                                                {
                                                    tempFileStruct->setProperty("modelWidth" , QString("%1").arg(QString::number(widthValue.toDouble(), 'd', 1)));
                                                }
                                            }
                                        }
                                    }
                                    if(gcodeAnalysisObject.contains("estimatedPrintTime"))
                                    {
                                        QJsonValue estimatedPrintTimeValue = gcodeAnalysisObject.value("estimatedPrintTime");
                                        if(estimatedPrintTimeValue.isDouble())
                                        {
                                            QString tempDay = QString::number((int)estimatedPrintTimeValue.toDouble() / (3600 * 24));
                                            QString tempHour = QString::number((int)estimatedPrintTimeValue.toDouble() % (3600 * 24) / 3600);
                                            int tempInt = ((int)estimatedPrintTimeValue.toDouble()  % (3600 * 24 * 3600) % 60) > 0 ? 1 : 0;
                                            QString tempMinute = QString::number(((int)estimatedPrintTimeValue.toDouble()  % (3600 * 24) % 3600 / 60) + tempInt);
                                            if(tempDay.toInt() > 0)
                                            {
                                                tempFileStruct->setProperty("estimatedPrintTime" , QString("%1 %2:%3").arg(tempDay + tr("day")).arg(tempHour, 2, QChar('0')).arg(tempMinute, 2, QChar('0')));
                                            }
                                            else if(tempHour.toInt() > 0)
                                            {
                                                tempFileStruct->setProperty("estimatedPrintTime" , QString("%1:%2").arg(tempHour, 2, QChar('0')).arg(tempMinute, 2, QChar('0')));
                                            }
                                            else
                                            {
                                                tempFileStruct->setProperty("estimatedPrintTime" , QString("%1").arg(tempMinute + tr("minute"), 2, QChar('0')));
                                            }
                                        }
                                        else
                                            tempFileStruct->setProperty("estimatedPrintTime" , "");
                                    }
                                }
                            }
                            if(tempObject.contains("name"))
                            {
                                QJsonValue nameValue = tempObject.value("name");
                                if(nameValue.isString())
                                {
                                    tempFileStruct->setProperty("fileName" , nameValue.toString());
                                }
                            }
                            if(tempObject.contains("path"))
                            {
                                QJsonValue pathValue = tempObject.value("path");
                                if(pathValue.isString())
                                {
                                    tempFileStruct->setProperty("filePath", pathValue.toString());
                                }
                            }
                            if(tempObject.contains("refs"))
                            {
                                QJsonValue refsValue = tempObject.value("refs");

                                if(refsValue.isObject())
                                {
                                    QJsonObject refsObject = refsValue.toObject();
                                    if(refsObject.contains("download"))
                                    {
                                        QJsonValue downloadValue = refsObject.value("download");
                                        if(downloadValue.isString())
                                        {
                                            tempFileStruct->setProperty("fileDownload" , downloadValue.toString());
                                        }
                                    }
                                    if(refsObject.contains("resource"))
                                    {
                                        QJsonValue resourceValue = refsObject.value("resource");
                                        if(resourceValue.isString())
                                        {
                                            tempFileStruct->setProperty("fileResource", resourceValue.toString());
                                            tempFileStruct->setProperty("fileClass" , resourceValue.toString().section('.', -1));
                                        }
                                    }
                                }
                            }
                            if(tempObject.contains("size"))
                            {
                                QJsonValue sizeValue = tempObject.value("size");
                                if(sizeValue.isDouble())
                                {
                                    QString tempGB = QString(QString::number(sizeValue.toDouble() / (1024 * 1024 * 1024), 'd', 1));
                                    if(tempGB.toDouble() > 0)
                                        tempFileStruct->setProperty("fileSize" , QString("%1").arg(tempGB + tr("GB")));
                                    else
                                    {
                                        QString tempMB = QString::number((sizeValue.toDouble() / (1024 * 1024)), 'd', 1);
                                        if(tempMB.toDouble() > 0)
                                            tempFileStruct->setProperty("fileSize" , QString("%1").arg(tempMB + tr("MB")));
                                        else
                                        {
                                            QString tempKB = QString::number(sizeValue.toDouble() / 1024, 'd', 1);
                                            if(tempKB.toDouble() > 0)
                                                tempFileStruct->setProperty("fileSize" , QString("%1").arg(tempKB + tr("KB"), 'd', 1));
                                            else
                                            {
                                                if(sizeValue.toDouble() > 0)
                                                    tempFileStruct->setProperty("fileSize" , QString("%1").arg("1" + tr("KB")));
                                                else
                                                    tempFileStruct->setProperty("fileSize" , QString("%1").arg("0" + tr("KB")));
                                            }
                                        }
                                    }
                                }
                            }
                            if(SDcardOrLocal)
                                tempLocalFolderStruct->fileList.append(tempFileStruct);
                            else
                                tempSDcardFolderStruct->fileList.append(tempFileStruct);
                        }
                    }
                }
            }
        }
        if(_QObj.contains("free"))
        {
            QJsonValue freeValue = _QObj.value("free");
            if(freeValue.isDouble())
            {
                QString tempGB = QString(QString::number(freeValue.toDouble() / (1024 * 1024 * 1024), 'd', 1));
                if(tempGB.toDouble() > 0)
                {
                    tempFolderStruct->setProperty("filesFree" , QString("%1").arg(tempGB + tr("GB")));
                }
                else
                {
                    QString tempMB = QString::number((freeValue.toDouble() / (1024 * 1024)), 'd', 1);
                    if(tempMB.toDouble() > 0)
                        tempFolderStruct->setProperty("filesFree" , QString("%1").arg(tempMB + tr("MB")));
                    else
                    {
                        QString tempKB = QString::number(freeValue.toDouble() / 1024, 'd', 1);
                        if(tempKB.toDouble() > 0)
                        {
                            tempFolderStruct->setProperty("filesFree" , QString("%1").arg(tempKB + tr("KB"), 'd', 1));
                        }
                        else
                        {
                            if(freeValue.toDouble() > 0)
                                tempFolderStruct->setProperty("filesFree" , QString("%1").arg("1" + tr("KB")));
                            else
                                tempFolderStruct->setProperty("filesFree" , QString("%1").arg("0" + tr("KB")));
                        }
                    }
                }
            }
        }
        if(_QObj.contains("total"))
        {
            QJsonValue totalValue = _QObj.value("total");
            if(totalValue.isDouble())
            {
                QString tempGB = QString(QString::number(totalValue.toDouble() / (1024 * 1024 * 1024), 'd', 1));
                if(tempGB.toDouble() > 0)
                {
                    tempFolderStruct->setProperty("filesTotal" , QString("%1").arg(tempGB + tr("GB")));

                }
                else
                {
                    QString tempMB = QString::number((totalValue.toDouble() / (1024 * 1024)), 'd', 1);
                    if(tempMB.toDouble() > 0)
                        tempFolderStruct->setProperty("filesTotal" , QString("%1").arg(tempMB + tr("MB")));
                    else
                    {
                        QString tempKB = QString::number(totalValue.toDouble() / 1024, 'd', 1);
                        if(tempKB.toDouble() > 0)
                        {
                            tempFolderStruct->setProperty("filesTotal" , QString("%1").arg(tempKB + tr("KB"), 'd', 1));
                        }
                        else
                        {
                            if(totalValue.toDouble() > 0)
                                tempFolderStruct->setProperty("filesTotal" , QString("%1").arg("1" + tr("KB")));
                            else
                                tempFolderStruct->setProperty("filesTotal" , QString("%1").arg("0" + tr("KB")));
                        }
                    }
                }
            }
        }
        //        QStringList tempStringList;
        //                tempStringList.append("C:/Users/BiQu_/Desktop/tempFolder/iU");
        //                tempStringList.append("C:/Users/BiQu_/Desktop/tempFolder/connectionDetection");
        //                tempStringList.append("C:/Users/BiQu_/Desktop/tempFolder/webSocketDemo2");
        //        QList<FolderInfo> tempFolderInfoList = getUsbFileInfo(tempStringList);
        QList<FolderInfo*> tempFolderInfoList = getUsbFileInfo(usbStringList);
        tempFolderStruct->nextNode.append(tempLocalFolderStruct);
        tempFolderStruct->nextNode.append(tempSDcardFolderStruct);
        foreach(FolderInfo* tempStrut, tempFolderInfoList)
        {
            tempFolderStruct->nextNode.append(tempStrut);
        }
        emit sentFolderInfoSignal(tempFolderStruct);
        //test
//        showFileInfoItem(tempFolderStruct);
    });
}

FolderInfo* FunctionModule::getChildrenFileInfo(QJsonObject tempObject)
{
    FolderInfo* tempFolderStruct = new FolderInfo();
    if(tempObject.contains("refs"))
    {
        QJsonValue refsValue = tempObject.value("refs");
        if(refsValue.isObject())
        {
            QJsonObject refsObject = refsValue.toObject();
            if(refsObject.contains("resource"))
            {
                QJsonValue resourceValue = refsObject.value("resource");
                if(resourceValue.isString())
                {
                    tempFolderStruct->property("folderResource").toString() = resourceValue.toString();
                    tempFolderStruct->setProperty("folderName" , resourceValue.toString().section('/', -1));
                }
            }
        }
    }
    if(tempObject.contains("size"))
    {
        QJsonValue sizeValue = tempObject.value("size");
        if(sizeValue.isDouble())
        {
            QString tempGB = QString(QString::number(sizeValue.toDouble() / (1024 * 1024 * 1024), 'd', 1));
            if(tempGB.toDouble() > 0)
                tempFolderStruct->setProperty("folderSize" , QString("%1").arg(tempGB + tr("GB")));
            else
            {
                QString tempMB = QString::number((sizeValue.toDouble() / (1024 * 1024)), 'd', 1);
                if(tempMB.toDouble() > 0)
                    tempFolderStruct->setProperty("folderSize" , QString("%1").arg(tempMB + tr("MB")));
                else
                {
                    QString tempKB = QString::number(sizeValue.toDouble() / 1024, 'd', 1);
                    if(tempKB.toDouble() > 0)
                        tempFolderStruct->setProperty("folderSize" , QString("%1").arg(tempKB + tr("KB"), 'd', 1));
                    else
                    {
                        if(sizeValue.toDouble() > 0)
                            tempFolderStruct->setProperty("folderSize" , QString("%1").arg("1" + tr("KB")));
                        else
                            tempFolderStruct->setProperty("folderSize" , QString("%1").arg("0" + tr("KB")));
                    }
                }
            }
        }
    }
    if(tempObject.contains("path"))
    {
        QJsonValue pathValue = tempObject.value("path");
        if(pathValue.isString())
        {
            tempFolderStruct->property("folderPath").toString() = pathValue.toString();
        }
    }
    if(tempObject.contains("children"))
    {
        QJsonValue childrenValue = tempObject.value("children");
        if(childrenValue.isArray())
        {
            QJsonArray childrenArray = childrenValue.toArray();
            for(int i = 0; i < childrenArray.count(); i++)
            {
                FileInfo* tempFileStruct = new FileInfo();
                bool childrenFlag = false;
                QString tempFileDisplay;
                QJsonValue childrenValue = childrenArray.at(i);
                if(childrenValue.isObject())
                {
                    QJsonObject childrenObject = childrenValue.toObject();
                    if(childrenObject.contains("children"))
                    {
                        childrenFlag = true;
                        FolderInfo* tempChildrenFolderStruct = getChildrenFileInfo(childrenObject);
                        tempFolderStruct->nextNode.append(tempChildrenFolderStruct);
                    }
                    if(!childrenFlag)
                    {
                        if(childrenObject.contains("display"))
                        {
                            QJsonValue displayValue = childrenObject.value("display");
                            if(displayValue.isString())
                            {
                                tempFileStruct->setProperty("fileDisplayName" , displayValue.toString());
                                tempFileDisplay = displayValue.toString();
                            }
                        }
                        if(childrenObject.contains("gcodeAnalysis"))
                        {
                            QJsonValue gcodeAnalysisValue = childrenObject.value("gcodeAnalysis");
                            if(gcodeAnalysisValue.isObject())
                            {
                                QJsonObject gcodeAnalysisObject = gcodeAnalysisValue.toObject();
                                if(gcodeAnalysisObject.contains("dimensions"))
                                {
                                    QJsonValue dimensionsValue = gcodeAnalysisObject.value("dimensions");
                                    if(dimensionsValue.isObject())
                                    {
                                        QJsonObject dimensionsObject = dimensionsValue.toObject();
                                        if(dimensionsObject.contains("depth"))
                                        {
                                            QJsonValue depthValue = dimensionsObject.value("depth");
                                            if(depthValue.isDouble())
                                            {
                                                tempFileStruct->setProperty("modelDepth" , QString("%1").arg(QString::number(depthValue.toDouble(), 'd', 1)));
                                            }
                                        }
                                        if(dimensionsObject.contains("height"))
                                        {
                                            QJsonValue heightValue = dimensionsObject.value("height");
                                            if(heightValue.isDouble())
                                            {
                                                tempFileStruct->setProperty("modelHeight" , QString("%1").arg(QString::number(heightValue.toDouble(), 'd', 1)));
                                            }
                                        }
                                        if(dimensionsObject.contains("width"))
                                        {
                                            QJsonValue widthValue = dimensionsObject.value("width");
                                            if(widthValue.isDouble())
                                            {
                                                tempFileStruct->setProperty("modelWidth" , QString("%1").arg(QString::number(widthValue.toDouble(), 'd', 1)));
                                            }
                                        }
                                    }
                                }
                                if(gcodeAnalysisObject.contains("estimatedPrintTime"))
                                {
                                    QJsonValue estimatedPrintTimeValue = gcodeAnalysisObject.value("estimatedPrintTime");
                                    if(estimatedPrintTimeValue.isDouble())
                                    {
                                        QString tempDay = QString::number((int)estimatedPrintTimeValue.toDouble() / (3600 * 24));
                                        QString tempHour = QString::number((int)estimatedPrintTimeValue.toDouble() % (3600 * 24) / 3600);
                                        int tempInt = ((int)estimatedPrintTimeValue.toDouble()  % (3600 * 24 * 3600) % 60) > 0 ? 1 : 0;
                                        QString tempMinute = QString::number(((int)estimatedPrintTimeValue.toDouble()  % (3600 * 24) % 3600 / 60) + tempInt);
                                        if(tempDay.toInt() > 0)
                                        {
                                            tempFileStruct->setProperty("estimatedPrintTime" , QString("%1 %2:%3").arg(tempDay + tr("day")).arg(tempHour, 2, QChar('0')).arg(tempMinute, 2, QChar('0')));
                                        }
                                        else if(tempHour.toInt() > 0)
                                        {
                                            tempFileStruct->setProperty("estimatedPrintTime" , QString("%1:%2").arg(tempHour, 2, QChar('0')).arg(tempMinute, 2, QChar('0')));
                                        }
                                        else
                                        {
                                            tempFileStruct->setProperty("estimatedPrintTime" , QString("%1").arg(tempMinute + tr("minute"), 2, QChar('0')));
                                        }
                                    }
                                    else
                                        tempFileStruct->setProperty("estimatedPrintTime" , "");
                                }
                            }
                        }
                        if(childrenObject.contains("name"))
                        {
                            QJsonValue nameValue = childrenObject.value("name");
                            if(nameValue.isString())
                            {
                                tempFileStruct->setProperty("fileName" , nameValue.toString());
                            }
                        }
                        if(childrenObject.contains("path"))
                        {
                            QJsonValue pathValue = childrenObject.value("path");
                            if(pathValue.isString())
                            {
                                tempFileStruct->setProperty("filePath" , pathValue.toString());
                            }
                        }
                        if(childrenObject.contains("refs"))
                        {
                            QJsonValue refsValue = childrenObject.value("refs");

                            if(refsValue.isObject())
                            {
                                QJsonObject refsObject = refsValue.toObject();
                                if(refsObject.contains("download"))
                                {
                                    QJsonValue downloadValue = refsObject.value("download");
                                    if(downloadValue.isString())
                                    {
                                        tempFileStruct->setProperty("fileDownload" , downloadValue.toString());
                                    }
                                }
                                if(refsObject.contains("resource"))
                                {
                                    QJsonValue resourceValue = refsObject.value("resource");
                                    if(resourceValue.isString())
                                    {
                                        tempFileStruct->setProperty("fileResource", resourceValue.toString());
                                        tempFileStruct->setProperty("fileClass" , resourceValue.toString().section('.', -1));
                                    }
                                }
                            }
                        }
                        if(childrenObject.contains("size"))
                        {
                            QJsonValue sizeValue = childrenObject.value("size");
                            if(sizeValue.isDouble())
                            {
                                QString tempGB = QString(QString::number(sizeValue.toDouble() / (1024 * 1024 * 1024), 'd', 1));
                                if(tempGB.toDouble() > 0)
                                    tempFileStruct->setProperty("fileSize" , QString("%1").arg(tempGB + tr("GB")));
                                else
                                {
                                    QString tempMB = QString::number((sizeValue.toDouble() / (1024 * 1024)), 'd', 1);
                                    if(tempMB.toDouble() > 0)
                                        tempFileStruct->setProperty("fileSize" , QString("%1").arg(tempMB + tr("MB")));
                                    else
                                    {
                                        QString tempKB = QString::number(sizeValue.toDouble() / 1024, 'd', 1);
                                        if(tempKB.toDouble() > 0)
                                            tempFileStruct->setProperty("fileSize" , QString("%1").arg(tempKB + tr("KB"), 'd', 1));
                                        else
                                        {
                                            if(sizeValue.toDouble() > 0)
                                                tempFileStruct->setProperty("fileSize" , QString("%1").arg("1" + tr("KB")));
                                            else
                                                tempFileStruct->setProperty("fileSize" , QString("%1").arg("0" + tr("KB")));
                                        }
                                    }
                                }
                            }
                        }
                        tempFolderStruct->fileList.append(tempFileStruct);
                    }
                }
            }
        }
    }
    return tempFolderStruct;
}

FolderInfo* FunctionModule::getUsbChildrenFileInfo(QString path)
{
    FolderInfo* tempStruct = new FolderInfo;

    tempStruct->setProperty("folderName" , path.section('/', -1));
    tempStruct->setProperty("folderPath" , path);

    QString dirpath = path;
    //设置要遍历的目录
    QDir dir(dirpath);

    //设置文件过滤器
    QStringList nameFilters;
    //设置文件过滤格式
    //nameFilters << "*";
    //将过滤后的文件名称存入到files列表中
    QStringList files = dir.entryList(nameFilters, QDir::Files| QDir::Readable, QDir::Type);
    QStringList dirs  = dir.entryList(nameFilters, QDir::Dirs | QDir::Readable, QDir::Name);
    foreach(QString tempString, files)
    {
        FileInfo* tempFileInfoStruct = new FileInfo();
        tempFileInfoStruct->setProperty("fileName" , tempString);
        tempFileInfoStruct->setProperty("filePath" , path + '/' + tempString);
        tempStruct->fileList.append(tempFileInfoStruct);
    }

    foreach(QString tempString, dirs)
    {
        if(tempString != "." && tempString!= "..")
        {
            //获取文件夹名
            tempStruct->setProperty("folderName" , path.section('/', -1));

            FolderInfo* tempStruct1 = getUsbChildrenFileInfo(path + "/" + tempString);

            tempStruct->nextNode.append(tempStruct1);
        }
    }
    return tempStruct;
}

void FunctionModule::postSelectTheFile(FileInfo* tempStruct)
{
    QJsonObject tempObject;
    tempObject.insert("print", false);
    tempObject.insert("command", "select");

    APIDisposeThread *newThread = new APIDisposeThread("post_0", "files", globalAccess->userinfo->cookies, tempObject, tempStruct->property("fileResource").toString());
    APIDisposeThread::runNewThread(newThread, "files", false);
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        qDebug() << __LINE__ << __FILE__ << _statusCode << _requestState << _QObj;
        //如果选择api没有问题, 就把这个文件结构体抛回给前端
        if(_statusCode == 204 && _requestState)
            emit sentSelectFileInfoSignal(tempStruct);
    });
}

void FunctionModule::postPrintFileCommand(FileInfo* tempStruct, QString command)
{
    QJsonObject tempObject;
    tempObject.insert("command", command);
    if(tempStruct->property("fileResource").toString().startsWith("http://"))
    {
        APIDisposeThread *newThread = new APIDisposeThread("post_0", command, globalAccess->userinfo->cookies, tempObject, globalAccess->MainUrl() + "job");
        APIDisposeThread::runNewThread(newThread, command, false);
        connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
            qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState;
            Q_UNUSED(_QObj)
        });
    }
    else
    {
        APIDisposeThread *newThread = new APIDisposeThread("post_0", "upload", globalAccess->userinfo->cookies, "", tempStruct->property("filePath").toString(), globalAccess->MainUrl() + "files/local");
        APIDisposeThread::runNewThread(newThread, "upload", false);
        connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
            qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState;
            Q_UNUSED(_QObj)
            if(_statusCode == 201 && _requestState)
            {
                QJsonObject tempSelectObject;
                tempSelectObject.insert("print", false);
                tempSelectObject.insert("command", "select");
                qDebug() << __LINE__ << __FILE__ << globalAccess->MainUrl() + "api/local/" + tempStruct->property("fileName").toString();
                APIDisposeThread *newThread = new APIDisposeThread("post_0", "files", globalAccess->userinfo->cookies, tempSelectObject, globalAccess->MainUrl() + "files/local/" + tempStruct->property("fileName").toString());
                APIDisposeThread::runNewThread(newThread, "files", false);
                connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
                    qDebug() << __LINE__ << __FILE__ << _statusCode << _requestState << _QObj;
                    if(_statusCode == 204 && _requestState)
                    {
                        APIDisposeThread *newThread = new APIDisposeThread("post_0", command, globalAccess->userinfo->cookies, tempObject, globalAccess->MainUrl() + "job");
                        APIDisposeThread::runNewThread(newThread, command, false);
                        connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
                            qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState;
                            Q_UNUSED(_QObj)
                        });
                    }
                });
            }
        });
    }
}

void FunctionModule::postUploadFileToOctoprint(QString folderPath, QString filePath)
{
    APIDisposeThread *newThread = new APIDisposeThread("post_0", "upload", globalAccess->userinfo->cookies, folderPath, filePath, globalAccess->MainUrl() + "files/local");
    APIDisposeThread::runNewThread(newThread, "upload", false);
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState;
        Q_UNUSED(_QObj)
    });
}
//test
void FunctionModule::showFileInfoItem(FolderInfo* showStruct, int header)
{
    QString tempFlagString;
    for(int i = 0; i < header; i++)
    {
        tempFlagString += "    ";
    }

    if(header != 0)
        qDebug() << tempFlagString << showStruct->property("folderName");
    else
        qDebug() << showStruct->property("folderName");
    foreach(FileInfo* tempStruct, showStruct->fileList)
    {
        if(header != 0)
            qDebug() << tempFlagString << tempStruct->property("fileName");
        else
            qDebug() << tempStruct->property("fileName");
    }

    foreach(FolderInfo* tempStruct, showStruct->nextNode)
    {
        showFileInfoItem(tempStruct , header + 1);
    }
}

void FunctionModule::downloadFile(FileInfo* downloadStruct, QString downloadGoal)
{
    QDir tempDir;
    if(tempDir.exists(downloadGoal))
    {
        tempDir.setCurrent(downloadGoal);
    }
    QFile *downloadFIle = new QFile(this);

    downloadFIle->setFileName(downloadStruct->property("fileDisplayName").toString());

    APIDisposeThread *newThread = new APIDisposeThread("get_1", "download", globalAccess->userinfo->cookies, QJsonObject (), downloadStruct->property("fileDownload").toString());
    APIDisposeThread::runNewThread(newThread, "download", false);
    connect(newThread, &APIDisposeThread::sentByte, this, [=](int _statusCode, QByteArray _tempByetArray, bool _requestState){
        qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState;
        if(downloadFIle->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            downloadFIle->write(_tempByetArray);
            downloadFIle->close();
            downloadFIle->deleteLater();
        }
    });
}

QList<FolderInfo*> FunctionModule::getUsbFileInfo(QStringList usbFolder)
{
    QList<FolderInfo*> tempStructList;
    foreach(QString tempFolderPath, usbFolder)
    {
        tempStructList.append(getUsbChildrenFileInfo(tempFolderPath));
    }
    return tempStructList;
}

void FunctionModule::delFile(QString filePathOrResource)
{
    if(filePathOrResource.startsWith("http://"))
    {
        APIDisposeThread *newThread = new APIDisposeThread("deleteResource", "del", globalAccess->userinfo->cookies, QJsonObject (), filePathOrResource);
        APIDisposeThread::runNewThread(newThread, "del", false);
        connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
            qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState;
            Q_UNUSED(_QObj)
        });
    }
    else
    {
        if(QFile::exists(filePathOrResource))
        {
            if(!QFile::remove(filePathOrResource))
            {
                QDir(filePathOrResource).removeRecursively();
            }
        }
    }
}

void FunctionModule::createFolder(FolderInfo* tempStruct, QString folderName)
{
    if(tempStruct->property("folderResource").toString().startsWith("http://"))
    {
        APIDisposeThread *newThread = new APIDisposeThread("post_2", "folderAdded", tempStruct->property("folderPath").toString(), folderName, globalAccess->userinfo->cookies, globalAccess->MainUrl() + "files/local");
        APIDisposeThread::runNewThread(newThread, "folderAdded", false);
        connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
            qDebug() << __LINE__ << __FILE__ <<  _statusCode  << _requestState << _QObj;
        });
    }
    else
    {
        QDir tempDir;
        if(QFile::exists(tempStruct->property("folderPath").toString()))
        {
            QDir::setCurrent(tempStruct->property("folderPath").toString());
            tempDir.mkdir(folderName);
        }
    }
}
//发送命令组
void FunctionModule::sentGcodeCommand(QStringList commands)
{
    QJsonObject commandObject;
    QJsonArray tempArray;
    foreach(QString command,commands)
    {
        if(command != "")
            tempArray.append(command);
    }
    commandObject.insert("commands", QJsonValue(tempArray));

    APIDisposeThread *newThread = new APIDisposeThread("post_0", "command", globalAccess->userinfo->cookies, commandObject, globalAccess->MainUrl() + "printer/command");
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        qDebug()  << __FILE__ << __LINE__ <<  _statusCode  << _requestState << _QObj;
    });
    APIDisposeThread::runNewThread(newThread, "command", false);
}

void FunctionModule::sentGcodeCommand(QString command)
{
    sentGcodeCommand({command,""});
}

void FunctionModule::postSetting(QList<QString> settingPathList, QList<bool> settingPathIsArray,QVariant settingValue)
{
    QString tempTypeString = settingPathList.last();

//    qDebug() << __FILE__ << __LINE__ << settingIsObject(settingPathList,settingPathIsArray,QJsonValue::fromVariant( settingValue));//test

    APIDisposeThread *newThread = new APIDisposeThread("post_0", "settings" + tempTypeString, globalAccess->userinfo->cookies, settingIsObject(settingPathList,settingPathIsArray,QJsonValue::fromVariant( settingValue)), globalAccess->MainUrl() + "settings");
    APIDisposeThread::runNewThread(newThread, "settings" + tempTypeString, false);
}

QJsonObject FunctionModule::settingIsObject(QList<QString> settingPathList, QList<bool> settingPathIsArray,QJsonValue settingValue)
{
    QJsonObject output;
    if(settingPathIsArray.takeLast())
    {
        QJsonArray data;
        data.append(settingValue);
        output.insert(settingPathList.takeLast(),data);
    }
    else
    {
        output.insert(settingPathList.takeLast(),settingValue);
    }
    if(settingPathList.count() > 0)
    {
        output = settingIsObject(settingPathList,settingPathIsArray,output);
    }
    return output;
}
//获取最新配置文件信息
void FunctionModule::getSettings()
{
    APIDisposeThread *newThread = new APIDisposeThread("get", "settings", globalAccess->userinfo->cookies);
    //接收线程返回的信息
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        if(_statusCode == 200 && _requestState)
            emit settingsUpdated(QJsonDocument(_QObj).toJson());
    });
    APIDisposeThread::runNewThread(newThread, "settings", false);
}

//执行机器连接
void FunctionModule::postConnect(QString command ,QString port,int baudrate,QString printerProfile ,bool autoconnect,bool save)
{
    QJsonObject output;
    if(command == "connect")
    {
        output.insert("port",port);
        output.insert("baudrate",baudrate);
        output.insert("printerProfile",printerProfile);
        output.insert("autoconnect",autoconnect);
        output.insert("save",save);
//        {
//            "port": "AUTO",
//            "baudrate": 0,
//            "printerProfile": "_default",
//            "autoconnect": true,
//            "save": true,
//            "command": "connect"
//        }
    }
    output.insert("command",command);
    APIDisposeThread *newThread = new APIDisposeThread("post_1", "connection", globalAccess->userinfo->cookies,output);

    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        Q_UNUSED(_QObj);
        if(_statusCode == 200 && _requestState)
            getSettings();
    });

    APIDisposeThread::runNewThread(newThread, "connection", false);
}

void FunctionModule::getCore()
{
    APIDisposeThread *newThread = new APIDisposeThread("get", "core", globalAccess->userinfo->cookies,QJsonObject(), globalAccess->MainUrl() + "system/commands/core");
    //接收线程返回的信息
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        if(_statusCode == 200 && _requestState)
            emit coreReply(QJsonDocument(_QObj).toJson());
    });
    APIDisposeThread::runNewThread(newThread, "core", false);
}
void FunctionModule::runCore(QString path)
{
    APIDisposeThread *newThread = new APIDisposeThread("post_0", "core", globalAccess->userinfo->cookies,QJsonObject(), path);
    APIDisposeThread::runNewThread(newThread, "core", false);
}

void FunctionModule::sentJobControl(QString command)
{
    QJsonObject commandObject;
    commandObject.insert("commands", command);

    APIDisposeThread *newThread = new APIDisposeThread("post_0", "job", globalAccess->userinfo->cookies, commandObject);
    connect(newThread, &APIDisposeThread::sentRequest, this, [=](int _statusCode, QJsonObject _QObj, bool _requestState){
        qDebug()  << __FILE__ << __LINE__ <<  _statusCode  << _requestState << _QObj;
    });
    APIDisposeThread::runNewThread(newThread, "job", false);
}
