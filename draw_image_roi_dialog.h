#ifndef DRAW_IMAGE_ROI_DIALOG_H
#define DRAW_IMAGE_ROI_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QGraphicsLineItem>
#include "GraphicsView.h"
#include <QDebug>
namespace Ui {
class Draw_Image_Roi_Dialog;
}

class Draw_Image_Roi_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Draw_Image_Roi_Dialog(QWidget *parent = nullptr);
    ~Draw_Image_Roi_Dialog();
    void CreateWidget();

    void ShowCentralCross(double centerWidth, double centerHeight,QGraphicsScene* scene, QGraphicsLineItem* lineRow, QGraphicsLineItem* linecCol, bool isShow = true);
    QRect get_roi();
    float get_scale();
private slots:
    void on_button_clicked();
    void on_button1_clicked();

private:
    GraphicsView* gview;
    QPen* pen;
    QImage m_img;
    double centerWidth;
    double centerHeight;
    int width = 640;
    int height = 640;
private:
    Ui::Draw_Image_Roi_Dialog *ui;
};

#endif // DRAW_IMAGE_ROI_DIALOG_H
