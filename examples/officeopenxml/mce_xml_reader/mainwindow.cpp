/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang <hello@debao.me>
**
** This library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Library General Public License version
** 2.1 or version 3 as published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public License
** along with this library; see the file LICENSE.LGPLv21 and LICENSE.LGPLv3
** included in the packaging of this file. If not, write to the Free
** Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
** 02110-1301, USA.
**
****************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <mcexmlstreamreader.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QXmlStreamReader>

using namespace QtOfficeOpenXml;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->action_Open, &QAction::triggered, this, &MainWindow::onOpen);
    connect(ui->action_About, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->mceLoadButton, &QPushButton::clicked, this, &MainWindow::onLoadButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpen()
{
    QSettings settings("mcereaderdemo.ini", QSettings::IniFormat);
    const QString path = settings.value("lastFile").toString();

    QString fn = QFileDialog::getOpenFileName( 0, tr("Open MCE Xml file"), path, "xml files (*.xml)");
    if (fn.isEmpty())
        return;
    settings.setValue("lastFile", fn);

    ui->orignalTextEdit->clear();
    ui->normalXmlOutEdit->clear();
    ui->mceOutTextEdit->clear();
    ui->nsListWidget->clear();

    doLoadOrignalFile(fn);
}

void MainWindow::onAbout()
{
    QMessageBox::information(this, tr("About"), tr("This example is designed to show how to used Mce::XmlStreamReader class."));
}

void MainWindow::onLoadButton()
{
    ui->mceOutTextEdit->clear();

    QSet<QString> understoodNamespaces;
    for (int i=0; i<ui->nsListWidget->count(); ++i) {
        QListWidgetItem *item = ui->nsListWidget->item(i);
        if (item->checkState() == Qt::Checked)
            understoodNamespaces.insert(item->text().split(' ')[1]);
    }

    Mce::XmlStreamReader reader(fileContent);
    reader.setMceUnderstoodNamespaces(understoodNamespaces);

    int depth = 0;
    while (reader.readNext() != QXmlStreamReader::Invalid) {
        if (reader.isEndElement()) {
            --depth;
        } else if (reader.isStartElement()) {
            QStringList attributeNames;
            foreach (QXmlStreamAttribute attrib, reader.attributes())
                attributeNames.append(attrib.qualifiedName().toString());

            ui->mceOutTextEdit->appendPlainText(QString("%1%2\t[%3]")
                                                  .arg(QString(depth*2, ' '))
                                                  .arg(reader.qualifiedName().toString())
                                                  .arg(attributeNames.join(' ')));
            ++depth;
        }
    }
    if (reader.hasError() && reader.error()!=QXmlStreamReader::PrematureEndOfDocumentError)
        ui->mceOutTextEdit->appendHtml(QString("<font color=red>%1</font>").arg(reader.errorString()));
}

void MainWindow::doLoadOrignalFile(const QString &filePath)
{
    QFile f(filePath);
    f.open(QFile::ReadOnly);
    fileContent = f.readAll();

    //Assume that the file encoded in UTF8
    //Shown file contents in orignal text edit.
    ui->orignalTextEdit->setPlainText(QString::fromUtf8(fileContent));

    //Shown all the start element names and
    //Find out all the namespaces used in this xml file.
    QXmlStreamReader reader(fileContent);
    QStringList nsList;
    int depth = 0;
    while (reader.readNext() != QXmlStreamReader::Invalid) {
        if (reader.isEndElement()) {
            --depth;
        } else if (reader.isStartElement()) {
            QStringList attributeNames;
            foreach (QXmlStreamAttribute attrib, reader.attributes())
                attributeNames.append(attrib.qualifiedName().toString());
            foreach (QXmlStreamNamespaceDeclaration nsDecl, reader.namespaceDeclarations()) {
                const QString ns = QString("[%1] %2")
                        .arg(nsDecl.prefix().toString())
                        .arg(nsDecl.namespaceUri().toString());
                if (!nsList.contains(ns))
                    nsList.append(ns);
            }

            ui->normalXmlOutEdit->appendPlainText(QString("%1%2\t[%3]")
                                                  .arg(QString(depth*2, ' '))
                                                  .arg(reader.qualifiedName().toString())
                                                  .arg(attributeNames.join(' ')));
            ++depth;
        }
    }
    if (reader.hasError() && reader.error()!=QXmlStreamReader::PrematureEndOfDocumentError)
        ui->mceOutTextEdit->appendHtml(QString("<font color=red>%1</font>").arg(reader.errorString()));


    //Show all the namespaces
    foreach (QString ns, nsList) {
        QListWidgetItem *item = new QListWidgetItem(ns, ui->nsListWidget);
        item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }
}
