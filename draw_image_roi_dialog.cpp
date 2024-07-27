#include "draw_image_roi_dialog.h"
#include "ui_draw_image_roi_dialog.h"

Draw_Image_Roi_Dialog::Draw_Image_Roi_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Draw_Image_Roi_Dialog)
{
    ui->setupUi(this);
    CreateWidget();

    QObject::connect(ui->select_btn, SIGNAL(clicked()), this, SLOT(on_button_clicked()));
    QObject::connect(ui->clear_btn,  SIGNAL(clicked()), this, SLOT(on_button1_clicked()));
   //QObject::connect(ui->show_btn,   SIGNAL(clicked()), this, SLOT(show_roi()));
}

Draw_Image_Roi_Dialog::~Draw_Image_Roi_Dialog()
{
    delete ui;
}

void Draw_Image_Roi_Dialog::CreateWidget()
{
    gview = new GraphicsView(this);
    gview->setMaximumSize(640,640);
    gview->setMinimumSize(640,640);
    pen = new QPen();
    ui->verticalLayout->addWidget(gview);

}

void Draw_Image_Roi_Dialog::ShowCentralCross(double centerWidth, double centerHeight,QGraphicsScene* scene, QGraphicsLineItem*lineRow, QGraphicsLineItem* linecCol, bool isShow)
{
    if (isShow)
    {
        qDebug()<<"centerWidth="<<centerWidth<<",centerHeight="<<centerHeight<<",lineRow="<<lineRow<<",linecCol="<<linecCol<<"\n";

      /*  pen->setColor(QColor(255, 0, 255));
        pen->setWidth(2);


        lineRow->setLine(-50, 0, 50, 0);
        linecCol->setLine(0, 50, 0, -50);

        lineRow->setPen(*pen);
        linecCol->setPen(*pen);

        scene->addItem(lineRow);
        scene->addItem(linecCol);*/

        //QPainter *painter = new QPainter(&m_img);

        pen->setWidth(2);
        pen->setColor(Qt::blue);
        /*painter->setPen(*pen);
        painter->drawLine(-50, 0, 50, 0);*/
        scene->addLine(0,0, 50, 50, *pen);
    }
}


void Draw_Image_Roi_Dialog::on_button_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, "open", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "image (*.bmp *.png *.jpg)");

    if (fileName.isEmpty())
    {
        return;
    }

    QImage image(fileName);

    qDebug()<<"image["<<image.width()<<","<<image.height()<<"]\n";
    width = image.width();
    height = image.height();
    gview->setBackImage(image);
}

void Draw_Image_Roi_Dialog::on_button1_clicked()
{

    gview->clear_all();
}

QRect Draw_Image_Roi_Dialog::get_roi()
{
    std::map<int, QRect> roi = gview->getRoi();
    auto scale = gview->GetScaledValue();
    qDebug()<<"scale="<<scale<<"\n";
    qDebug()<<"roi.size="<<roi.size()<<"\n";

    QRect rect = roi[0];

    return rect;

}
float Draw_Image_Roi_Dialog::get_scale()
{
    return gview->GetScaledValue();
}
