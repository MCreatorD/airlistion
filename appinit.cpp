#include "appinit.h"
#include "qmutex.h"
#include "qapplication.h"
#include "qevent.h"
#include "qwidget.h"

AppInit *AppInit::self = 0;
AppInit *AppInit::Instance()
{
    if (!self) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!self) {
            self = new AppInit;
        }
    }

    return self;
}

AppInit::AppInit(QObject *parent) : QObject(parent),isLeftPressDown(false)
{
}

static QPoint mousePoint;
static bool mousePressed = false;

bool AppInit::eventFilter(QObject *obj, QEvent *evt)
{
    QWidget *w = (QWidget *)obj;
    if (!w->property("canMove").toBool()) {
        return QObject::eventFilter(obj, evt);
    }

    //static QPoint mousePoint;
    //static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        dealmousePressEvent(w,event);
//        if (event->button() == Qt::LeftButton) {
//            mousePressed = true;
//            mousePoint = event->globalPos() - w->pos();
//            return true;
//        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
      dealmouseReleaseEvent(w,event);
//        mousePressed = false;
//        return true;
    } else if (event->type() == QEvent::MouseMove) {
//        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
//            w->move(event->globalPos() - mousePoint);
//            return true;
//        }
            dealmouseMoveEvent(w,event);
    }

    return QObject::eventFilter(obj, evt);
}

void AppInit::start()
{
    qApp->installEventFilter(this);
}



void AppInit::region(QObject *obj,const QPoint &cursorGlobalPoint)
{
    QWidget *w = (QWidget *)obj;
    // 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
    QRect rect = w->rect();
    QPoint tl =  w->mapToGlobal(rect.topLeft());
    QPoint rb =  w->mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 左上角
        dir = LEFTTOP;

        w->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        dir = RIGHTBOTTOM;
        w->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        dir = LEFTBOTTOM;
        w->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 右上角
        dir = RIGHTTOP;
        w->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        dir = LEFT;
        w->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        dir = RIGHT;
        w->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        // 上边
        dir = UP;
        w->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
        dir = DOWN;
        w->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        dir = NONE;
        w->setCursor(QCursor(Qt::ArrowCursor));
    }
}



void AppInit::dealmousePressEvent(QObject *obj, QMouseEvent *event)
{
    QWidget *w = (QWidget *)obj;
    switch(event->button()) {
     case Qt::LeftButton:
         isLeftPressDown = true;
         if(dir != NONE) {
             w->mouseGrabber();
         } else {
             dragPosition = event->globalPos() - w->frameGeometry().topLeft();
         }
         break;
     case Qt::RightButton:
         //w->close();
         break;
     default:
         //QWidget::mousePressEvent(event);
        break;
     }

}

void AppInit::dealmouseReleaseEvent(QObject *obj, QMouseEvent *event)
{
    QWidget *w = (QWidget *)obj;
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        if(dir != NONE) {
            w->releaseMouse();
            w->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void AppInit::dealmouseMoveEvent(QObject *obj, QMouseEvent *event)
{
    QWidget *w = (QWidget *)obj;
//    if (mousePressed && (event->buttons() && Qt::LeftButton)) {
//        w->move(event->globalPos() - mousePoint);
//        event->accept();
//    }
     QPoint gloPoint = event->globalPos();
       QRect rect = w->rect();
       QPoint tl = w->mapToGlobal(rect.topLeft());
       QPoint rb = w->mapToGlobal(rect.bottomRight());

       if(!isLeftPressDown) {
            region(obj,gloPoint);
       } else {

           if(dir != NONE) {
               QRect rMove(tl, rb);

               switch(dir) {
               case LEFT:
                   if(rb.x() - gloPoint.x() <= w->minimumWidth())
                       rMove.setX(tl.x());
                   else
                       rMove.setX(gloPoint.x());
                   break;
               case RIGHT:
                   rMove.setWidth(gloPoint.x() - tl.x());
                   break;
               case UP:
                   if(rb.y() - gloPoint.y() <= w->minimumHeight())
                       rMove.setY(tl.y());
                   else
                       rMove.setY(gloPoint.y());
                   break;
               case DOWN:
                   rMove.setHeight(gloPoint.y() - tl.y());
                   break;
               case LEFTTOP:
                   if(rb.x() - gloPoint.x() <= w->minimumWidth())
                       rMove.setX(tl.x());
                   else
                       rMove.setX(gloPoint.x());
                   if(rb.y() - gloPoint.y() <= w->minimumHeight())
                       rMove.setY(tl.y());
                   else
                       rMove.setY(gloPoint.y());
                   break;
               case RIGHTTOP:
                   rMove.setWidth(gloPoint.x() - tl.x());
                   rMove.setY(gloPoint.y());
                   break;
               case LEFTBOTTOM:
                   rMove.setX(gloPoint.x());
                   rMove.setHeight(gloPoint.y() - tl.y());
                   break;
               case RIGHTBOTTOM:
                   rMove.setWidth(gloPoint.x() - tl.x());
                   rMove.setHeight(gloPoint.y() - tl.y());
                   break;
               default:
                   break;
               }
               w->setGeometry(rMove);
           } else {
               w->move(event->globalPos() - dragPosition);
               event->accept();
           }
       }
       //QDialog::mouseMoveEvent(event);

}


