#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDebug>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_partWidget(0),m_package(0)
{
    ui->setupUi(this);

    connect(ui->action_Open, SIGNAL(triggered()), SLOT(onOpen()));
    connect(ui->action_About, SIGNAL(triggered()), SLOT(onAbout()));

    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(onItemClicked(QTreeWidgetItem*,int)));
}

MainWindow::~MainWindow()
{
    delete m_package;
    delete m_partWidget;
    delete ui;
}

void MainWindow::onOpen()
{
    QSettings settings("opcpackagedemo.ini", QSettings::IniFormat);
    const QString path = settings.value("lastFile").toString();

    QString fn = QFileDialog::getOpenFileName( 0, tr("Open OPC Package"), path, "Office files (*.xlsx *.docx *.pptx);;All files(*.*)");
    if (fn.isEmpty())
        return;

    m_package = Opc::Package::open(fn, QIODevice::ReadOnly);
    if (!m_package->isOpen()) {
        //Error occur.
        statusBar()->showMessage(tr("Fail to open the package %1").arg(fn));
        return;
    }
    settings.setValue("lastFile", fn);
    ui->treeWidget->clear();

    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget, QStringList()<<"/"<<"", 0);
    rootItem->setExpanded(true);
    foreach(Opc::PackageRelationship *relation, m_package->relationships()) {
        Opc::PackagePart *part = m_package->part(relation->target());
        if (part)
            visit(part, rootItem);
    }
}

void MainWindow::visit(Opc::PackagePart *part, QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parentItem, QStringList()<<part->partName()<<part->contentType(), 1);
    item->setExpanded(true);
    foreach(Opc::PackageRelationship *relation, part->relationships()) {
        Opc::PackagePart *part = m_package->part(relation->target());
        if (part)
            visit(part, item);
    }
}

void MainWindow::onAbout()
{
    QMessageBox::information(this, tr("About"), tr("This example is designed to show how to used Opc::Package class."));
}

void MainWindow::onItemClicked(QTreeWidgetItem *item, int /*column*/)
{
    if (item->type() != 1)
        return;

    Opc::PackagePart *part = m_package->part(item->text(0));
    if (!part)
        statusBar()->showMessage(tr("Error open the part %1").arg(item->text(0)), 2000);

    QByteArray data = part->getDevice()->readAll();
    part->releaseDevice();

    if (m_partWidget) {
        delete m_partWidget;
        m_partWidget = 0;
    }
    qDebug()<<part->contentType();

    if (part->contentType().endsWith("xml") ||
            part->contentType().startsWith("plain")) {
        QPlainTextEdit *edit = new QPlainTextEdit(QString::fromUtf8(data));
        m_partWidget = edit;
    } else if (part->contentType().startsWith("image")) {
        QLabel *label = new QLabel();
        label->setPixmap(QPixmap::fromImage(QImage::fromData(data)));
        m_partWidget = label;
    } else {
        //Show in bin edit mode.
        m_partWidget = new QPlainTextEdit(QString::fromLatin1(data.toHex()));
    }

    m_partWidget->setWindowTitle(part->partName());
    m_partWidget->show();
}
