﻿#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void initForm();
    void buttonClick();

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();


private:
    Ui::Widget *ui;

public:
    void setBtnPos();
    void setBtnIcon();
private:
    //分割符中的按钮after Splitter
    QPushButton *m_pButton;
    //右边窗口 接ui
    QFrame *m_Rframe;
    //左边窗口 接ui
    QLabel *m_Lframe;
    //分割器
    QSplitter *m_sp;

    //变量规定是否初始化
    bool m_bInitShow;
    //左右布局的宽度
    QList<qint32> base;
    //分割符中间按钮的方向
    bool m_l_R;

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void initSplitterForm();
    void sl_splitterMoved(int pos, int index);
    void sl_btnClicked();
};

#endif // WIDGET_H
