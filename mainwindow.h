#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include "filechoosewidget.h"
#include <QMenu>

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:

private:
    FileChooseWidget *w;
    QMenu *m_menu;
    QMenu *menu_how=new QMenu;
    QActionGroup *howGroup;
    QAction *m_black;
    QAction *m_color;
};

#endif // MAINWINDOW_H
