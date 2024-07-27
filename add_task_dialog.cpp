#include "add_task_dialog.h"
#include "ui_add_task_dialog.h"
#include "draw_image_roi_dialog.h"
Add_Task_Dialog::Add_Task_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Task_Dialog)
{
    ui->setupUi(this);
    connect(ui->draw_btn, SIGNAL(clicked()), this, SLOT(draw_ui()));
}

Add_Task_Dialog::~Add_Task_Dialog()
{
    delete ui;
}
void Add_Task_Dialog::ok()
{
    close();
}
void Add_Task_Dialog::channel()
{
    channel();
}
QString Add_Task_Dialog::get_task_name()
{
    return ui->line_task_name->text();
}
QString Add_Task_Dialog::get_task_url()
{
    return ui->line_camera_addr->text();
}
QString Add_Task_Dialog::get_edge_addr()
{
    return ui->line_edge_addr->text();
}
QString Add_Task_Dialog::get_edge_port()
{
    return ui->line_port->text();
}
QString Add_Task_Dialog::get_roi()
{
    return ui->line_roi->text();
}
void Add_Task_Dialog::draw_ui()
{
    Draw_Image_Roi_Dialog * dw = new Draw_Image_Roi_Dialog();
    auto ret = dw->exec();
    if(ret == QDialog::Accepted)
    {
        QRect rect = dw->get_roi();
        float scale = dw->get_scale();

        int x0 = rect.x() / scale;
        int y0 = rect.y() / scale;
        rect_[0].setX(x0);
        rect_[0].setY(y0);

        int x1 = (rect.x() + rect.width()) / scale;
        int y1 = y0;
        rect_[1].setX(x1);
        rect_[1].setY(y1);
        int x2 = x1;
        int y2 = (rect.y() + rect.height()) / scale;
        rect_[2].setX(x2);
        rect_[2].setY(y2);

        int x3 = x0;
        int y3 = y2;
        rect_[3].setX(x3);
        rect_[3].setY(y3);

        QString ret = QString::number(x0) + "," + QString::number(y0) + "," +
                      QString::number(x1) + "," + QString::number(y1) + "," +
                      QString::number(x2) + "," + QString::number(y2) + "," +
                      QString::number(x3) + "," + QString::number(y3);
        qDebug()<<"["<<ret<<"]\n";
        ui->line_roi->setText(ret);
    }
}
