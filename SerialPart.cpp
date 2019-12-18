#include "SerialPart.h"
//#pragma execution_character_set("utf-8")
SerialPart::SerialPart(QObject *parent) : QObject(parent)
{
    serialport_m = new QSerialPort;
    QObject::connect(&time,SIGNAL(timeout()),this,SLOT(SerialRead_Date()));
    //遍历能打开的串口设备，添加到ui界面的Item列表
    //this->Serialfind_port((Widget*)parent);
}

void SerialPart::comdelay()
{
    time.start(50);//50ms
    buf.append(serialport_m->readAll());
    qDebug()<<buf;
}
//void SerialPart::Serialfind_port(Widget *w)
//{

//}

void SerialPart::SerialRead_Date()
{
time.stop();
    //buf = serialport_m->readAll();
   if(!buf.isEmpty())          //将数据显示到文本串口
    {
        //获取之前的文件信息
        QString str ;//= w->getUi()->textBrowser->toHtml();
        //if(str.size() >= 10 * 1024) str.clear();
       // qDebug()<<"textBrowser"<<str;
        //qDebug()<<"buf"<<buf[0];
        //str = QString::fromLocal8Bit(str); //支持中文显示
        qDebug()<<buf.toHex().at(0)<<buf.toHex().at(1)<<buf.toHex().at(2)<<buf.toHex().at(3);
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


        //w->getUi()->textBrowser->clear();

        //w->getUi()->textBrowser->append();
        w->getUi()->textBrowser->append(str);
    }
    buf.clear();    //清空缓存区
}

void SerialPart::Serialfind_port()
{
    //清除列表
    qDebug()<<"Serialfind_port";
     w->getUi()->Serial_PortBox->clear();
    //遍历能打开的串口设备，添加到ui界面的Item列表
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);   //设置串口
        if(serial.open(QIODevice::ReadWrite))
        {
            w->getUi()->Serial_PortBox->addItem( info.portName()+ ":"+info.description());//在comboBox那添加串口号
            serial.close();
        }
    }
}


