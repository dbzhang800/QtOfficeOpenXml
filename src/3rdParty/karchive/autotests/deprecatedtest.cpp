/* This file is part of the KDE project
   Copyright (C) 2006, 2010 David Faure <faure@kde.org>
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

#include <kzip.h>

#include <QtTest/QtTest>

static const char s_zipFileName[] = "deprecatedtest.zip";

class DeprecatedTest : public QObject
{
    Q_OBJECT

#ifndef KARCHIVE_NO_DEPRECATED
private Q_SLOTS:
    void testKArchiveWriteFile()
    {
        KZip zip(s_zipFileName);

        QVERIFY(zip.open(QIODevice::WriteOnly));

        const QByteArray fileData("There could be a fire, if there is smoke.");
        const QString fileName = QLatin1String("wisdom");
        QVERIFY(zip.writeFile(fileName, "konqi", "dragons", fileData.constData(), fileData.size()));

        QVERIFY(zip.close());

        QVERIFY(zip.open(QIODevice::ReadOnly));

        const KArchiveDirectory *dir = zip.directory();
        QVERIFY(dir != 0);
        const QStringList listing = dir->entries();
        QCOMPARE(listing.count(), 1);
        QCOMPARE(listing.at(0), fileName);
        const KArchiveEntry *entry = dir->entry(listing.at(0));
        QCOMPARE(entry->permissions(), mode_t(0100644));
        QVERIFY(!entry->isDirectory());
        const KArchiveFile *fileEntry = static_cast<const KArchiveFile *>(entry);
        QCOMPARE(fileEntry->size(), fileData.size());
        QCOMPARE(fileEntry->data(), fileData);
    }

    /**
     * @see QTest::cleanupTestCase()
     */
    void cleanupTestCase()
    {
        QFile::remove(s_zipFileName);
    }
#endif
};

QTEST_MAIN(DeprecatedTest)

#include <deprecatedtest.moc>
