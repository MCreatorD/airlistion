#include "widget.h"
#include "ui_widget.h"
#include "iconhelper.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initForm();
    //初始化分割符
    this->initSplitterForm();

}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == ui->widgetTitle) {
            on_btnMenu_Max_clicked();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void Widget::initForm()
{
    this->setProperty("form", true);
    this->setProperty("canMove", true);

    //关闭界面原始按钮
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
        //this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint| Qt::WindowSystemMenuHint );
    this->setWindowFlags(Qt::FramelessWindowHint );
    //设置图标是使用fontawesome-webfont.ttf字体库 使用https://fontawesome.com/cheatsheet?from=io 去了解
    IconHelper::Instance()->setIcon(ui->labIco, QChar(0xf099), 30);
    IconHelper::Instance()->setIcon(ui->btnMenu_Min, QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->btnMenu_Max, QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QChar(0xf00d));

    //ui->widgetMenu->setVisible(false);
    ui->widgetTitle->installEventFilter(this);
    ui->widgetTitle->setProperty("form", "title");
    ui->widgetTop->setProperty("nav", "top");
    ui->labTitle->setText(QStringLiteral("空中数据监控软件"));
    //ui->labTitle->setFont(QFont("Microsoft Yahei", 20));
    this->setWindowTitle(ui->labTitle->text());

    //单独设置指示器大小
    int addWidth = 20;
    int addHeight = 10;
    int rbtnWidth = 15;
    int ckWidth = 13;
    int scrWidth = 12;
    int borderWidth = 3;
    QStringList qss;
    qss.append(QString("QComboBox::drop-down,QDateEdit::drop-down,QTimeEdit::drop-down,QDateTimeEdit::drop-down{width:%1px;}").arg(addWidth));
    qss.append(QString("QComboBox::down-arrow,QDateEdit[calendarPopup=\"true\"]::down-arrow,QTimeEdit[calendarPopup=\"true\"]::down-arrow,"
                       "QDateTimeEdit[calendarPopup=\"true\"]::down-arrow{width:%1px;height:%1px;right:2px;}").arg(addHeight));
    qss.append(QString("QRadioButton::indicator{width:%1px;height:%1px;}").arg(rbtnWidth));
    qss.append(QString("QCheckBox::indicator,QGroupBox::indicator,QTreeWidget::indicator,QListWidget::indicator{width:%1px;height:%1px;}").arg(ckWidth));
    qss.append(QString("QScrollBar:horizontal{min-height:%1px;border-radius:%2px;}QScrollBar::handle:horizontal{border-radius:%2px;}"
                       "QScrollBar:vertical{min-width:%1px;border-radius:%2px;}QScrollBar::handle:vertical{border-radius:%2px;}").arg(scrWidth).arg(scrWidth / 2));
    qss.append(QString("QWidget#widget_top>QToolButton:pressed,QWidget#widget_top>QToolButton:hover,"
                       "QWidget#widget_top>QToolButton:checked,QWidget#widget_top>QLabel:hover{"
                       "border-width:0px 0px %1px 0px;}").arg(borderWidth));
    qss.append(QString("QWidget#widgetleft>QPushButton:checked,QWidget#widgetleft>QToolButton:checked,"
                       "QWidget#widgetleft>QPushButton:pressed,QWidget#widgetleft>QToolButton:pressed{"
                       "border-width:0px 0px 0px %1px;}").arg(borderWidth));
    this->setStyleSheet(qss.join(""));

    QSize icoSize(32, 32);
    int icoWidth = 85;

    //设置顶部导航按钮
    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns) {
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    }

    ui->btnMain->click();
}

void Widget::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns) {
        if (btn == b) {
            btn->setChecked(true);
        } else {
            btn->setChecked(false);
        }
    }

    if (name == QStringLiteral("主界面")) {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == QStringLiteral("小工具")) {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == QStringLiteral("算法")) {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == QStringLiteral("协议")) {
        ui->stackedWidget->setCurrentIndex(3);
    } else if (name == QStringLiteral("退出")) {
        exit(0);
    }
}

void Widget::on_btnMenu_Close_clicked()
{
    close();
}

void Widget::on_btnMenu_Min_clicked()
{
    showMinimized();
}

void Widget::on_btnMenu_Max_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
    }

    this->setProperty("canMove", max);
    max = !max;
}

void Widget::initSplitterForm()
{
    m_Lframe = ui->labContent;
    m_Rframe=ui->textBrowser;
    m_sp= ui->splitter;

    //初始化分隔符
    m_pButton= new QPushButton(this); // after Splitter
    m_bInitShow=false;
    //设置初始位置
    //setBtnPos();
    m_l_R = 0;


    connect(m_sp,SIGNAL(splitterMoved(int,int)),this,SLOT(sl_splitterMoved(int,int)));
    connect(m_pButton,SIGNAL(clicked()),this,SLOT(sl_btnClicked()));

    m_Lframe->setMouseTracking(true);
    m_Rframe->setMouseTracking(true);
    m_sp->setMouseTracking(true);
    m_pButton->setMouseTracking(true);
    setMouseTracking(true);

}

void Widget::setBtnPos()//把Button移动到分割线中间位置
{
    //界面的
    int width=m_Lframe->width();
     int heigh=(m_Lframe->height())/2+ui->widgetTitle->height();
     qDebug()<<"m_Lframe width"<<m_Lframe->width();
     qDebug()<<"m_Lframe heigh"<<m_Lframe->height();
     qDebug()<<"width"<<width;
     qDebug()<<"heigh"<<heigh;

     m_pButton->move(width,heigh);
}

void Widget::setBtnIcon()
{
    if(m_Lframe->width()!=0)
     {
         m_pButton->setIcon(QIcon("://qss/psblack/add_left.png"));
     }
     else
     {
         m_pButton->setIcon(QIcon("://qss/psblack/add_right.png"));
    }
}

void Widget::showEvent(QShowEvent *event)
{
    qDebug()<<"showEvent";
    if(!m_bInitShow)
    {
        qDebug()<<"m_bInitShow";
        m_bInitShow=true;

        QPalette p2=m_Lframe->palette(); // initial the frame back round
        p2.setColor(QPalette::Window,Qt::gray);
        p2.setColor(QPalette::WindowText,Qt::red);
        m_Lframe->setPalette(p2);
        m_Lframe->setAutoFillBackground(true);
        //m_Lframe->resize(300,m_Lframe->height());
        //m_Lframe->setMaximumWidth(400);
        base= m_sp->sizes();

        m_pButton->setFixedSize(20,20);
        m_pButton->setIconSize(QSize(20,20));
        m_pButton->setVisible(true);
        m_pButton->setStyleSheet("border:none;");
        setBtnPos();  //设置button位置
        setBtnIcon(); //显示对应图片
    }

    QWidget::showEvent(event);
}

void Widget::resizeEvent(QResizeEvent *event)
{
    setBtnPos();
     QWidget::resizeEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
//    int isWidth  = m_pButton->pos().x()+m_pButton->width();
//    int isHeight = m_pButton->pos().y()+m_pButton->height();

//    int x=event->pos().x();
//    int y=event->pos().y();

//    if(x>m_pButton->pos().x()&&x<isWidth&&y>m_pButton->y()&&y<isHeight)
//    {
//        m_pButton->setVisible(true);
//    }
//    else
//    {
//        m_pButton->setVisible(false);
//    }
}


void Widget::sl_splitterMoved(int pos, int index)
{

    setBtnPos();
    qDebug()<<"m_Lframe->width()"<<m_Lframe->width();
    if(m_Lframe->width()==0&&(m_l_R == 0))
    {
        m_l_R = 1;
        setBtnIcon();
        qDebug()<<"setBtnIcon 1";
    }else if(m_Lframe->width()!=0&&(m_l_R == 1))
    {
        m_l_R = 0;
        setBtnIcon();
        qDebug()<<"setBtnIcon 0";
    }
}
void Widget::sl_btnClicked()//设置左右布局的宽度
{
    qDebug()<<"sl_btnClicked";
    // static  QList<qint32> base= m_sp->sizes(); //keep the initial pos
     QList<qint32>  newSize;
//     QList<QString> list;
//     list << "one" << "two" << "three";
//     // list: ["one", "two", "three"]
     if(m_Lframe->width()!=0)
     {
         //newSize<<base.at(0)+base.at(1)<<0;
         newSize<<0<<0;
     }
     else
     {
         newSize<<base.at(0)<<base.at(1);
     }
     //左右部件大小
     qDebug()<<"newSize"<<newSize;
     m_sp->setSizes(newSize);  //move the SPLITTER

     setBtnPos();
     setBtnIcon();
}

