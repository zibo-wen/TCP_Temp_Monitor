#include "widget.h"
#include <QDesktopWidget>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
    w.show();
    return a.exec();
}
