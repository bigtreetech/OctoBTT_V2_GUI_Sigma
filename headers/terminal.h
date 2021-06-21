#ifndef TERMINAL_H
#define TERMINAL_H

#include <QProcess>
#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>

/// 指针自动释放
/// 例子：
/// Terminal* myterminal = new Terminal();
/// {信号槽绑定}
/// myterminal->SendCMD("df -h | awk '{print $6}' | grep /media/usb");

class Terminal : public QObject
{
    Q_OBJECT
    class ProcessPool;
signals:
    //最后发出的所有接收内容
    void CMD_Reply(QStringList value);
    //单发信号(每行有效行发出)
    void CMD_Single_Reply(QString value);
    //最后发出的所有错误内容
    void CMD_Error(QStringList value);
    //单发错误信号(每行有效错误行发出)
    void CMD_Single_Error(QString value);
    //线程结束
    void thread_exit();

public:
    explicit Terminal(QString _cmd_Password = "raspberry",QString _terminalPath = "/bin/bash");
    ~Terminal();

    void SendCMD(QString CommandLine, int time_out = 5000);

    Q_INVOKABLE static QString character_conversion(QString);//字符集转换方法，针对WPA扫描字符
private:
//    void loopDalay(int ms = 1);
//    void runThread();
//    static int terminal_thread_count;
//    static const int terminal_thread_MAX = 100;

public slots:
    //外部终止线程槽
    Q_INVOKABLE void stopTerminal();

private slots:
//    void  processErroroccurred(QProcess::ProcessError error);
//    void  processFinished(int exitCode, QProcess::ExitStatus exitStatus);
//    void  processReadyreadstandarderror();
//    void  processReadyreadstandardoutput();
//    void  processStarted();
//    void  processStatechanged(QProcess::ProcessState newState);
    void  timeOutTimeout();

public:
    QString cmd_Password = "raspberry";
    QString terminalPath = "/bin/bash";

    volatile bool stop = false;
//    volatile int msDelayTime = 10;

private:
    QString command;
    QStringList StandardOutput;
    QStringList StandardError;


    QTimer timeOut;
//    QProcess tmpprc;
//    QEventLoop loop;
//    QTimer delayTimer;
    ProcessPool* processPool;
};

class Terminal::ProcessPool: public QThread
{
    Q_OBJECT
    friend class Terminal;
public slots:
    void start_Thread(QString CommandLine);
//    void stop_Thread();
private slots:
    void  processErroroccurred(QProcess::ProcessError error);
    void  processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void  processReadyreadstandarderror();
    void  processReadyreadstandardoutput();
    void  processStarted();
    void  processStatechanged(QProcess::ProcessState newState);

public:
    QString cmd_Password = "raspberry";
    QString terminalPath = "/bin/bash";
    ~ProcessPool();

    volatile bool stopflag = false;

private:
//    void loopDalay(int ms = 1);

    QString command;
    QStringList StandardOutput;
    QStringList StandardError;

//    QProcess tmpprc;
//    QTimer delayTimer;
//    QEventLoop loop;

protected:
    void run();

signals:
    //最后发出的所有接收内容
    void CMD_Reply(QStringList value);
    //单发信号(每行有效行发出)
    void CMD_Single_Reply(QString value);
    //最后发出的所有错误内容
    void CMD_Error(QStringList value);
    //单发错误信号(每行有效错误行发出)
    void CMD_Single_Error(QString value);
    //线程结束
    void thread_exit();
    //线程结束
    void thread_run();

};
#endif // TERMINAL_H
