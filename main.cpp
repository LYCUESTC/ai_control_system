#include "ai_ctl_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ai_Ctl_Widget w;
    w.show();
    return a.exec();
}
