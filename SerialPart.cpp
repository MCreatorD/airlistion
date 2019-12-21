#include "SerialPart.h"
#include <QThread>

SerialPart::SerialPart(QObject *parent) : QObject(parent),hasData(false)
{
    serialport_m = new QSerialPort;
    //QObject::connect(&time,SIGNAL(timeout()),this,SLOT(SerialRead_Date()));
    connect(serialport_m,SIGNAL(readyRead()),this,SLOT(comdelay()));
}

void SerialPart::comdelay()
{
    //time.start(20);//50ms
    buf.append(serialport_m->readAll());
    //emit hasUartData();
    //qDebug()<<buf;
    hasData = true;
}

void SerialPart::SerialRead_Date()
{
    //time.stop();
    //buf = serialport_m->readAll();
   if(!buf.isEmpty())          //将数据显示到文本串口
    {
        //获取之前的文件信息
        QString str ;//= w->getUi()->textBrowser->toHtml();
        //qDebug()<<buf.toHex().at(0)<<buf.toHex().at(1)<<buf.toHex().at(2)<<buf.toHex().at(3);
        if(buf.toHex().at(0) == 'a'&&buf.toHex().at(1) == '1'&&buf.toHex().at(2) == 'f'&&buf.toHex().at(3) == 'd')
        {
            //qDebug()<<"buf"<<0;
           str += " \" <font color=\"#FF0000\">"+QString::fromLocal8Bit(buf)+"</font> \" ";
        }
        else if(buf.toHex().at(0) == 'a'&&buf.toHex().at(1) == '1'&&buf.toHex().at(2) == 'f'&&buf.toHex().at(3) == 'c')
        {
            //qDebug()<<"buf"<<1;
            str += " \"<font color=\"#00FF00\">"+QString::fromLocal8Bit(buf)+"</font> \" ";
        }
        w->getUi()->textBrowser->append(str);
    }
   buf.clear();    //清空缓存区
}

void SerialPart::doWork(const QString &paramete)
{
    qDebug()<<"dowork"<<paramete;
    while(1)
    {
        //qDebug()<<"doWork";
        if(serialport_m->isOpen())
        {
            while(!serialport_m->atEnd())
            {
                QString str = QString::fromLocal8Bit(serialport_m->readAll());
                QStringList tmplist = SearchComitList(str);
                //qDebug()<<"Source:"<<str<<"start:"<<str.startsWith(QString::fromLocal8Bit("↓A"))<<str.startsWith(QString::fromLocal8Bit("↑A"))<<"end:"<<str.endsWith("\r\n")<<"end";
                //qDebug()<<"QStringList:"<<tmplist.size();
                if(!tmplist.isEmpty())
                {
                    emit CommitList(tmplist);
                    //判断是否需要保存日志
                    if(1)
                    {


                    }
                }
            }
        }
//        if(hasData)
//        {
//            hasData = false;

//            qDebug()<<buf;
//        }
        //QThread::msleep(40);

    }

    //time.start(20);//50ms
//    buf.append(serialport_m->readAll());
//    qDebug()<<buf;
}

QStringList SerialPart::Serialfind_port()
{
    QStringList ComNum;//返回可以正常打开的串口
    //清除列表
    qDebug()<<"Serialfind_port";
    //遍历能打开的串口设备，添加到ui界面的Item列表
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);   //设置串口
        if(serial.open(QIODevice::ReadWrite))
        {
            ComNum.append(info.portName()+ ":"+info.description());
            serial.close();
        }
    }
    return ComNum;
}

QStringList SerialPart::SearchComitList(QString Buf)
{
    /*
     * 解析数据缓冲
     * 1. 有头有尾 完整帧 直接解析
     * 2. 有头无尾 判断最后一个完整帧位置 前面解析 后面保留为需要解析的数据头部
     * 3. 有尾巴无头的 抛弃
    */
    static bool hasHead = false;
    static bool hasEnd = false;
    QStringList ret;
    if(hasHead==true)//之前有头部信息没有使用则 拼接后面的数据
    {
        qDebug()<<"sub add sub";
        Buf = subBuf + Buf;
    }

    if(Buf.startsWith(QString::fromLocal8Bit("↓A"))||Buf.startsWith(QString::fromLocal8Bit("↑A")))
    {
        hasHead = true ;
    }else{
         hasHead = false ;
    }
    if(Buf.endsWith("\r\n"))
    {
        hasEnd = true;
    }else
    {
        hasEnd = false;
    }

    if(hasHead==true && hasEnd==true)
    {
        //查找里面总共有多少消息帧
        ret = Buf.split("\r\n");
        //qDebug()<<"ret cnt0"<<ret.count()-1<<"ret"<<ret.at(0);

        hasHead = false;
        hasEnd  = false;
    }
    else if(hasHead==true && hasEnd==false)
    {
        //查找最后一个出现"\r\n"的位置 保存后一段数据
         int position = Buf.lastIndexOf("\r\n");
         //qDebug()<<"all"<<Buf.size()<<"sub"<<position;
         if(position == -1)
         {
             hasHead = true;
             qDebug()<<"sub  buff";
             subBuf = Buf;
            return    ret;
         }
         //含有完整帧则先提取信息
         //查找里面总共有多少消息帧
         ret = Buf.split("\r\n");
         //qDebug()<<"ret cnt1"<<ret.count()-1<<"ret"<<ret.at(0);

        //保留\r\n之后的数据同时判断是否是下个数据的开头 如果是的则标记有头 否则标记无头
        if(Buf.size() - position == 3)//\r\n 之后只有一个字符
        {
            if(Buf[(position+2)] == QString::fromLocal8Bit("↓")||Buf[(position+2)] == QString::fromLocal8Bit("↑"))
            {
                qDebug()<<"ok";
                hasHead=true;
                subBuf = Buf.mid(position+3,Buf.size());
                //qDebug()<<"subBuf:"<<subBuf;
            }else
            {
                hasHead=false;
            }
        }else if(Buf.size()-position>3)//\r\n 之后大于一个字符
        {
            if(Buf[(position+2)] == QString::fromLocal8Bit("↓")||Buf[(position+2)] == QString::fromLocal8Bit("↑"))
            {
                if(Buf[(position+3)] == QString::fromLocal8Bit("A"))
                {
                    qDebug()<<"ok";
                    hasHead=true;
                    subBuf = Buf.mid(position+4,Buf.size());
                    //qDebug()<<"subBuf:"<<subBuf;
                }

            }else
            {
                hasHead=false;
            }
        }
    }else if(hasHead==false && hasEnd==false)
    {
        //当前帧无头判断直接是否有头 或者有头数据先拼接再判断

    }
     //qDebug()<<"Source:"<<str<<"start:"<<str.startsWith(QString::fromLocal8Bit("↓A"))<<str.startsWith(QString::fromLocal8Bit("↑A"))<<"end:"<<str.endsWith("\r\n")<<"end";

    return ret;
}


//

