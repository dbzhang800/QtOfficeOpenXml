/****************************************************************************
**
** Copyright (C) 2014-2015 Debao Zhang <hello@debao.me>
**
** This file is part of the QtOfficeOpenXml library.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPLv2 included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
****************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "binedit.h"
#include "imagewidget.h"
#include "mcexmlwidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QCloseEvent>
#include <QFontMetrics>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_package(0)
{
    ui->setupUi(this);
    ui->partContentSmartButton->hide();

    createActions();
    createMenuBar();

    connect(ui->partListWidget, SIGNAL(currentTextChanged(QString)), SLOT(onPartChanged(QString)));
    connect(ui->partContentButton, SIGNAL(clicked()), SLOT(onShowContentButtonClicked()));
    connect(ui->partContentSmartButton, SIGNAL(clicked()), SLOT(onShowContentSmartButtonClicked()));

    loadSettings();
}

MainWindow::~MainWindow()
{
    if (m_package)
        closePackage();
    delete ui;
}

void MainWindow::onActionOpenTriggered()
{
    QString fn = QFileDialog::getOpenFileName( 0, tr("Open OPC Package"), m_recentFilesList.isEmpty() ? "" : m_recentFilesList[0], "Office files (*.xlsx *.docx *.pptx);;All files(*.*)");
    if (fn.isEmpty())
        return;

    openPackage(fn);
}

void MainWindow::onActionRecentFileTriggered()
{
    QAction *act = qobject_cast<QAction *>(sender());
    QString fileName = act->data().toString();

    if (!openPackage(fileName)) {
        m_recentFilesList.removeOne(fileName);
        updateRecentFilesMenu();
    }
}

bool MainWindow::openPackage(const QString &name)
{
    if (m_package)
        closePackage();

    m_package = Opc::Package::open(name, QIODevice::ReadOnly);
    if (!m_package) {
        //Error occur.
        statusBar()->showMessage(tr("Fail to open the package %1").arg(name));
        setWindowTitle("Demo for Opc Package");
        return false;
    }

    m_recentFilesList.removeOne(name);
    m_recentFilesList.insert(0, name);
    if (m_recentFilesList.size() > 50)
        m_recentFilesList.removeLast();
    updateRecentFilesMenu();

    setWindowTitle(QString("%1 - Demo for Opc Package").arg(name));

    ui->partListWidget->addItem("/");
    foreach (Opc::PackagePart *part, m_package->parts())
        ui->partListWidget->addItem(part->partName());
    return true;
}

void MainWindow::onActionCloseTriggered()
{
    if (!m_package)
        return;
    closePackage();
}

void MainWindow::closePackage()
{
    ui->partListWidget->clear();
    ui->partContentButton->setDisabled(true);
    ui->partContentSmartButton->hide();
    ui->partRelationshipListWidget->clear();
    ui->partNameEdit->clear();
    ui->partContentTypeEdit->clear();

    delete m_package;
    m_package = 0;
}

void MainWindow::onActionAboutTriggered()
{
    QMessageBox::information(this, tr("About"), tr("This example is designed for ..."));
}

void MainWindow::onPartChanged(const QString &partName)
{
    ui->partContentButton->setDisabled(true);
    ui->partContentSmartButton->hide();
    ui->partRelationshipListWidget->clear();
    ui->partNameEdit->clear();
    ui->partContentTypeEdit->clear();

    if (partName.isEmpty())
        return;

    if (partName == "/") {
        foreach (Opc::PackageRelationship *relation, m_package->relationships())
            ui->partRelationshipListWidget->addItem(QString("%1: %2").arg(relation->id(), relation->target()));
        return;
    }

    Opc::PackagePart *part = m_package->part(partName);
    if (!part) {
        statusBar()->showMessage(tr("Error open the part %1").arg(partName), 2000);
        return;
    }

    ui->partContentButton->setEnabled(true);
    ui->partNameEdit->setText(part->partName());
    ui->partContentTypeEdit->setText(part->contentType());

    foreach (Opc::PackageRelationship *relation, part->relationships())
        ui->partRelationshipListWidget->addItem(QString("%1: %2").arg(relation->id(), relation->target()));

    //Figure out whether we can provide smart show for this part.
    if (part->contentType().startsWith("image/")) {
        ui->partContentSmartButton->setText("Show Image");
        ui->partContentSmartButton->setProperty("smart", "image");
        ui->partContentSmartButton->show();
    } else if (part->contentType().endsWith("+xml")
               || part->contentType() == "text/xml"
               || part->contentType() == "application/xml"
               || part->contentType() == "text/xml-external-parsed-entity"
               || part->contentType() == "application/xml-external-parsed-entity"
               || part->contentType() == "application/xml-dtd" ) {
        ui->partContentSmartButton->setText("Show Formatted Xml");
        ui->partContentSmartButton->setProperty("smart", "xml");
        ui->partContentSmartButton->show();
    }
}

void MainWindow::onShowContentButtonClicked()
{
    Opc::PackagePart *part = m_package->part(ui->partNameEdit->text());
    if (!part)
        return;

    //Extract the data from part stream.
    QByteArray contentData = part->getDevice()->readAll();
    part->releaseDevice();

    //Show data in bin edit.
    BinEdit *bEdit = new BinEdit;
    bEdit->setData(contentData);
    bEdit->setWindowTitle(part->partName());

    bEdit->setReadOnly(true);
    bEdit->setAttribute(Qt::WA_DeleteOnClose);
    QFont font = bEdit->font();
    font.setPointSize(14);
    bEdit->setFont(font);
    bEdit->resize(800, 600);

    bEdit->show();
}

void MainWindow::onShowContentSmartButtonClicked()
{
    Opc::PackagePart *part = m_package->part(ui->partNameEdit->text());
    if (!part)
        return;

    if (ui->partContentSmartButton->property("smart") == "xml") {
        //Show formatted xml file contents
        MceXmlWidget *edit = new MceXmlWidget(part->getDevice()->readAll());
        part->releaseDevice();

        edit->setAttribute(Qt::WA_DeleteOnClose);
        edit->setWindowTitle(part->partName());
        edit->resize(800, 600);
        edit->show();
    } else if (ui->partContentSmartButton->property("smart") == "image") {
        //Show image
        ImageWidget *edit = new ImageWidget;
        edit->setAttribute(Qt::WA_DeleteOnClose);
        edit->setWindowTitle(part->partName());
        QImage image = QImage::fromData(part->getDevice()->readAll());
        part->releaseDevice();

        edit->setPixmap(QPixmap::fromImage(image));
        edit->resize(800, 600);
        edit->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    saveSettings();
    evt->accept();
}

void MainWindow::createActions()
{
    m_actionOpen = new QAction(tr("&Open..."), this);
    m_actionClose = new QAction(tr("Close"), this);
    m_actionQuit = new QAction(tr("&Quit"), this);

    m_menuRecentFiles = new QMenu(tr("Recent files"), this);

    m_actionAbout = new QAction(tr("&About..."), this);

    connect(m_actionOpen, SIGNAL(triggered()), SLOT(onActionOpenTriggered()));
    connect(m_actionClose, SIGNAL(triggered()), SLOT(onActionCloseTriggered()));
    connect(m_actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_actionAbout, SIGNAL(triggered()), SLOT(onActionAboutTriggered()));
}

void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_actionOpen);
    fileMenu->addAction(m_menuRecentFiles->menuAction());
    fileMenu->addAction(m_actionClose);
    fileMenu->addSeparator();
    fileMenu->addAction(m_actionQuit);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_actionAbout);
}

void MainWindow::updateRecentFilesMenu()
{
    m_menuRecentFiles->clear();
    for (int i=0; i<m_recentFilesList.size(); ++i) {
        QString title = QString("%1 %2").arg(i)
                .arg(QFontMetrics(font()).elidedText(m_recentFilesList.at(i), Qt::ElideMiddle, 300));
        QAction *act = new QAction(title, m_menuRecentFiles);
        act->setData(m_recentFilesList.at(i));
        m_menuRecentFiles->addAction(act);

        connect(act, SIGNAL(triggered()), SLOT(onActionRecentFileTriggered()));
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("opcpackagedemo.ini", QSettings::IniFormat);
    m_recentFilesList = settings.value("RecentFiles").toStringList();

    updateRecentFilesMenu();
}

void MainWindow::saveSettings()
{
    QSettings settings("opcpackagedemo.ini", QSettings::IniFormat);
    settings.setValue("RecentFiles", m_recentFilesList);
}
