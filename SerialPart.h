#ifndef SERIALPART_H
#define SERIALPART_H

#include <QObject>
#include "widget.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "ui_widget.h"
#include <QDebug>
#include <QTimer>

namespace Ui {
class Widget;
}
class SerialPart : public QObject
{
    Q_OBJECT
public:
    explicit SerialPart(QObject *parent = 0);

signals:

public slots:
    void comdelay();
    void SerialRead_Date();       //读取串口数据
public://对于串口读取查找
    //void SerialRead_Date(class Widget *w);       //读取串口数据
    void Serialfind_port();       //查找可用串口
public:
    inline QSerialPort *getserialport()
    {
        return serialport_m;
    }
 private:
    QSerialPort *serialport_m;
    QByteArray buf;
    QTimer time;
 //链接界面
    class Widget *w;
  public:
    inline void setuiWidget(class Widget *uiw)
    {
        w = uiw;
    }
};

#endif // SERIALPART_H
