#include "SerialPart.h"

SerialPart::SerialPart(QObject *parent) : QObject(parent)
{
    serialport_m = new QSerialPort;
    //遍历能打开的串口设备，添加到ui界面的Item列表
    //this->Serialfind_port((Widget*)parent);
}
//void SerialPart::Serialfind_port(Widget *w)
//{

//}
void SerialPart::SerialRead_Date()
{
    QByteArray buf;
    buf = serialport_m->readAll();
   if(!buf.isEmpty())          //将数据显示到文本串口
    {
        //获取之前的文件信息
        QString str = w->getUi()->textBrowser->toHtml();
        if(str.size() >= 10 * 1024) str.clear();
       // qDebug()<<"textBrowser"<<str;
        //qDebug()<<"buf"<<buf[0];
        if(buf[0] == (char)'0')
        {
            qDebug()<<"buf"<<0;
           str += " \" <font color=\"#FF0000\">"+tr(buf)+"</font> \" ";
        }else if(buf[0] == (char)'1')
        {
            qDebug()<<"buf"<<1;
            str += " \"<font color=\"#00FF00\">"+tr(buf)+"</font> \" ";
        }
        w->getUi()->textBrowser->clear();

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


