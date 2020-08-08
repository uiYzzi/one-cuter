#include "mainwindow.h"
#include <DTitlebar>
#include <DMainWindow>
#include <QDragEnterEvent>
#include "filechoosewidget.h"
DWIDGET_USE_NAMESPACE
QString color="-C";
MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent),
        m_menu(new QMenu),
        m_black(new QAction("黑白模式")),
        m_color(new QAction("彩色模式"))

{
    //按钮互斥
    howGroup = new QActionGroup(this);
    howGroup->addAction(m_black);
    howGroup->addAction(m_color);
    menu_how->setTitle(tr("黑白/彩色"));
    menu_how->addAction(m_black);
    menu_how->addAction(m_color);
    //窗体毛玻璃
    //setAttribute(Qt::WA_TranslucentBackground);
    //setTranslucentBackground(true);
    //setEnableBlurWindow(true);
    m_black->setCheckable(true);
    m_color->setCheckable(true);
    m_color->setChecked(true);
    m_menu->addMenu(menu_how);
    titlebar()->setMenu(m_menu);
    w = new FileChooseWidget;
    connect(m_black,&QAction::triggered,[=](){m_color->setChecked(false);color="-c 10 -e H";});
    connect(m_color,&QAction::triggered,[=](){m_black->setChecked(false);color="-C";});
    resize(w->size()); //设置窗口大小
    //setWindowOpacity(0.5);
    setFixedSize(480, 380);
    //setTitlebarShadowEnabled(false);//取消标题栏阴影
    setCentralWidget(w);
    DTitlebar *tb = titlebar();
    tb->setTitle("");
    tb->setIcon(QIcon::fromTheme(":/new/prefix1/images/icon.svg"));
    //tb->setBlurBackground(true);//启用标题栏毛玻璃

}

MainWindow::~MainWindow()
{

}
