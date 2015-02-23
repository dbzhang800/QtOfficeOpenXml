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
#include <QtOfficeOpenXml>

using namespace QtOfficeOpenXml;

void createXlsxDocument(const QString &name);

int main()
{
    //We create a xlsx file first.
    QString documentName("abcd.xlsx");
    createXlsxDocument(documentName);

    //Read the file.
    Sml::Document xlsx(documentName);
    qDebug()<<xlsx.cellAt("A1")->rawValue();
    qDebug()<<xlsx.cellAt("Sheet1!C5")->rawValue();

    return 0;
}

void createXlsxDocument(const QString &documentName)
{
    Sml::Document xlsx;
    xlsx.addSheet("Sheet1");
    xlsx.currentWorksheet()->writeNumber("A1", 1);
    xlsx.currentWorksheet()->writeNumber("C5", 10000);

    xlsx.saveAs(documentName);
}
