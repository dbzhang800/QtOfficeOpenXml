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
#include "recentfiles.h"
#include <mcexmlstreamreader.h>

#include <QFileInfo>
#include <QDir>
#include <QTemporaryFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QCloseEvent>
#include <QFontMetrics>
#include <QScopedPointer>

static bool isXmlContentType(const QString &type)
{
    return type.endsWith(QLatin1String("+xml"))
           || type == QLatin1String("text/xml")
           || type == QLatin1String("application/xml")
           || type == QLatin1String("text/xml-external-parsed-entity")
           || type == QLatin1String("application/xml-external-parsed-entity")
           || type == QLatin1String("application/xml-dtd");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_package(0)
{
    ui->setupUi(this);
    m_recentFiles = new RecentFiles(this);

    createActions();
    createMenuBar();

    connect(m_recentFiles, SIGNAL(activated(QString)), SLOT(onRecentFileTriggered(QString)));
    connect(ui->partListWidget, SIGNAL(currentTextChanged(QString)), SLOT(onPartItemChanged(QString)));
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
    if (isWindowModified()) {
        int ret = getUserCloseOption();
        if (ret == QMessageBox::Yes)
            doSavePackage(m_package->packageName(), true, false);
        else if (ret == QMessageBox::Cancel)
            return;
    }

    QString fn = QFileDialog::getOpenFileName( 0, tr("Open OPC Package"), m_recentFiles->mostRecentFile(), "Office files (*.xlsx *.docx *.pptx);;All files(*.*)");
    if (fn.isEmpty())
        return;

    openPackage(fn);
}

void MainWindow::onRecentFileTriggered(const QString &fileName)
{
    if (m_package && m_package->packageName() == fileName)
        return;

    if (isWindowModified()) {
        int ret = getUserCloseOption();
        if (ret == QMessageBox::Yes)
            doSavePackage(m_package->packageName(), true, false);
        else if (ret == QMessageBox::Cancel)
            return;
    }

    if (!openPackage(fileName))
        m_recentFiles->remove(fileName);
}

bool MainWindow::openPackage(const QString &name)
{
    if (m_package)
        closePackage();

    m_package = Opc::Package::open(name, QIODevice::ReadOnly);
    if (!m_package) {
        //Error occur.
        statusBar()->showMessage(tr("Fail to open the package %1").arg(name));
        setWindowTitle("Edit for xml part of opc package");
        return false;
    }

    m_recentFiles->add(name);

    setWindowTitle(QString("%1[*] - Edit for xml part of opc package").arg(name));

    foreach (Opc::PackagePart *part, m_package->parts()) {
        QListWidgetItem *item = new QListWidgetItem(part->partName(), ui->partListWidget);
        if (!isXmlContentType(part->contentType()))
            item->setFlags(item->flags()&~Qt::ItemIsEnabled);
    }
    return true;
}

int MainWindow::getUserCloseOption()
{
    if (isWindowModified()) {
        return QMessageBox::question(this, QString("Save changes"),
                             QString("Package %1 has been changed, do you want to save it?").arg(m_package->packageName()),
                             QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    }
    return QMessageBox::No;
}

void MainWindow::onActionCloseTriggered()
{
    if (!m_package)
        return;

    if (isWindowModified()) {
        int ret = getUserCloseOption();
        if (ret == QMessageBox::Yes)
            doSavePackage(m_package->packageName(), true, false);
        else if (ret == QMessageBox::Cancel)
            return;
    }

    closePackage();
}

void MainWindow::onActionSaveTriggered()
{
    if (isWindowModified())
        doSavePackage(m_package->packageName(), true, true);
}

void MainWindow::onActionSaveAsTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save package", m_recentFiles->mostRecentFile(), "Office files (*.xlsx *.docx *.pptx);;All files(*.*)");
    if (fileName.isEmpty())
        return;
    doSavePackage(fileName, true, true);
}

void MainWindow::closePackage()
{
    if (!m_package)
        return;

    ui->partListWidget->clear();
    ui->partNameEdit->clear();
    ui->partContentTypeEdit->clear();
    ui->partContentEdit->setDocument(0);
    ui->partContentEdit->setDisabled(true);

    qDeleteAll(m_partDocumentHash);
    m_partDocumentHash.clear();

    delete m_package;
    m_package = 0;
}

void MainWindow::doSavePackage(const QString &fileName, bool closeOld, bool openNew)
{
    QTemporaryFile tempPackageFile(QString("XXXXXX.%1").arg(QFileInfo(m_package->packageName()).suffix()));
    tempPackageFile.open();

    QScopedPointer<Opc::Package> tempPackage(Opc::Package::open(&tempPackageFile, QIODevice::WriteOnly));

    foreach (Opc::PackagePart *part, m_package->parts()) {
        Opc::PackagePart *newPart = tempPackage->createPart(part->partName(), part->contentType());
        QIODevice *newDev = newPart->getDevice();

        if (m_partDocumentHash.contains(part->partName()) && m_partDocumentHash[part->partName()]->isModified()) {
            newDev->write(m_partDocumentHash[part->partName()]->toPlainText().toUtf8());
        } else {
            newDev->write(part->getDevice()->readAll());
            part->releaseDevice();
        }
        newPart->releaseDevice();
    }
    tempPackage->close();

    //Close current package, copy the temp package to target place,
    //then open the target package.
    if (closeOld)
        closePackage();
    QFile::remove(fileName);
    if (!QFile::copy(tempPackageFile.fileName(), fileName)) {
        statusBar()->showMessage(QString("Error: can not write %1").arg(fileName), 3000);
    }
    if (openNew)
        openPackage(fileName);
}

void MainWindow::onActionAboutTriggered()
{
    QMessageBox::information(this, tr("About"), tr("This example is designed for ..."));
}

void MainWindow::onPartItemChanged(const QString &partName)
{
    ui->partNameEdit->clear();
    ui->partContentTypeEdit->clear();
    ui->partContentEdit->setDisabled(true);

    if (partName.isEmpty())
        return;

    Opc::PackagePart *part = m_package->part(partName);
    if (!part) {
        statusBar()->showMessage(tr("Error open the part %1").arg(partName), 2000);
        return;
    }

    ui->partNameEdit->setText(part->partName());
    ui->partContentTypeEdit->setText(part->contentType());

    QTextDocument *document;

    //Figure out whether this part has been extract to temp file or not.
    if (m_partDocumentHash.contains(part->partName())) {
        document = m_partDocumentHash[part->partName()];
    } else {
        QByteArray formattedData;
        //Extract content of the part.
        //Re-format the xml file content first, then write to data cache.

        //QXmlStreamWriter used here to generate the formatted xml file data.
        QXmlStreamWriter writer(&formattedData);
        writer.setAutoFormatting(true);

        //Load the file content by QXmlStreamReader
        QXmlStreamReader reader(part->getDevice());
        while(!reader.atEnd()) {
            reader.readNext();
            //Prefer Mce::writeCurrentToken() to QXmlStreamWriter::writeCurrentToken()
            if (!reader.isWhitespace() && reader.tokenType() != QXmlStreamReader::Invalid)
                Mce::writeCurrentToken(writer, reader);
        }
        part->releaseDevice();

        document = new QTextDocument(this);
        document->setDocumentLayout(new QPlainTextDocumentLayout(document));
        document->setProperty("partName", part->partName());
        document->setPlainText(QString::fromUtf8(formattedData));
        document->setModified(false);
        m_partDocumentHash[part->partName()] = document;

        connect(document, SIGNAL(modificationChanged(bool)), SLOT(onPartModificationChanged(bool)));
        //connect(document, SIGNAL(undoAvailable(bool)), SLOT(onPartModificationChanged(bool)));
    }

    //Show part content
    ui->partContentEdit->setDocument(document);
    ui->partContentEdit->setEnabled(true);
}

void MainWindow::onPartModificationChanged(bool changed)
{
    QTextDocument *document = qobject_cast<QTextDocument *>(sender());
    if (!document)
        return;
    QString partName = document->property("partName").toString();

    for (int i=0; i<ui->partListWidget->count(); ++i) {
        QListWidgetItem *item = ui->partListWidget->item(i);
        if (item->text() == partName) {
            QFont font = item->font();
            font.setBold(changed);
            item->setFont(font);
            break;
        }
    }

    bool modified = false;
    foreach (QTextDocument *doc, m_partDocumentHash) {
        if (doc->isModified()) {
            modified = true;
        }
    }
    setWindowModified(modified);
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    if (isWindowModified()) {
        int ret = QMessageBox::question(this, QString("Save changes"),
                             QString("Package %1 has been changed, do you want to save it?").arg(m_package->packageName()),
                             QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (ret == QMessageBox::Yes) {
            doSavePackage(m_package->packageName(), true, false);
        } else if (ret == QMessageBox::Cancel) {
            evt->ignore();
            return;
        }
    }

    saveSettings();
    evt->accept();
}

void MainWindow::createActions()
{
    m_actionOpen = new QAction(tr("&Open..."), this);
    m_actionOpen->setShortcut(QKeySequence::Open);
    m_actionClose = new QAction(tr("Close"), this);
    m_actionSave = new QAction(tr("&Save"), this);
    m_actionSave->setShortcut(QKeySequence::Save);
    m_actionSaveAs = new QAction(tr("Save as..."), this);
    m_actionSaveAs->setShortcut(QKeySequence::SaveAs);
    m_actionQuit = new QAction(tr("&Quit"), this);

    m_actionAbout = new QAction(tr("&About..."), this);

    connect(m_actionOpen, SIGNAL(triggered()), SLOT(onActionOpenTriggered()));
    connect(m_actionClose, SIGNAL(triggered()), SLOT(onActionCloseTriggered()));
    connect(m_actionSave, SIGNAL(triggered()), SLOT(onActionSaveTriggered()));
    connect(m_actionSaveAs, SIGNAL(triggered()), SLOT(onActionSaveAsTriggered()));
    connect(m_actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_actionAbout, SIGNAL(triggered()), SLOT(onActionAboutTriggered()));
}

void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_actionOpen);
    fileMenu->addAction(m_recentFiles->menu()->menuAction());
    fileMenu->addAction(m_actionClose);
    fileMenu->addAction(m_actionSave);
    fileMenu->addAction(m_actionSaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(m_actionQuit);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_actionAbout);
}

void MainWindow::loadSettings()
{
    QSettings settings;
    m_recentFiles->setFileList(settings.value("RecentFiles").toStringList());
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("RecentFiles", m_recentFiles->fileList());
}
