#define RUNDIR "/opt/apps/top.yzzi.onecuter/files/"
#include "filechoosewidget.h"
#include <DFileDialog>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QPixmap>
#include <QDebug>
#include <DLabel>
#include <QImageReader>
#include <QVBoxLayout>
#include <DApplicationHelper>
#include <sstream>
#include "utils.h"
//以下大量代码来自https://github.com/linuxdeepin/deepin-deb-installer

using namespace std;
QHash<QString, QPixmap> m_imgCacheHash;
FileChooseWidget::FileChooseWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setAcceptDrops(true);
    m_iconImage = new DLabel(this);//图标
    m_iconImage->setFixedSize(239, 170);
    m_iconImage->setPixmap(Utils::renderSVG(":/new/prefix1/images/unchanged.png", QSize(239, 170)));

    m_changedImage = new DLabel(this);//成功图标
    m_changedImage->setFixedSize(157, 170);
    m_changedImage->setPixmap(Utils::renderSVG(":/new/prefix1/images/changed.png", QSize(157, 170)));


    DGuiApplicationHelper *guiAppHelp = DGuiApplicationHelper::instance();
    text = new QLineEdit(this);
    text->setPlaceholderText("请先在此填入二维码的网址或文字内容");
    text->setMinimumWidth(400);
    text->setAlignment(Qt::AlignCenter);

    closeBtn = new QPushButton(this);
    closeBtn->setText(tr("返回"));



#ifdef SHOWBORDER
    m_chooseFileBtn->setStyleSheet("QPushButton{border:1px solid black;}");
#endif


    QVBoxLayout *centralLayout = new QVBoxLayout(this);
    centralLayout->addSpacing(55);
    centralLayout->addWidget(m_iconImage);
    centralLayout->setAlignment(m_iconImage, Qt::AlignHCenter);
    centralLayout->addWidget(m_changedImage);
    centralLayout->setAlignment(m_changedImage, Qt::AlignHCenter);
    m_changedImage->hide();

    centralLayout->addSpacing(30);
    centralLayout->addWidget(m_dndTips);
    centralLayout->setAlignment(m_dndTips, Qt::AlignHCenter);

    centralLayout->addSpacing(9);
    centralLayout->addWidget(closeBtn);
    centralLayout->setAlignment(closeBtn, Qt::AlignHCenter);
    closeBtn->hide();
    centralLayout->addWidget(text);
    centralLayout->setAlignment(text, Qt::AlignHCenter);

    centralLayout->addStretch();

    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(centralLayout);

    connect(closeBtn, &ChooseFileButton::clicked, this, &FileChooseWidget::closeChanged);

}

void FileChooseWidget::dragEnterEvent(QDragEnterEvent *e)
{
    //对拖放事件进行筛选
    if (true)
    {
        e->acceptProposedAction();	//放行，否则不会执行dropEvent()函数
    }
}


void FileChooseWidget::dropEvent(QDropEvent *e)
{
    foreach(auto item,pathlist)
    {
        pathlist.removeOne(item);
        qDebug()<<"qlist.size()="<<pathlist.size();
    }
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return ;
    qDebug()<< urls.size();
    foreach (QUrl u, urls)
    {
        QString filepath = u.toLocalFile();
        pathlist.append(filepath);
    }
    pathlist = pathlist.toSet().toList();
    emit FileChooseWidget::cuter(pathlist);
}


void FileChooseWidget::cuter(QStringList selected_files)
{
    qDebug()<<selected_files;
    ostringstream mklk;
    QString pathStr;
    QString tmp;
    QString qmklk;
    QString textdata=text->text();
    int cmd;

        tmp = selected_files.at(0);
        qDebug()<<"tmp ="<< tmp;
        pathStr=tmp.left(tmp.lastIndexOf("/"));
        qDebug()<<pathStr;
        mklk.str("");

        if(textdata=="")
        {

        }
        else
        {
            mklk<<"python3 "<<RUNDIR<<"CuteR.py "<<color.toStdString()<<" -o \""<<pathStr.toStdString()<<"/sample_output.png"<<"\" \""<<tmp.toStdString()<<"\" "<<textdata.toStdString();
            //mklk<<RUNDIR<<"tinypng"<<" -f \""<<tmp.toStdString()<<"\" -d \""<<pathStr.toStdString()<<"\"";
            qmklk = QString::fromStdString(mklk.str());
            qDebug()<<qmklk;
            cmd=system(qmklk.toUtf8());
            qDebug()<<cmd;
            FileChooseWidget::changedshow();
        }
}

void FileChooseWidget::changedshow()
{
    m_iconImage->hide();
    text->hide();
    m_changedImage->show();
    closeBtn->show();
}

void FileChooseWidget::closeChanged()
{
    m_iconImage->show();
    text->show();
    m_changedImage->hide();
    closeBtn->hide();
    text->clear();
}

FileChooseWidget::~FileChooseWidget()
{

}

