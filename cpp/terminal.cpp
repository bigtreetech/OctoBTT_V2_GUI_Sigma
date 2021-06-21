#include "../headers/terminal.h"

Terminal::Terminal(QString _cmd_Password ,QString _terminalPath ) : QObject()
{
    processPool = new ProcessPool();
    cmd_Password = _cmd_Password;
    terminalPath = _terminalPath;
    timeOut.setSingleShot(true);

    connect(processPool,&Terminal::ProcessPool::CMD_Single_Error,this,[=](QString value){emit CMD_Single_Error(value);});
    connect(processPool,&Terminal::ProcessPool::CMD_Error,this,[=](QStringList value){emit CMD_Error(value);});
    connect(processPool,&Terminal::ProcessPool::CMD_Single_Reply,this,[=](QString value){emit CMD_Single_Reply(value);});
    connect(processPool,&Terminal::ProcessPool::CMD_Reply,this,[=](QStringList value){emit CMD_Reply(value);});

    connect(&timeOut,&QTimer::timeout,this,&Terminal::timeOutTimeout);

    connect(processPool,&Terminal::ProcessPool::thread_exit,processPool,&Terminal::ProcessPool::quit);

    connect(processPool,&Terminal::ProcessPool::thread_exit,this,[=]()
    {
//        delete qobject_cast<ProcessPool*>(sender());
        emit thread_exit();
//        delete this;//删除指针
        this->deleteLater();
        this->destroyed();
    });
}

Terminal::~Terminal()
{
}

void Terminal::SendCMD(QString CommandLine ,int time_out)
{
    processPool->cmd_Password = cmd_Password;
    processPool->terminalPath = terminalPath;
    if(time_out > 0)
    {
        timeOut.setInterval(time_out);
//        timeOut.start(time_out);
        connect(processPool,&Terminal::ProcessPool::thread_run,this,[=]()
        {
            timeOut.start();
        });
    }
    processPool->start_Thread(CommandLine);
}

QString Terminal::character_conversion(QString value)
{
    QString Data = value;
    while (Data.indexOf("\\\\")>=0)//多余\\字符预处理
    {
        Data = Data.replace("\\\\","*\\*");
    }
    while (Data.indexOf("\\x")>=0)//转译字符处理
    {
        int index = Data.indexOf("\\x");
        QString Block = Data.mid(index,12);
        Block = Block.replace("\\x", QString());
        QString Blocks ;
        Blocks.append(Block.mid(0,2).toUShort(0,16));
        Blocks.append(Block.mid(2,2).toUShort(0,16));
        Blocks.append(Block.mid(4,2).toUShort(0,16));
        Data = Data.replace(index, 12, tr(Blocks.toLatin1().data()));
    }
    while (Data.indexOf("*\\*")>=0)//多余\\字符终处理
    {
        Data = Data.replace("*\\*","\\");
    }
    return Data;
}

void Terminal::timeOutTimeout()
{
    stopTerminal();
}


void Terminal::stopTerminal()
{
    processPool->stopflag = true;
}

///
/// ProcessPool
///

Terminal::ProcessPool::~ProcessPool()
{
    this->deleteLater();
    this->destroyed();
}

void Terminal::ProcessPool::processStarted()
{
//        qDebug() << __FILE__ << __LINE__ << qobject_cast<QProcess*>(sender())->processId() << "started: " << command;
//    Terminal::terminal_thread_count ++;
    emit thread_run();
    qobject_cast<QProcess*>(sender())->write(command.toLocal8Bit()+ "\n");
}

void Terminal::ProcessPool::processStatechanged(QProcess::ProcessState newState)
{
    Q_UNUSED(newState)
    //    qDebug() << __FILE__ << __LINE__  << qobject_cast<QProcess*>(sender())->processId() << "ProcessState: " << newState;
}


void Terminal::ProcessPool::processReadyreadstandarderror()
{
    foreach(QString item , QString::fromLocal8Bit(qobject_cast<QProcess*>(sender())->readAllStandardError()).split('\n'))
    {
        if(item.trimmed() != "")
        {
            StandardError.append(Terminal::character_conversion(item));
            emit CMD_Single_Error(Terminal::character_conversion(item));
        }
    }
}

void Terminal::ProcessPool::processReadyreadstandardoutput()
{
    QStringList value;
    QString last_rec;
    foreach(QString item , QString::fromLocal8Bit(qobject_cast<QProcess*>(sender())->readAllStandardOutput()).split('\n'))
    {
        if(item.trimmed() != "")
        {
            last_rec=item.trimmed();
            if(last_rec != "finished")
                StandardOutput.append(Terminal::character_conversion(item));
            emit CMD_Single_Reply(Terminal::character_conversion(item));
        }
//           qDebug() << __FILE__ << __LINE__ << qobject_cast<QProcess*>(sender())->processId() << "StandardOutput: " << item;
    }
    if(last_rec == "finished")
    {
        stopflag = true;
    }
}

void Terminal::ProcessPool::processErroroccurred(QProcess::ProcessError error)
{
    StandardError.append(QString(error));
    emit CMD_Single_Error(QString(error));
//        qDebug() << __FILE__ << __LINE__  << tmpprc.processId() << "ProcessError: " << ProcessError;
}

void Terminal::ProcessPool::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    stopflag = true;

//    qobject_cast<QProcess*>(sender())->close();
    qobject_cast<QProcess*>(sender())->terminate();
    qobject_cast<QProcess*>(sender())->kill();
}

void Terminal::ProcessPool::start_Thread(QString CommandLine)
{
    command = CommandLine.trimmed();
    if(command.isEmpty() || command == "")
    {
        return;
    }
    if(command.contains("sudo "))
    {
        int startlocal = command.indexOf("sudo ");
        command = command.replace(startlocal,5,"echo \"" + cmd_Password + "\" | sudo -S ");//片段阔幅代码未处理
    }
    command = command + " ; echo finished";

    this->start();
}

void Terminal::ProcessPool::run()//线程运行核心
{
    qRegisterMetaType<QProcess::ProcessState>();
    qRegisterMetaType<QProcess::ProcessError>();
    qRegisterMetaType<QProcess::ExitStatus>();

    QProcess tmpprc;

    //接收处理
    QObject::connect(&tmpprc,&QProcess::started,this,&Terminal::ProcessPool::processStarted);
    QObject::connect(&tmpprc,&QProcess::readyReadStandardOutput,this,&Terminal::ProcessPool::processReadyreadstandardoutput);
    QObject::connect(&tmpprc,&QProcess::readyReadStandardError,this,&Terminal::ProcessPool::processReadyreadstandarderror);
    QObject::connect(&tmpprc,&QProcess::errorOccurred,this,&Terminal::ProcessPool::processErroroccurred);
    QObject::connect(&tmpprc,&QProcess::stateChanged,this,&Terminal::ProcessPool::processStatechanged);
    QObject::connect(&tmpprc,QOverload<int,             QProcess::ExitStatus>::of(&QProcess::finished),this,&Terminal::ProcessPool::processFinished);

//    while (Terminal::terminal_thread_count >= Terminal::terminal_thread_MAX) {
//        msleep(100);
//    }//线程限制

    tmpprc.start(terminalPath);//启动进程

    while(!stopflag)
    {
        tmpprc.waitForFinished(100);
    }
    tmpprc.waitForFinished(100);
    if(tmpprc.state() == QProcess::Running)
    {
        tmpprc.kill();
        tmpprc.waitForFinished();
    }
    tmpprc.waitForFinished();

//    for(int i = StandardOutput.count() ;i > 0 ; i--)
//    {
//        if(StandardOutput.last().trimmed() == "")
//            StandardOutput.removeLast();
//        else if(StandardOutput.last().trimmed() == "finished")
//            StandardOutput.removeLast();
//        else
//            break;
//    }
    emit CMD_Reply(StandardOutput);
    emit CMD_Error(StandardError);
    msleep(1000);
    emit thread_exit();
//    Terminal::terminal_thread_count --;
}
