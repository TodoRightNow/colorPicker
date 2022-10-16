#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w(QColor(98, 0, 0));
    w.show();
    return a.exec();
}
