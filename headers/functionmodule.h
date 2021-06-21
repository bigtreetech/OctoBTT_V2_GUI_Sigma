#ifndef FUNCTIONMODULE_H
#define FUNCTIONMODULE_H

#include "disposethreadwithapi.h"
#include "accesscontrol.h"
#include <QMetaEnum>
#include <QObject>

/**************************************************
*   文件名: functionModule.h and functionModule.cpp
*   依赖文件: accesscontrol.h and accesscontrol.cpp disposethreadwithapi.h and disposethreadwithapi.cpp
*   工程引用:
*   文件用途: 用于文件的多种API操作以及外载设备的文件处理
*
*   使用举例:
*      设置全局变量 globalFunction [FunctionModule *globalFunction = new FunctionModule();]
*      通过以下信号调用对应的方法
*      //[外载设备路径链表]
*      void getFileInfoSignal(QStringList); //获取所有文件信息(octoprint, SDcard, 外载设备) 此方法会通过sentFolderInfoSignal(FolderInfo)抛出最终文件夹的结构体
*      //[要选择的文件的文件结构体]
*      void postSelectTheFileSignal(FileInfo);  //选择文件API操作 此方法会通过sentSelectFileInfoSignal(FileInfo)抛出API操作成功后的文件结构体
*      //[前端选择的文件结构体][字符串命令]
*      void postPrintFileCommandSignal(FileInfo, QString);  //打印指令传送 start or cancel
*      //[上传octoprint目标文件夹的文件夹结构体][要上传的文件的文件结构体]
*      void postUploadFileToOctoprintSignal(FolderInfo, FileInfo); //从外载设备中上传文件至octoprint
*      //[要下载的octoprint文件的文件结构体][要保存文件的外载设备文件夹的文件结构体]
*      void downloadFileSignal(FileInfo, FolderInfo); //从octoprint下载文件至外载设备中
*      //[要删除的文件的文件结构体]
*      void delFileSignal(FileInfo);    //删除文件
*      //[要删除的文件夹的文件夹结构体]
*      void delFolderSignal(FolderInfo);            //删除文件夹
*      //[要创建新目录的文件夹的文件夹接头][新的文件夹名称]
*      void createFolderSignal(FolderInfo, QString); //创建文件夹
*      //[命令字符串]
*      void sentGcodeCommandSignal(QString);        //发送gcode指令
*
*      最后修改日期: 2021 / 5 / 27
**************************************************/

extern AccessControl *globalAccess;

struct FileInfo:public QObject    /* 单个文件存储的信息结构体 */
{
    Q_OBJECT

    Q_PROPERTY(QString fileName MEMBER fileName NOTIFY fileName_Change);
    Q_PROPERTY(QString filePath MEMBER filePath NOTIFY filePath_Change);
    Q_PROPERTY(QString fileSize MEMBER fileSize NOTIFY fileSize_Change);
    Q_PROPERTY(QString fileClass MEMBER fileClass NOTIFY fileClass_Change);
    Q_PROPERTY(QString modelWidth MEMBER modelWidth NOTIFY modelWidth_Change);

    Q_PROPERTY(QString modelDepth MEMBER modelDepth NOTIFY modelDepth_Change);
    Q_PROPERTY(QString modelHeight MEMBER modelHeight NOTIFY modelHeight_Change);
    Q_PROPERTY(QString fileResource MEMBER fileResource NOTIFY fileResource_Change);
    Q_PROPERTY(QString fileDownload MEMBER fileDownload NOTIFY fileDownload_Change);
    Q_PROPERTY(QString fileDisplayName MEMBER fileDisplayName NOTIFY fileDisplayName_Change);

    Q_PROPERTY(QString estimatedPrintTime MEMBER estimatedPrintTime NOTIFY estimatedPrintTime_Change);
private:
    QString fileName;
    QString filePath;
    QString fileSize;
    QString fileClass;
    QString modelWidth;

    QString modelDepth;
    QString modelHeight;
    QString fileResource;
    QString fileDownload;
    QString fileDisplayName;

    QString estimatedPrintTime;
signals:
    void fileName_Change();
    void filePath_Change();
    void fileSize_Change();
    void fileClass_Change();
    void modelWidth_Change();

    void modelDepth_Change();
    void modelHeight_Change();
    void fileResource_Change();
    void fileDownload_Change();
    void fileDisplayName_Change();

    void estimatedPrintTime_Change();
};
struct FolderInfo: public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString folderName MEMBER folderName NOTIFY folderName_Change);
    Q_PROPERTY(QString folderPath MEMBER folderPath NOTIFY folderPath_Change);
//    Q_PROPERTY(QList<FolderInfo*> nextNode MEMBER nextNode NOTIFY nextNode_Change);
    Q_PROPERTY(QString folderResource MEMBER folderResource NOTIFY folderResource_Change);
    Q_PROPERTY(QString folderSize MEMBER folderSize NOTIFY folderSize_Change);
//    Q_PROPERTY(QList<FileInfo*> fileList MEMBER fileList NOTIFY fileList_Change);
    Q_PROPERTY(QString filesFree MEMBER filesFree NOTIFY filesFree_Change);
    Q_PROPERTY(QString filesTotal MEMBER filesTotal NOTIFY filesTotal_Change);
public:
    Q_INVOKABLE QList<FolderInfo*> nextNode;     /* 保存下级文件夹的结构体 */
    Q_INVOKABLE QList<FileInfo*> fileList;       /* 保存当前文件夹所有的文件 */

    
private:
    QString folderName;             /* 当前文件夹的名字 */
    QString folderPath;             /* 当前文件夹的路径 */
//    QStringList dirList;
    
    QString folderResource;         /* 保存当前文件夹资源路径 */
    QString folderSize;             /* 当前文件夹的大小 */
    QString filesFree;              /* 总的闲置空间 */
    QString filesTotal;             /* 总空间 */
signals:
    void folderName_Change();
    void folderPath_Change();
//    void nextNode_Change();
    void folderResource_Change();
    void folderSize_Change();
//    void fileList_Change();
    void filesFree_Change();
    void filesTotal_Change();
};

//Q_DECLARE_METATYPE(FolderInfo)
//Q_DECLARE_METATYPE(FileInfo)

class FunctionModule : public QObject
{
    Q_OBJECT
public:
    explicit FunctionModule(QObject *parent = nullptr);

    //获取所有文件信息(octoprint, SDcard, 外载设备)
    void getFileInfo(QStringList usbStringList = QStringList());
    //选择文件API操作
    void postSelectTheFile(FileInfo* tempStruct);
    //打印指令传送 start or cancel
    void postPrintFileCommand(FileInfo* tempStruct, QString command);
    //从外载设备中上传文件至octoprint
    void postUploadFileToOctoprint(QString folderPath, QString filePath);
    //从octoprint下载文件至外载设备中
    void downloadFile(FileInfo* downloadStruct, QString downloadGoal);
    //删除文件
    void delFile(QString filePathOrResource);
    //创建文件夹
    void createFolder(FolderInfo* tempStruct, QString folderName);
    //发送gcode指令
    Q_INVOKABLE void sentGcodeCommand(QStringList commands);
    Q_INVOKABLE void sentGcodeCommand(QString command);
    Q_INVOKABLE void sentJobControl(QString sentJobControl);
    Q_INVOKABLE void postSetting(QList<QString> settingPathList, QList<bool> settingPathIsArray,QVariant settingValue);
    Q_INVOKABLE void postConnect(QString command ,QString port = "AUTO",int baudrate = 0,QString printerProfile = "_default" ,bool autoconnect = true,bool save = true);
    Q_INVOKABLE void getSettings();
    Q_INVOKABLE void getCore();
    Q_INVOKABLE void runCore(QString path);
private:
    //获取外载设备的文件夹的结构体信息
    QList<FolderInfo*> getUsbFileInfo(QStringList usbFolder);
    //获取文件夹下级文件夹的结构体
    FolderInfo* getChildrenFileInfo(QJsonObject tempObject);
    //获取外载设备的文件夹的下级文件夹结构体信息
    FolderInfo* getUsbChildrenFileInfo(QString path);
    //test测试结构体 //用于测试获取的文件结构体是否正确
    void showFileInfoItem(FolderInfo* showStruct ,int header = 0);
    QJsonObject settingIsObject(QList<QString> settingPathList, QList<bool> settingPathIsArray,QJsonValue settingValue);
signals:
    //发送文件夹信息结构体
    void sentFolderInfoSignal(FolderInfo*);
    //发送选择的文件的文件结构体
    void sentSelectFileInfoSignal(FileInfo*);
    //==前端调用信号==
    //[外载设备路径链表]
    void getFileInfoSignal(QStringList);
    //[要选择的文件的文件结构体]
    void postSelectTheFileSignal(FileInfo*);
    //[字符串命令]
    void postPrintFileCommandSignal(FileInfo*, QString);
    //[上传octoprint目标文件夹的文件夹结构体][要上传的文件的文件结构体]
    void postUploadFileToOctoprintSignal(FolderInfo*, FileInfo*);
    //[要下载的octoprint文件的文件结构体][要保存文件的外载设备文件夹的文件结构体]
    void downloadFileSignal(FileInfo*, FolderInfo*);
    //[要删除的文件的文件结构体]
    void delFileSignal(FileInfo*);
    //[要删除的文件夹的文件夹结构体]
    void delFolderSignal(FolderInfo*);
    //[要创建新目录的文件夹的文件夹接头][新的文件夹名称]
    void createFolderSignal(FolderInfo*, QString);
    //[命令字符串]
    void sentGcodeCommandSignal(QString);
    //settings数据返回
    void settingsUpdated(QString json);
    //core数据返回
    void coreReply(QString json);
private:
};

#endif // FUNCTIONMODULE_H
