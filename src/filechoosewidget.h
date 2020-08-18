#ifndef FILECHOOSEWIDGET_H
#define FILECHOOSEWIDGET_H

#include "choosefilebutton.h"
#include <QWidget>
#include <QSettings>
#include <DLabel>
#include <QPushButton>
#include <DBlurEffectWidget>

DWIDGET_USE_NAMESPACE
extern QString color;

namespace Ui {
class FileChooseWidget;
}

class FileChooseWidget : public DBlurEffectWidget
{
    Q_OBJECT
signals:
    void packagesSelected(const QStringList files) const;

public:
    explicit FileChooseWidget(DBlurEffectWidget *parent = nullptr);
    ~FileChooseWidget();
private slots:
    void closeChanged();
    void cuter(QStringList selected_files);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void changedshow();
private:
    DLabel *m_iconImage {nullptr};
    DLabel *m_changedImage {nullptr};
    DLabel *m_dndTips {nullptr};
    DLabel *split_line {nullptr};
    QSettings m_settings;
    QList<QString> pathlist;
    QLineEdit *text;
    QPushButton *closeBtn;

};

#endif // WIDGET_H
