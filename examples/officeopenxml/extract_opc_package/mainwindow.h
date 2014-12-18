#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opcpackage.h>
#include <opcpackagepart.h>
using namespace QtOfficeOpenXml;

namespace Ui {
class MainWindow;
}
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onOpen();
    void onAbout();
    void onItemClicked(QTreeWidgetItem *item, int column);

private:
    void visit(Opc::PackagePart *part, QTreeWidgetItem *parentItem);

    Ui::MainWindow *ui;
    QWidget *m_partWidget;
    Opc::Package *m_package;
};

#endif // MAINWINDOW_H
