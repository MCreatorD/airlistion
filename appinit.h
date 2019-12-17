#ifndef APPINIT_H
#define APPINIT_H

#include <QObject>
#include <QMouseEvent>

#define PADDING 2
enum Direction { UP=0, DOWN=1, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE };

class AppInit : public QObject
{
    Q_OBJECT
public:
    static AppInit *Instance();
    explicit AppInit(QObject *parent = 0);    

    void start();

protected:
    bool eventFilter(QObject *obj, QEvent *evt);

private:
    static AppInit *self;

signals:

public slots:

public:
    void region(QObject *obj,const QPoint &cursorGlobalPoint);
private:
    bool isLeftPressDown;  // 判断左键是否按下
    QPoint dragPosition;   // 窗口移动拖动时需要记住的点
    Direction dir;        // 窗口大小改变时，记录改变方向

public slots:
    void dealmouseReleaseEvent(QObject *obj,QMouseEvent *event);
    void dealmouseMoveEvent(QObject *obj,QMouseEvent *event);
    void dealmousePressEvent(QObject *obj,QMouseEvent *event);
};

#endif // APPINIT_H
