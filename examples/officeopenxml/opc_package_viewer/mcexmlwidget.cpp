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
#include "mcexmlwidget.h"
#include "ui_mcexmlwidget.h"
#include <mcexmlstreamreader.h>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

using namespace QtOfficeOpenXml;

MceXmlWidget::MceXmlWidget(const QByteArray &xmlData, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MceXmlWidget), fileContent(xmlData)
{
    ui->setupUi(this);
    connect(ui->mceUpdateButton, &QPushButton::clicked, this, &MceXmlWidget::onUpdateButton);

    doLoadOrignalXmlData();
}

MceXmlWidget::~MceXmlWidget()
{
    delete ui;
}

void MceXmlWidget::onUpdateButton()
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

    //QXmlStreamWriter used here to format the xml file data.
    QByteArray formattedData;
    QXmlStreamWriter writer(&formattedData);
    writer.setAutoFormatting(true);

    while(!reader.atEnd()) {
        reader.readNext();
        if (!reader.isWhitespace() && reader.tokenType() != QXmlStreamReader::Invalid)
            Mce::writeCurrentToken(writer, reader);
    }

    ui->mceOutTextEdit->setPlainText(formattedData);
    if (reader.hasError() && reader.error()!=QXmlStreamReader::PrematureEndOfDocumentError)
        ui->mceOutTextEdit->appendHtml(QString("<font color=red>%1</font>").arg(reader.errorString()));
}

void MceXmlWidget::doLoadOrignalXmlData()
{
    //We need to find out all the namespaces used in this xml file.
    QStringList nsList;

    //QXmlStreamWriter used here to generate the formatted xml file data.
    QByteArray formattedData;
    QXmlStreamWriter writer(&formattedData);
    writer.setAutoFormatting(true);

    //Load the file content by QXmlStreamReader
    QXmlStreamReader reader(fileContent);
    while(!reader.atEnd()) {
        reader.readNext();
        //Prefer Mce::writeCurrentToken() to QXmlStreamWriter::writeCurrentToken()
        if (!reader.isWhitespace() && reader.tokenType() != QXmlStreamReader::Invalid)
            Mce::writeCurrentToken(writer, reader);
        if (reader.isStartElement()) {
            foreach (QXmlStreamNamespaceDeclaration nsDecl, reader.namespaceDeclarations()) {
                const QString ns = QString("[%1] %2")
                        .arg(nsDecl.prefix().toString())
                        .arg(nsDecl.namespaceUri().toString());
                if (!nsList.contains(ns))
                    nsList.append(ns);
            }
        }
    }

    ui->normalXmlOutEdit->setPlainText(formattedData);
    if (reader.hasError() && reader.error()!=QXmlStreamReader::PrematureEndOfDocumentError)
        ui->normalXmlOutEdit->appendHtml(QString("<font color=red>%1</font>").arg(reader.errorString()));

    //Show all the namespaces
    foreach (QString ns, nsList) {
        QListWidgetItem *item = new QListWidgetItem(ns, ui->nsListWidget);
        item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }

    //...
    onUpdateButton();
}
