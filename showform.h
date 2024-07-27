#ifndef SHOWFORM_H
#define SHOWFORM_H

#include <QWidget>

namespace Ui {
class ShowForm;
}

class ShowForm : public QWidget
{
    Q_OBJECT

public:
    explicit ShowForm(QWidget *parent = nullptr);
    ~ShowForm();
public slots:
    void show_image(QString filepath);
private:
    Ui::ShowForm *ui;
};

#endif // SHOWFORM_H
