#include "dialog.h"
#include <QApplication>
#include<QIcon>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    w.setWindowIcon(QIcon(QStringLiteral(":/rc/pic4.png")));
    w.show();

    return a.exec();
}
