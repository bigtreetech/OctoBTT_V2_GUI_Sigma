#include "../headers/usbfs.h"

USBFS::USBFS(QStringList *receiver,QString _chk_cmd) : QObject()
{
    chk_cmd = _chk_cmd;
    QObject::connect(&terminal,&Terminal::CMD_Reply,this,[=](QStringList reply)
    {
        for(int i = reply.count() ; i > 0 ; i--)
        {
            if(reply[i - 1].trimmed() == "/media/usb")
            {
                reply.removeAt(i - 1);
            }
        }
        if(receiver != NULL)
            *receiver = reply;
//        qDebug() << "";
//        qDebug() <<"USBFS:\n\t"<< reply;
//        qDebug() << "";
        emit Check_Reply(reply);

//        assert(this);
//        qDebug() << &terminal;
//        Q_ASSERT(this);
        this->deleteLater();
        this->destroyed();
//        delete this;
    });

//    terminal.msDelayTime = checkDelayMS;
}

//USBFS::~USBFS()
//{
//    delete this;
//}

void USBFS::checkusb()
{
    terminal.SendCMD(chk_cmd,checkDelayMS);
}
