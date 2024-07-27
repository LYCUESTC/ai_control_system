#ifndef ADD_TASK_DIALOG_H
#define ADD_TASK_DIALOG_H

#include <QDialog>

namespace Ui {
class Add_Task_Dialog;
}

class Add_Task_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Task_Dialog(QWidget *parent = nullptr);
    ~Add_Task_Dialog();
    QString get_roi();
    QRect rect_[4];
public slots:
    void ok();
    void channel();
    QString get_task_name();
    QString get_task_url();
    QString get_edge_addr();
    QString get_edge_port();

    void draw_ui();
    //void exit_dig();
private:

private:
    Ui::Add_Task_Dialog *ui;
};

#endif // ADD_TASK_DIALOG_H
