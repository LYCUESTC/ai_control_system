#include "showform.h"
#include "ui_showform.h"

ShowForm::ShowForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowForm)
{
    ui->setupUi(this);
}

ShowForm::~ShowForm()
{
    delete ui;
}
void ShowForm::show_image(QString filepath)
{
    QImage image;
    if (image.load(filepath))
    {
        //仅仅只是导入之后的图片仍然是原来的大小，这个时候我们需要缩放
        ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->size()));
    }
}
