/*
 *  Copyright (C) 2002-2005 David Faure   <faure@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation;
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */
#ifndef KFILTERTEST_H
#define KFILTERTEST_H

#include <QtCore/QObject>

class KFilterTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test_block_write();
    void test_block_read();
    void test_biggerWrites();
    void test_getch();
    void test_textstream();
    void test_readall();
    void test_uncompressed();
    void test_findFilterByMimeType_data();
    void test_findFilterByMimeType();
    void test_deflateWithZlibHeader();
    void test_pushData();
    void test_saveFile_data();
    void test_saveFile();

private:
    void test_block_write(const QString &fileName, const QByteArray &data);
    void test_block_read(const QString &fileName);
    void test_getch(const QString &fileName);
    void test_textstream(const QString &fileName);
    void test_readall(const QString &fileName, const QString &mimeType, const QByteArray &expectedData);

private:
    QString pathgz;
    QString pathbz2;
    QString pathxz;
    QString pathnone;
    QByteArray testData;
};

#endif
