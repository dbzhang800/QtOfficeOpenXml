#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("OpcPackageXmlEdit");
    a.setOrganizationName("QtOfficeOpenXml");
    MainWindow w;
    w.show();

    return a.exec();
}
