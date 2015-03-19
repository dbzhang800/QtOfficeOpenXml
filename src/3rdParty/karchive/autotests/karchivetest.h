/* This file is part of the KDE project
   Copyright (C) 2006 David Faure <faure@kde.org>
   Copyright (C) 2012 Mario Bensi <mbensi@ipsquad.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KARCHIVETEST_H
#define KARCHIVETEST_H

#include <QtCore/QObject>
#include <config-compression.h>

class KArchiveTest : public QObject
{
    Q_OBJECT

    void setupData();
    void setup7ZipData();

private Q_SLOTS:
    void initTestCase();

    void testEmptyFilename();
    void testNullDevice();
    void testNonExistentFile();
    void testCreateTar_data();
    void testCreateTar();
    void testCreateTarXXX_data()
    {
        setupData();
    }
    void testCreateTarXXX();
    void testReadTar_data()
    {
        setupData();
    }
    void testReadTar();
    void testUncompress_data()
    {
        setupData();
    }
    void testUncompress();
    void testTarFileData_data()
    {
        setupData();
    }
    void testTarFileData();
    void testTarCopyTo_data()
    {
        setupData();
    }
    void testTarCopyTo();
    void testTarReadWrite_data()
    {
        setupData();
    }
    void testTarReadWrite();
    void testTarMaxLength_data();
    void testTarMaxLength();
    void testTarGlobalHeader();
    void testTarPrefix();
    void testTarDirectoryForgotten();
    void testTarRootDir();
    void testTarDirectoryTwice();

    void testCreateZip();
    void testCreateZipError();
    void testReadZipError();
    void testReadZip();
    void testZipFileData();
    void testZipCopyTo();
    void testZipMaxLength();
    void testZipWithNonLatinFileNames();
    void testZipWithOverwrittenFileName();
    void testZipAddLocalDirectory();
    void testZipReadRedundantDataDescriptor_data();
    void testZipReadRedundantDataDescriptor();

    void testRcc();

#if HAVE_XZ_SUPPORT
    void testCreate7Zip_data()
    {
        setup7ZipData();
    }
    void testCreate7Zip();
    void testRead7Zip_data()
    {
        setup7ZipData();
    }
    void testRead7Zip();
    void test7ZipFileData_data()
    {
        setup7ZipData();
    }
    void test7ZipFileData();
    void test7ZipCopyTo_data()
    {
        setup7ZipData();
    }
    void test7ZipCopyTo();
    void test7ZipReadWrite_data()
    {
        setup7ZipData();
    }
    void test7ZipReadWrite();
    void test7ZipMaxLength_data()
    {
        setup7ZipData();
    }
    void test7ZipMaxLength();
#endif

    void cleanupTestCase();
};

#endif
