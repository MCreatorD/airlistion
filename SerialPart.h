#ifndef SERIALPART_H
#define SERIALPART_H

#include <QObject>
#include "widget.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "ui_widget.h"
#include <QDebug>
#include <QTimer>
/*
 * 封装自己的串口类
 * 1. 查找串口资源返回值QStringList
 * 2. 打开关闭串口
 * 3. 启动读数据的线程 处理读数据和解析数据拼接数据
*/
namespace Ui {
class Widget;
}
class SerialPart : public QObject
{
    Q_OBJECT
public:
    explicit SerialPart(QObject *parent = 0);

signals:
    void resultReady(const QString &result);
    void CommitList(QStringList);
public slots:
    void comdelay();
    void SerialRead_Date();       //读取串口数据
    void doWork(const QString &parameter) ;
public://对于串口读取查找
    QStringList Serialfind_port();       //查找可用串口
    QStringList SearchComitList(QString Buf);
public:
    inline QSerialPort *getserialport()
    {
        return serialport_m;
    }
 private:
    QSerialPort *serialport_m;
    QByteArray buf;
    bool hasData;
    QString subBuf;
    //QTimer time;
 //链接界面
    class Widget *w;
  public:
    inline void setuiWidget(class Widget *uiw)
    {
        w = uiw;
    }
};

#endif // SERIALPART_H
