#include <QApplication>
#include <QTextCodec>
#include "serverwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec1 = QTextCodec::codecForName("utf8");
    ServerWindow serverWin;
    serverWin.show();
    return a.exec();
}
