#ifndef AI_CTL_WIDGET_H
#define AI_CTL_WIDGET_H

#include <QWidget>
#include <memory>
#include <QStandardItemModel>
#include <QMenu>
#include "qhttpserver.h"
#include "listItemdata.h"
#include "showform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Ai_Ctl_Widget; }
QT_END_NAMESPACE

class Ai_Ctl_Widget : public QWidget
{
    Q_OBJECT

public:
    Ai_Ctl_Widget(QWidget *parent = nullptr);
    ~Ai_Ctl_Widget();
    void init_data();

private slots:
    void start_server();
    void stop_server();

    void start_task();
    void stop_task();

    void show_image();
    void on_listView_clicked(const QModelIndex &index);
    void slotCustomContextMenuRequested(const QPoint &pos);
private:
    void add_data(MuItemData myitemdata);
    bool test();
signals:
    void receiveData();
private:
    std::shared_ptr<QHttpServer> httpServer_;
    QStandardItemModel *pModel;
    QStandardItemModel* task_model_;
    QMap<int, QString> index_image_;
    QMenu * menu;
    ShowForm * showForm;
private:
    Ui::Ai_Ctl_Widget *ui;
};
#endif // AI_CTL_WIDGET_H
