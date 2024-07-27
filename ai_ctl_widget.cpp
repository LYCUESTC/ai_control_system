#include "ai_ctl_widget.h"
#include "ui_ai_ctl_widget.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMenu>
#include <QMessageBox>
#include "listItemdata.h"
#include "itemdelegate.h"
#include "add_task_dialog.h"
#include "httpclient/httpclient.hpp"
#include "showform.h"
using namespace Bidstack::Http;

Ai_Ctl_Widget::Ai_Ctl_Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Ai_Ctl_Widget)
{
    ui->setupUi(this);
    ui->close_btn->setEnabled(false);
    ui->textEdit->setTextColor(Qt::white);
    QDateTime curDateTime = QDateTime::currentDateTime();
    ui->textEdit->append(">> " + curDateTime.toString("yyyy-MM-dd hh:mm:ss") + " Server start...");
    task_model_ = new QStandardItemModel();
    /* 设置列数 */
    task_model_->setColumnCount(4);
    task_model_->setHeaderData(0, Qt::Horizontal, "task_name");
    task_model_->setHeaderData(1, Qt::Horizontal, "device addr");
    task_model_->setHeaderData(2, Qt::Horizontal, "camera addr");
    task_model_->setHeaderData(3, Qt::Horizontal, "roi");
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{color:black;padding-left:4px;border:1px solid#6c6c6c;}");
    ui->tableView->setModel(task_model_);
    /* 设置列宽在可视界面自适应宽度 */

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    /* 行颜色交替显示 */
    ui->tableView->setAlternatingRowColors(true);
    auto verh = ui->tableView->verticalHeader();
    verh->setDefaultSectionSize(24);

    /* 不允许在图形界面修改内容 */
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* 显示表 */
    ui->tableView->show();
    pModel = new QStandardItemModel();

    init_data();

    connect(ui->start_btn, SIGNAL(clicked()), this, SLOT(start_server()));
    connect(ui->close_btn, SIGNAL(clicked()), this, SLOT(stop_server()));
    connect(ui->opentask,SIGNAL(clicked()), this, SLOT(start_task()));
    connect(ui->closetask,SIGNAL(clicked()), this, SLOT(stop_task()));
}

Ai_Ctl_Widget::~Ai_Ctl_Widget()
{
    delete ui;
}

void Ai_Ctl_Widget::slotCustomContextMenuRequested(const QPoint &pos)
{
    if(!((ui->listView->selectionModel()->selectedIndexes()).empty()))
    {
        auto curindex = ui->listView->currentIndex();
        qDebug()<<"curindex:"<<curindex.row()<<"\n";
        qDebug()<<"image path="<<index_image_[curindex.row()]<<"\n";
        menu->exec(ui->listView->mapToGlobal(pos));
        showForm = new ShowForm();
        showForm->show_image(index_image_[curindex.row()]);
        showForm->show();
    }
}
void Ai_Ctl_Widget::init_data()
{
    //打开右键菜单属性
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

    //右键菜单
    menu = new QMenu(ui->listView);
    menu->addAction("show image");
    //响应右键菜单信号槽
    connect(ui->listView, &QListView::customContextMenuRequested,
              this,&Ai_Ctl_Widget::slotCustomContextMenuRequested);


    MuItemDelegate *pItemDelegate = new MuItemDelegate(this);
    ui->listView->setItemDelegate(pItemDelegate);
    ui->listView->setModel(pModel);
    connect(ui->listView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(on_listView_clicked(const QModelIndex &)));
    QDateTime curDateTime = QDateTime::currentDateTime();
    ui->textEdit->append(">> " + curDateTime.toString("yyyy-MM-dd hh:mm:ss") + " init successful...");

}

void Ai_Ctl_Widget::add_data(MuItemData myitemdata)
{
    QStandardItem *pItem = new QStandardItem;
    pItem->setBackground(QBrush(QColor(Qt::red)));
    pItem->setData(QVariant::fromValue(myitemdata), Qt::UserRole+1);
    pModel->appendRow(pItem);

}
void Ai_Ctl_Widget::stop_server()
{
    httpServer_->destroyed();
    httpServer_->disconnect();
    httpServer_.reset();
    ui->start_btn->setEnabled(true);
    ui->close_btn->setEnabled(false);
    ui->flages->setStyleSheet("border-image: url(:/images/images/off.png);");
    QDateTime curDateTime = QDateTime::currentDateTime();
    ui->textEdit->append(">> " + curDateTime.toString("yyyy-MM-dd hh:mm:ss") + " Server stop...");
}
void Ai_Ctl_Widget::start_server()
{
    httpServer_ = std::make_shared<QHttpServer>();
    httpServer_->route("/hello", [](const QHttpServerRequest &request) {
        qDebug()<<"body:"<<request.body()<<"\n";
        return "Hello world";
    });
    httpServer_->route("/alarm/<arg>/", [this]( const QString task_name, const QString filename, const QHttpServerRequest &request)
    {

        QString save_file = "images/";
        save_file.append(task_name);
        QDir dir;
        if(dir.mkpath(save_file))//创建多级目录，如果已存在则会返回去true
        {
            qDebug()<<"mkdir path "<< save_file<< "\n";
        }
        save_file.append("/"+filename);
        qDebug()<<"save_file="<<save_file<<"\n";
        qDebug() << "1"<< request.headers().first()<<"\n";
        QFile file(save_file);
        file.open(QIODevice::WriteOnly);
        file.write(request.body().data(), request.body().size());        // write to stderr
        file.close();

        qDebug() << request.value("image/jpeg");
        MuItemData muitemdata;
        muitemdata.iconPath = save_file;
        muitemdata.songsNb = request.query().queryItemValue("name");
        muitemdata.singer = request.query().queryItemValue("alarm_time");
        QString data = muitemdata.singer.toLatin1();

        qDebug()<<"muitemdata.singer="<<data<<"\n";
        int index = index_image_.size();
        index_image_[index] = save_file;
        add_data(muitemdata);
        QString str = "{\"code\":0,\"msg\":\"successful\"}";
        return str;//request.remoteAddress().toString();
    });
    const auto port = httpServer_->listen(QHostAddress::Any, 6008);
    if (!port)
    {
        qDebug() << QCoreApplication::translate(
                    "QHttpServerExample", "Server failed to listen on a port.");
        return;
    }
    ui->flages->setStyleSheet("border-image: url(:/images/images/on.png);");
    ui->start_btn->setEnabled(false);
    ui->close_btn->setEnabled(true);
    qDebug() << QCoreApplication::translate(
                "QHttpServer start", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);
    QDateTime curDateTime = QDateTime::currentDateTime();
    ui->textEdit->append(">> " + curDateTime.toString("yyyy-MM-dd hh:mm:ss") + QCoreApplication::translate(
                             "QHttpServer start", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port));
}
bool Ai_Ctl_Widget::test()
{
    bool is_next = false;

    return is_next;
}
void Ai_Ctl_Widget::start_task()
{
    Add_Task_Dialog * ad = new Add_Task_Dialog();
    auto ret = ad->exec();

    if(ret == QDialog::Accepted)
    {
//        QStandardItem *item = new QStandardItem();
//        item->setData(ad->get_task_name(), 0);
//        task_model_->appendRow(item);
//        task_model_->setItem(task_model_->indexFromItem(item).row(),1,new QStandardItem(ad->get_edge_addr()));
//        task_model_->setItem(task_model_->indexFromItem(item).row(),2,new QStandardItem(ad->get_task_url()));
//        ui->tableView->setRowHeight(0,10);
#if 1
        HttpRequest *request = new HttpRequest();
        QMap<QString, QString> headers;
        /*
            {
                "task_id":1,
                "task_name":"task002",
                "type":1,
                "rtsp_url":"rtsp://192.168.11.139/person",
                "regions":[[0, 0, 1920, 0, 1920, 1080, 0, 1080]]
            }
        */
        QJsonObject json;
        json.insert("type", 0);
        json.insert("task_name", ad->get_task_name());
        json.insert("rtsp_url", ad->get_task_url());
        auto rect = ad->rect_;


        QString ret = QString::number(rect[0].x()) + "," + QString::number(rect[0].y()) + "," +
                      QString::number(rect[1].x()) + "," + QString::number(rect[1].y()) + "," +
                      QString::number(rect[2].x()) + "," + QString::number(rect[2].y()) + "," +
                      QString::number(rect[3].x()) + "," + QString::number(rect[3].y());
        qDebug()<<"["<<ret<<"]\n";
        QJsonArray jsonArray;
        QJsonArray reg;
        reg.append(rect[0].x());
        reg.append(rect[0].y());
        reg.append(rect[1].x());
        reg.append(rect[1].y());
        reg.append(rect[2].x());
        reg.append(rect[2].y());
        reg.append(rect[3].x());
        reg.append(rect[3].y());
        jsonArray.append(reg);
        json.insert("regions", jsonArray);

        // 构建 JSON 文档
        QJsonDocument document;
        document.setObject(json);
        QByteArray byteArray = document.toJson(QJsonDocument::Compact);
        QString json_str = QString(byteArray);
        qDebug()<<"json_str="<<json_str<<"\n";
        headers.insert("Content-Type", "application/json");
        request->setMethod("POST");
        QString ipc_url = "http://"+ad->get_edge_addr()+":"+ad->get_edge_port()+"/unmanned/operate";
        qDebug()<<"ipc_url="<<ipc_url<<"\n";
        request->setUrl(QUrl(ipc_url));
        request->setHeaders(headers);
        request->setProperty("Content-Type","application/json");
        //QString json = "{\"test\":1}";
        HttpBody *hbody = new HttpBody(json_str);

        request->setBody(hbody);
        HttpClient *client = new HttpClient();
        HttpResponse *response = client->send(request);
        if(response != nullptr)
        {
            qDebug()<<"http client response null\n";
            qDebug() << "Headers:" << response->headers();
            qDebug() << "HTTP Status:" << response->status();
            qDebug() << "Body:" << response->body()->toString();
            QJsonParseError err_rpt;
            QJsonDocument  root_Doc = QJsonDocument::fromJson(response->body()->toString().toLatin1(), &err_rpt);//字符串格式化为JSON

            if(err_rpt.error != QJsonParseError::NoError)
            {
                qDebug() << "JSON format error";
                QMessageBox::warning(nullptr, // 父窗口，nullptr表示没有父窗口
                                          "Warning", // 标题
                                          "Task startup failed.", // 文本
                                          QMessageBox::Ok // 默认按钮
                                          ); // 取消按钮

            }
            else    //JSON格式正确
            {
                qDebug() << "JSON：\n" << root_Doc;

                QJsonObject root_Obj = root_Doc.object();
                QJsonValue statusCode = root_Obj.value("statusCode");
                int code = statusCode.toInt();
                qDebug()<<"code="<<code<<"\n";
                if(code == 200)
                {
                    qDebug()<<"task_name:"<<ad->get_task_name()<<"task_url:"<<ad->get_task_url()<<"\n";
                    QStandardItem *item = new QStandardItem();
                    item->setData(ad->get_task_name(), 0);
                    task_model_->appendRow(item);
                    task_model_->setItem(task_model_->indexFromItem(item).row(),1,new QStandardItem(ad->get_edge_addr()+":"+ad->get_edge_port()));
                    task_model_->setItem(task_model_->indexFromItem(item).row(),2,new QStandardItem(ad->get_task_url()));
                    task_model_->setItem(task_model_->indexFromItem(item).row(),3,new QStandardItem(ad->get_roi()));
                }
            }
        }
        delete hbody;
        delete request;
        delete client;
#endif
    }
}

void Ai_Ctl_Widget::stop_task()
{
    QModelIndex cindex = ui->tableView->currentIndex();
    QModelIndex pindex;
    QString pstr;
    QString task_name = task_model_->data(task_model_->index(cindex.row(), 0)).toString();
    QString edge_addr = task_model_->data(task_model_->index(cindex.row(), 1)).toString();
    QString rtsp_url = task_model_->data(task_model_->index(cindex.row(), 0)).toString();
//    for (int i = 0; i < task_model_->columnCount(); i++ )
//    {
//        pindex = task_model_->index(cindex.row(), i);
//        pstr = task_model_->data(pindex).toString();
//        qDebug()<<"data is:"<<pstr<<"\n";
//    }

    HttpRequest *request = new HttpRequest();
    QMap<QString, QString> headers;
    /*
        {
            "isOpen": 1,
            "task_name":"test",
            "ipc_url":"rtsp://admin:lyc123456@192.168.1.64"
        }
    */
    QJsonObject json;
    json.insert("type", 1);
    json.insert("task_name", task_name);
    //json.insert("ipc_url", rtsp_url);

    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString json_str = QString(byteArray);
    qDebug()<<"json_str="<<json_str<<"\n";
    headers.insert("Content-Type", "application/json");
    request->setMethod("POST");
    QString ipc_url = "http://"+edge_addr+"/unmanned/operate";
    qDebug()<<"ipc_url="<<ipc_url<<"\n";
    request->setUrl(QUrl(ipc_url));
    request->setHeaders(headers);
    request->setProperty("Content-Type","application/json");

    HttpBody *hbody = new HttpBody(json_str);

    request->setBody(hbody);
    HttpClient *client = new HttpClient();
    HttpResponse *response = client->send(request);
    if(response != nullptr)
    {
        qDebug()<<"http client response null\n";
        qDebug() << "Headers:" << response->headers();
        qDebug() << "HTTP Status:" << response->status();
        qDebug() << "Body:" << response->body()->toString();
        QJsonParseError err_rpt;
        QJsonDocument  root_Doc = QJsonDocument::fromJson(response->body()->toString().toLatin1(), &err_rpt);//字符串格式化为JSON

        if(err_rpt.error != QJsonParseError::NoError)
        {
            qDebug() << "JSON格式错误";

        }
        else    //JSON格式正确
        {
            qDebug() << "JSON格式正确：\n" << root_Doc;

            QJsonObject root_Obj = root_Doc.object();
            QJsonValue statusCode = root_Obj.value("statusCode");
            int code = statusCode.toInt();
            qDebug()<<"code="<<code<<"\n";
            if(code == 200)
            {
                int iSel = cindex.row();
                task_model_->removeRow(iSel);
            }
        }
    }
    delete hbody;
    delete request;
    delete client;
}
void Ai_Ctl_Widget::show_image()
{
    QModelIndex cindex = ui->listView->currentIndex();
    qDebug()<<"cindex"<<cindex.column()<<"\n";
}
void Ai_Ctl_Widget::on_listView_clicked(const QModelIndex &index)
{
    qDebug()<<"cindex:"<<index.column()<<","<<index.row()<<"\n";
    qDebug()<<"image path="<<index_image_[index.row()]<<"\n";
}
