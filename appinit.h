#ifndef APPINIT_H
#define APPINIT_H

#include <QObject>
#include <QMouseEvent>
class AppInit : public QObject
{
    Q_OBJECT
public:
    static AppInit *Instance();
    explicit AppInit(QObject *parent = 0);    

    void start();

protected:
    bool eventFilter(QObject *obj, QEvent *evt);
    bool dealmouseMoveEvent(QObject *obj, QMouseEvent *event);
private:
    static AppInit *self;

signals:

public slots:
};

#endif // APPINIT_H
