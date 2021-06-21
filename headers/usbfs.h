#ifndef USBFS_H
#define USBFS_H
#include "terminal.h"
/// 创建指针方式引用，会自动释放指针
/// 指针声明后，绑定信号槽，调用checkusb即可，处理完槽后退出会自动释放指针
/// 依赖模块包：terminal
/// 例子：
/// USBFS *chk = new USBFS();
/// {信号槽绑定片段}
/// chk->checkusb();
class USBFS: public QObject
{
Q_OBJECT
signals:
    void Check_Reply(QStringList value);
public:
//    explicit USBFS(QStringList *receiver = nullptr , QString _chk_cmd = "df -h | awk '{print $6}' | grep /media/usb");
    explicit USBFS(QStringList *receiver = nullptr , QString _chk_cmd = "df -h | awk '{print $9}' | grep /System/Volumes");
//    ~USBFS();
    void checkusb();
    int checkDelayMS = 10;
public:
    QString chk_cmd = "df -h | awk '{print $6}' | grep /media/usb";
//    QString chk_cmd = "df -h | awk '{print $9}' | grep /System/Volumes";
private:
    Terminal terminal;
};
#endif // USBFS_H
