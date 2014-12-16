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

#include "karchivetest.h"
#include <ktar.h>
#include <kzip.h>
#include <k7zip.h>
#include <krcc.h>

#include <QtTest/QtTest>
#include <QtCore/QFileInfo>
#include <kfilterdev.h>
#include <qtemporarydir.h>

#ifndef Q_OS_WIN
#include <unistd.h> // symlink
#include <errno.h>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#else
#include <grp.h>
#include <pwd.h>
#endif

QTEST_MAIN(KArchiveTest)

static const int SIZE1 = 100;

/**
 * Writes test fileset specified archive
 * @param archive archive
 */
static void writeTestFilesToArchive(KArchive *archive)
{
    QVERIFY(archive->writeFile("empty", "", 0100644, "weis", "users"));
    QVERIFY(archive->writeFile("test1", QByteArray("Hallo"), 0100440, QString("weis"), QString("users")));
    // Now let's try with the prepareWriting/writeData/finishWriting API
    QVERIFY(archive->prepareWriting("test2", "weis", "users", 8));
    QVERIFY(archive->writeData("Hallo ", 6));
    QVERIFY(archive->writeData("Du", 2));
    QVERIFY(archive->finishWriting(8));
    // Add local file
    QFile localFile("test3");
    QVERIFY(localFile.open(QIODevice::WriteOnly));
    QVERIFY(localFile.write("Noch so einer", 13) == 13);
    localFile.close();

    QVERIFY(archive->addLocalFile("test3", "z/test3"));

    // writeFile API
    QVERIFY(archive->writeFile("my/dir/test3", "I do not speak German\nDavid.", 0100644, "dfaure", "hackers"));

    // Now a medium file : 100 null bytes
    char medium[ SIZE1 ];
    memset(medium, 0, SIZE1);
    QVERIFY(archive->writeFile("mediumfile", QByteArray(medium, SIZE1)));
    // Another one, with an absolute path
    QVERIFY(archive->writeFile("/dir/subdir/mediumfile2", QByteArray(medium, SIZE1)));

    // Now a huge file : 20000 null bytes
    int n = 20000;
    char *huge = new char[ n ];
    memset(huge, 0, n);
    QVERIFY(archive->writeFile("hugefile", QByteArray(huge, n)));
    delete [] huge;

    // Now an empty directory
    QVERIFY(archive->writeDir("aaaemptydir"));

#ifndef Q_OS_WIN
    // Add local symlink
    QVERIFY(archive->addLocalFile("test3_symlink", "z/test3_symlink"));
#endif
}

static QString getCurrentUserName()
{
#if defined(Q_OS_UNIX)
    struct passwd *pw = getpwuid(getuid());
    return pw ? QFile::decodeName(pw->pw_name) : QString::number(getuid());
#elif defined(Q_OS_WIN)
    wchar_t buffer[255];
    DWORD size = 255;
    bool ok = GetUserNameW(buffer, &size);
    if (!ok) {
        return QString();
    }
    return QString::fromWCharArray(buffer);
#else
    return QString();
#endif
}

static QString getCurrentGroupName()
{
#if defined(Q_OS_UNIX)
    struct group *grp = getgrgid(getgid());
    return grp ? QFile::decodeName(grp->gr_name) : QString::number(getgid());
#elif defined(Q_OS_WIN)
    return QString();
#else
    return QString();
#endif
}

enum { WithUserGroup = 1, WithTime = 0x02 }; // ListingFlags

static QStringList recursiveListEntries(const KArchiveDirectory *dir, const QString &path, int listingFlags)
{
    QStringList ret;
    QStringList l = dir->entries();
    l.sort();
    Q_FOREACH (const QString &it, l) {
        const KArchiveEntry *entry = dir->entry(it);

        QString descr;
        descr += QString("mode=") + QString::number(entry->permissions(), 8) + ' ';
        if (listingFlags & WithUserGroup) {
            descr += QString("user=") + entry->user() + ' ';
            descr += QString("group=") + entry->group() + ' ';
        }
        descr += QString("path=") + path + (it) + ' ';
        descr += QString("type=") + (entry->isDirectory() ? "dir" : "file");
        if (entry->isFile()) {
            descr += QString(" size=") + QString::number(static_cast<const KArchiveFile *>(entry)->size());
        }
        if (!entry->symLinkTarget().isEmpty()) {
            descr += QString(" symlink=") + entry->symLinkTarget();
        }

        if (listingFlags & WithTime) {
            descr += QString(" time=") + entry->date().toString("dd.MM.yyyy hh:mm:ss");
        }

        //qDebug() << descr;
        ret.append(descr);

        if (entry->isDirectory()) {
            ret += recursiveListEntries((KArchiveDirectory *)entry, path + it + '/', listingFlags);
        }
    }
    return ret;
}

/**
 * Verifies contents of specified archive against test fileset
 * @param archive archive
 */
static void testFileData(KArchive *archive)
{
    const KArchiveDirectory *dir = archive->directory();

    const KArchiveFile *f = dir->file("z/test3");
    QByteArray arr(f->data());
    QCOMPARE(arr.size(), 13);
    QCOMPARE(arr, QByteArray("Noch so einer"));

    // Now test using createDevice()
    QIODevice *dev = f->createDevice();
    QByteArray contents = dev->readAll();
    QCOMPARE(contents, arr);
    delete dev;

    dev = f->createDevice();
    contents = dev->read(5); // test reading in two chunks
    QCOMPARE(contents.size(), 5);
    contents += dev->read(50);
    QCOMPARE(contents.size(), 13);
    QCOMPARE(QString::fromLatin1(contents.constData()), QString::fromLatin1(arr.constData()));
    delete dev;

    const KArchiveEntry *e = dir->entry("mediumfile");
    QVERIFY(e && e->isFile());
    f = (KArchiveFile *)e;
    QCOMPARE(f->data().size(), SIZE1);

    f = dir->file("hugefile");
    QCOMPARE(f->data().size(), 20000);

    e = dir->entry("aaaemptydir");
    QVERIFY(e && e->isDirectory());
    QVERIFY(!dir->file("aaaemptydir"));

    e = dir->entry("my/dir/test3");
    QVERIFY(e && e->isFile());
    f = (KArchiveFile *)e;
    dev = f->createDevice();
    QByteArray firstLine = dev->readLine();
    QCOMPARE(QString::fromLatin1(firstLine.constData()), QString::fromLatin1("I do not speak German\n"));
    QByteArray secondLine = dev->read(100);
    QCOMPARE(QString::fromLatin1(secondLine.constData()), QString::fromLatin1("David."));
    delete dev;
#ifndef Q_OS_WIN
    e = dir->entry("z/test3_symlink");
    QVERIFY(e);
    QVERIFY(e->isFile());
    QCOMPARE(e->symLinkTarget(), QString("test3"));
#endif

    // Test "./" prefix for KOffice (xlink:href="./ObjectReplacements/Object 1")
    e = dir->entry("./hugefile");
    QVERIFY(e && e->isFile());
    e = dir->entry("./my/dir/test3");
    QVERIFY(e && e->isFile());

    // Test directory entries
    e = dir->entry("my");
    QVERIFY(e && e->isDirectory());
    e = dir->entry("my/");
    QVERIFY(e && e->isDirectory());
    e = dir->entry("./my/");
    QVERIFY(e && e->isDirectory());
}

static void testReadWrite(KArchive *archive)
{
    QVERIFY(archive->writeFile("newfile", "New File", 0100440, "dfaure", "users"));
}

static void testCopyTo(KArchive *archive)
{
    const KArchiveDirectory *dir = archive->directory();
    QTemporaryDir tmpDir;
    const QString dirName = tmpDir.path() + '/';

    QVERIFY(dir->copyTo(dirName));

    QVERIFY(QFile::exists(dirName + "dir"));
    QVERIFY(QFileInfo(dirName + "dir").isDir());

    QFileInfo fileInfo1(dirName + "dir/subdir/mediumfile2");
    QVERIFY(fileInfo1.exists());
    QVERIFY(fileInfo1.isFile());
    QCOMPARE(fileInfo1.size(), Q_INT64_C(100));

    QFileInfo fileInfo2(dirName + "hugefile");
    QVERIFY(fileInfo2.exists());
    QVERIFY(fileInfo2.isFile());
    QCOMPARE(fileInfo2.size(), Q_INT64_C(20000));

    QFileInfo fileInfo3(dirName + "mediumfile");
    QVERIFY(fileInfo3.exists());
    QVERIFY(fileInfo3.isFile());
    QCOMPARE(fileInfo3.size(), Q_INT64_C(100));

    QFileInfo fileInfo4(dirName + "my/dir/test3");
    QVERIFY(fileInfo4.exists());
    QVERIFY(fileInfo4.isFile());
    QCOMPARE(fileInfo4.size(), Q_INT64_C(28));

#ifndef Q_OS_WIN
    const QString fileName = dirName + "z/test3_symlink";
    const QFileInfo fileInfo5(fileName);
    QVERIFY(fileInfo5.exists());
    QVERIFY(fileInfo5.isFile());
    // Do not use fileInfo.readLink() for unix symlinks
    // It returns the -full- path to the target, while we want the target string "as is".
    QString symLinkTarget;
    const QByteArray encodedFileName = QFile::encodeName(fileName);
    QByteArray s;
#if defined(PATH_MAX)
    s.resize(PATH_MAX + 1);
#else
    int path_max = pathconf(encodedFileName.data(), _PC_PATH_MAX);
    if (path_max <= 0) {
        path_max = 4096;
    }
    s.resize(path_max);
#endif
    int len = readlink(encodedFileName.data(), s.data(), s.size() - 1);
    if (len >= 0) {
        s[len] = '\0';
        symLinkTarget = QFile::decodeName(s);
    }
    QCOMPARE(symLinkTarget, QString("test3"));
#endif
}

/**
 * Prepares dataset for archive filter tests
 */
void KArchiveTest::setupData()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("mimeType");

    QTest::newRow(".tar.gz") << "karchivetest.tar.gz" << "application/x-gzip";
#if HAVE_BZIP2_SUPPORT
    QTest::newRow(".tar.bz2") << "karchivetest.tar.bz2" << "application/x-bzip";
#endif
#if HAVE_XZ_SUPPORT
    QTest::newRow(".tar.lzma") << "karchivetest.tar.lzma" << "application/x-lzma";
    QTest::newRow(".tar.xz") << "karchivetest.tar.xz" << "application/x-xz";
#endif
}

/**
 * @see QTest::initTestCase()
 */
void KArchiveTest::initTestCase()
{
#ifndef Q_OS_WIN
    // Prepare local symlink
    QFile::remove("test3_symlink");
    if (::symlink("test3", "test3_symlink") != 0) {
        qDebug() << errno;
        QVERIFY(false);
    }
#endif
}

void KArchiveTest::testEmptyFilename()
{
    QTest::ignoreMessage(QtWarningMsg, "KArchive: No file name specified");
    KTar tar(QLatin1String(""));
    QVERIFY(!tar.open(QIODevice::ReadOnly));
}

void KArchiveTest::testNullDevice()
{
    QIODevice *nil = 0;
    QTest::ignoreMessage(QtWarningMsg, "KArchive: Null device specified");
    KTar tar(nil);
    QVERIFY(!tar.open(QIODevice::ReadOnly));
}

void KArchiveTest::testNonExistentFile()
{
    KTar tar(QLatin1String("nonexistent.tar.gz"));
    QVERIFY(!tar.open(QIODevice::ReadOnly));
}

void KArchiveTest::testCreateTar_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::newRow(".tar") << "karchivetest.tar";
}

/**
 * @dataProvider testCreateTar_data
 */
void KArchiveTest::testCreateTar()
{
    QFETCH(QString, fileName);

    // With    tempfile: 0.7-0.8 ms, 994236 instr. loads
    // Without tempfile:    0.81 ms, 992541 instr. loads
    // Note: use ./karchivetest 2>&1 | grep ms
    //       to avoid being slowed down by the kDebugs.
    QBENCHMARK {

        KTar tar(fileName);
        QVERIFY(tar.open(QIODevice::WriteOnly));

        writeTestFilesToArchive(&tar);

        QVERIFY(tar.close());

        QFileInfo fileInfo(QFile::encodeName(fileName));
        QVERIFY(fileInfo.exists());
        // We can't check for an exact size because of the addLocalFile, whose data is system-dependent
        QVERIFY(fileInfo.size() > 450);

    }

    // NOTE The only .tar test, cleanup here
    //QFile::remove(fileName);
}

/**
 * @dataProvider setupData
 */
void KArchiveTest::testCreateTarXXX()
{
    QFETCH(QString, fileName);

    // With    tempfile: 1.3-1.7 ms, 2555089 instr. loads
    // Without tempfile:    0.87 ms,  987915 instr. loads
    QBENCHMARK {

        KTar tar(fileName);
        QVERIFY(tar.open(QIODevice::WriteOnly));

        writeTestFilesToArchive(&tar);

        QVERIFY(tar.close());

        QFileInfo fileInfo(QFile::encodeName(fileName));
        QVERIFY(fileInfo.exists());
        // We can't check for an exact size because of the addLocalFile, whose data is system-dependent
        QVERIFY(fileInfo.size() > 350);

    }
}

//static void compareEntryWithTimestamp(const QString &dateString, const QString &expectedDateString, const QDateTime &expectedDateTime)
// Made it a macro so that line numbers are meaningful on failure

#define compareEntryWithTimestamp(dateString, expectedDateString, expectedDateTime) \
    { \
        /* Take the time from the listing and chop it off */ \
        const QDateTime dt = QDateTime::fromString(dateString.right(19), "dd.MM.yyyy hh:mm:ss"); \
        QString _str(dateString); \
        _str.chop(25); \
        QCOMPARE(_str, expectedDateString); \
        \
        /* Compare the times separately with allowed 2 sec diversion */ \
        QVERIFY(dt.secsTo(expectedDateTime) <= 2); \
    }

/**
 * @dataProvider setupData
 */
void KArchiveTest::testReadTar() // testCreateTarGz must have been run first.
{
    QFETCH(QString, fileName);

    // 1.6-1.7 ms per interaction, 2908428 instruction loads
    // After the "no tempfile when writing fix" this went down
    // to 0.9-1.0 ms, 1689059 instruction loads.
    // I guess it finds the data in the kernel cache now that no KTempFile is
    // used when writing.
    QBENCHMARK {

        KTar tar(fileName);

        QVERIFY(tar.open(QIODevice::ReadOnly));

        const KArchiveDirectory *dir = tar.directory();
        QVERIFY(dir != 0);
        const QStringList listing = recursiveListEntries(dir, "", WithUserGroup | WithTime);

        QFileInfo localFileData("test3");

#ifndef Q_OS_WIN
        QCOMPARE(listing.count(), 15);
#else
        QCOMPARE(listing.count(), 14);
#endif
        QString owner = localFileData.owner().isEmpty() ? getCurrentUserName() : localFileData.owner();
        QString group = localFileData.group().isEmpty() ? getCurrentGroupName() : localFileData.group();
        QString emptyTime = QDateTime().toString("dd.MM.yyyy hh:mm:ss");
        const QDateTime creationTime = QFileInfo(fileName).created();

        compareEntryWithTimestamp(listing[0], QString("mode=40755 user= group= path=aaaemptydir type=dir"), creationTime);

        QCOMPARE(listing[ 1], QString("mode=40777 user=%1 group=%2 path=dir type=dir time=%3").arg(owner).arg(group).arg(emptyTime));
        QCOMPARE(listing[ 2], QString("mode=40777 user=%1 group=%2 path=dir/subdir type=dir time=%3").arg(owner).arg(group).arg(emptyTime));
        compareEntryWithTimestamp(listing[3], QString("mode=100644 user= group= path=dir/subdir/mediumfile2 type=file size=100"), creationTime);
        compareEntryWithTimestamp(listing[4], QString("mode=100644 user=weis group=users path=empty type=file size=0"), creationTime);
        compareEntryWithTimestamp(listing[5], QString("mode=100644 user= group= path=hugefile type=file size=20000"), creationTime);
        compareEntryWithTimestamp(listing[6], QString("mode=100644 user= group= path=mediumfile type=file size=100"), creationTime);
        QCOMPARE(listing[ 7], QString("mode=40777 user=%1 group=%2 path=my type=dir time=").arg(owner).arg(group));
        QCOMPARE(listing[ 8], QString("mode=40777 user=%1 group=%2 path=my/dir type=dir time=").arg(owner).arg(group));
        compareEntryWithTimestamp(listing[9], QString("mode=100644 user=dfaure group=hackers path=my/dir/test3 type=file size=28"), creationTime);
        compareEntryWithTimestamp(listing[10], QString("mode=100440 user=weis group=users path=test1 type=file size=5"), creationTime);
        compareEntryWithTimestamp(listing[11], QString("mode=100644 user=weis group=users path=test2 type=file size=8"), creationTime);
        QCOMPARE(listing[12], QString("mode=40777 user=%1 group=%2 path=z type=dir time=").arg(owner).arg(group));

        // This one was added with addLocalFile, so ignore mode/user/group.
        QString str = listing[13];
        str.replace(QRegExp("mode.*path"), "path");

        compareEntryWithTimestamp(str, QString("path=z/test3 type=file size=13"), creationTime);

#ifndef Q_OS_WIN
        str = listing[14];
        str.replace(QRegExp("mode.*path"), "path");

        compareEntryWithTimestamp(str, QString("path=z/test3_symlink type=file size=0 symlink=test3"), creationTime);
#endif

        QVERIFY(tar.close());
    }
}

/**
 * This tests the decompression using kfilterdev, basically.
 * To debug KTarPrivate::fillTempFile().
 *
 * @dataProvider setupData
 */
void KArchiveTest::testUncompress()
{
    QFETCH(QString, fileName);
    QFETCH(QString, mimeType);

    // testCreateTar must have been run first.
    QVERIFY(QFile::exists(fileName));
    KFilterDev filterDev(fileName);
    QByteArray buffer;
    buffer.resize(8 * 1024);
    //qDebug() << "buffer.size()=" << buffer.size();
    QVERIFY(filterDev.open(QIODevice::ReadOnly));

    qint64 totalSize = 0;
    qint64 len = -1;
    while (!filterDev.atEnd() && len != 0) {
        len = filterDev.read(buffer.data(), buffer.size());
        QVERIFY(len >= 0);
        totalSize += len;
        // qDebug() << "read len=" << len << " totalSize=" << totalSize;
    }
    filterDev.close();
    // qDebug() << "totalSize=" << totalSize;
    QVERIFY(totalSize > 26000); // 27648 here when using gunzip
}

/**
 * @dataProvider setupData
 */
void KArchiveTest::testTarFileData()
{
    QFETCH(QString, fileName);

    // testCreateTar must have been run first.
    KTar tar(fileName);
    QVERIFY(tar.open(QIODevice::ReadOnly));

    testFileData(&tar);

    QVERIFY(tar.close());
}

/**
 * @dataProvider setupData
 */
void KArchiveTest::testTarCopyTo()
{
    QFETCH(QString, fileName);

    // testCreateTar must have been run first.
    KTar tar(fileName);
    QVERIFY(tar.open(QIODevice::ReadOnly));

    testCopyTo(&tar);

    QVERIFY(tar.close());
}

/**
 * @dataProvider setupData
 */
void KArchiveTest::testTarReadWrite()
{
    QFETCH(QString, fileName);

    // testCreateTar must have been run first.
    KTar tar(fileName);
    QVERIFY(tar.open(QIODevice::ReadWrite));

    testReadWrite(&tar);
    testFileData(&tar);

    QVERIFY(tar.close());

    // Reopen it and check it
    {
        KTar tar(fileName);
        QVERIFY(tar.open(QIODevice::ReadOnly));
        testFileData(&tar);
        const KArchiveDirectory *dir = tar.directory();
        const KArchiveEntry *e = dir->entry("newfile");
        QVERIFY(e && e->isFile());
        const KArchiveFile *f = (KArchiveFile *)e;
        QCOMPARE(f->data().size(), 8);
    }

    // NOTE This is the last test for this dataset. so cleanup here
    QFile::remove(fileName);
}

void KArchiveTest::testTarMaxLength_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::newRow("maxlength.tar.gz") << "karchivetest-maxlength.tar.gz";
}

/**
 * @dataProvider testTarMaxLength_data
 */
void KArchiveTest::testTarMaxLength()
{
    QFETCH(QString, fileName);

    KTar tar(fileName);

    QVERIFY(tar.open(QIODevice::WriteOnly));

    // Generate long filenames of each possible length bigger than 98...
    // Also exceed 512 byte block size limit to see how well the ././@LongLink
    // implementation fares
    for (int i = 98; i < 514; i++) {
        QString str, num;
        str.fill('a', i - 10);
        num.setNum(i);
        num = num.rightJustified(10, '0');
        tar.writeFile(str + num, "hum");
    }
    // Result of this test : works perfectly now (failed at 482 formerly and
    // before that at 154).
    QVERIFY(tar.close());

    QVERIFY(tar.open(QIODevice::ReadOnly));

    const KArchiveDirectory *dir = tar.directory();
    QVERIFY(dir != 0);
    const QStringList listing = recursiveListEntries(dir, "", WithUserGroup);

    QCOMPARE(listing[  0], QString("mode=100644 user= group= path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000098 type=file size=3"));
    QCOMPARE(listing[  3], QString("mode=100644 user= group= path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000101 type=file size=3"));
    QCOMPARE(listing[  4], QString("mode=100644 user= group= path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000102 type=file size=3"));

    QCOMPARE(listing.count(), 416);

    QVERIFY(tar.close());

    // NOTE Cleanup here
    QFile::remove(fileName);
}

void KArchiveTest::testTarGlobalHeader()
{
    KTar tar(QFINDTESTDATA(QLatin1String("global_header_test.tar.gz")));
    QVERIFY2(tar.open(QIODevice::ReadOnly), "global_header_test.tar.gz");

    const KArchiveDirectory *dir = tar.directory();
    QVERIFY(dir != 0);

    const QStringList listing = recursiveListEntries(dir, "", WithUserGroup);

    QCOMPARE(listing.count(), 2);

    QCOMPARE(listing[  0], QString("mode=40775 user=root group=root path=Test type=dir"));
    QCOMPARE(listing[  1], QString("mode=664 user=root group=root path=Test/test.txt type=file size=0"));

    QVERIFY(tar.close());
}

void KArchiveTest::testTarPrefix()
{
    KTar tar(QFINDTESTDATA(QLatin1String("tar_prefix_test.tar.gz")));
    QVERIFY2(tar.open(QIODevice::ReadOnly), "tar_prefix_test.tar.gz");

    const KArchiveDirectory *dir = tar.directory();
    QVERIFY(dir != 0);

    const QStringList listing = recursiveListEntries(dir, "", WithUserGroup);

    QCOMPARE(listing[  0], QString("mode=40775 user=root group=root path=Test type=dir"));
    QCOMPARE(listing[  1], QString("mode=40775 user=root group=root path=Test/qt-jambi-qtjambi-4_7 type=dir"));
    QCOMPARE(listing[  2], QString("mode=40775 user=root group=root path=Test/qt-jambi-qtjambi-4_7/examples type=dir"));
    QCOMPARE(listing[  3], QString("mode=40775 user=root group=root path=Test/qt-jambi-qtjambi-4_7/examples/generator type=dir"));
    QCOMPARE(listing[  4], QString("mode=40775 user=root group=root path=Test/qt-jambi-qtjambi-4_7/examples/generator/trolltech_original type=dir"));
    QCOMPARE(listing[  5], QString("mode=40775 user=root group=root path=Test/qt-jambi-qtjambi-4_7/examples/generator/trolltech_original/java type=dir"));
    QCOMPARE(listing[  6], QString("mode=40775 user=root group=root path=Test/qt-jambi-qtjambi-4_7/examples/generator/trolltech_original/java/com type=dir"));
    QCOMPARE(listing[  7], QString("mode=40775 user=root group=root path=Test/qt-jambi-qtjambi-4_7/examples/generator/trolltech_original/java/com/trolltech type=dir"));
    QCOMPARE(listing[  8], QString("mode=40775 user=root group=root path=Test/qt-jambi-qtjambi-4_7/examples/generator/trolltech_original/java/com/trolltech/examples type=dir"));
    QCOMPARE(listing[  9], QString("mode=664 user=root group=root path=Test/qt-jambi-qtjambi-4_7/examples/generator/trolltech_original/java/com/trolltech/examples/GeneratorExample.html type=file size=43086"));

    QCOMPARE(listing.count(), 10);

    QVERIFY(tar.close());
}

void KArchiveTest::testTarDirectoryForgotten()
{
    KTar tar(QFINDTESTDATA(QLatin1String("tar_directory_forgotten.tar.gz")));
    QVERIFY2(tar.open(QIODevice::ReadOnly), "tar_directory_forgotten.tar.gz");

    const KArchiveDirectory *dir = tar.directory();
    QVERIFY(dir != 0);

    const QStringList listing = recursiveListEntries(dir, "", WithUserGroup);

    QVERIFY(listing[9].contains("trolltech/examples/generator"));
    QVERIFY(listing[10].contains("trolltech/examples/generator/GeneratorExample.html"));

    QCOMPARE(listing.count(), 11);

    QVERIFY(tar.close());
}

void KArchiveTest::testTarRootDir() // bug 309463
{
    KTar tar(QFINDTESTDATA(QLatin1String("tar_rootdir.tar.gz")));
    QVERIFY2(tar.open(QIODevice::ReadOnly), qPrintable(tar.fileName()));

    const KArchiveDirectory *dir = tar.directory();
    QVERIFY(dir != 0);

    const QStringList listing = recursiveListEntries(dir, "", WithUserGroup);
    //qDebug() << listing.join("\n");

    QVERIFY(listing[0].contains("%{APPNAME}.cpp"));
    QVERIFY(listing[1].contains("%{APPNAME}.h"));
    QVERIFY(listing[5].contains("main.cpp"));

    QCOMPARE(listing.count(), 10);
}

void KArchiveTest::testTarDirectoryTwice() // bug 206994
{
    KTar tar(QFINDTESTDATA(QLatin1String("tar_directory_twice.tar.gz")));
    QVERIFY(tar.open(QIODevice::ReadOnly));

    const KArchiveDirectory *dir = tar.directory();
    QVERIFY(dir != 0);

    const QStringList listing = recursiveListEntries(dir, "", WithUserGroup);
    //qDebug() << listing.join("\n");

    QVERIFY(listing[0].contains("path=d"));
    QVERIFY(listing[1].contains("path=d/f1.txt"));
    QVERIFY(listing[2].contains("path=d/f2.txt"));

    QCOMPARE(listing.count(), 3);
}
///

static const char s_zipFileName[] = "karchivetest.zip";
static const char s_zipMaxLengthFileName[] = "karchivetest-maxlength.zip";
static const char s_zipLocaleFileName[] = "karchivetest-locale.zip";
static const char s_zipMimeType[] = "application/vnd.oasis.opendocument.text";

void KArchiveTest::testCreateZip()
{
    KZip zip(s_zipFileName);

    QVERIFY(zip.open(QIODevice::WriteOnly));

    zip.setExtraField(KZip::NoExtraField);

    zip.setCompression(KZip::NoCompression);
    QByteArray zipMimeType(s_zipMimeType);
    zip.writeFile("mimetype", zipMimeType);
    zip.setCompression(KZip::DeflateCompression);

    writeTestFilesToArchive(&zip);

    QVERIFY(zip.close());

    QFile zipFile(QFile::encodeName(s_zipFileName));
    QFileInfo fileInfo(zipFile);
    QVERIFY(fileInfo.exists());
    QVERIFY(fileInfo.size() > 300);

    // Check that the header with no-compression and no-extrafield worked.
    // (This is for the "magic" for koffice documents)
    QVERIFY(zipFile.open(QIODevice::ReadOnly));
    QByteArray arr = zipFile.read(4);
    QCOMPARE(arr, QByteArray("PK\003\004"));
    QVERIFY(zipFile.seek(30));
    arr = zipFile.read(8);
    QCOMPARE(arr, QByteArray("mimetype"));
    arr = zipFile.read(zipMimeType.size());
    QCOMPARE(arr, zipMimeType);
}

void KArchiveTest::testCreateZipError()
{
    // Giving a directory name to kzip must give an error case in close(), see #136630.
    // Otherwise we just lose data.
    KZip zip(QDir::currentPath());

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0) // 523440a15f in QSaveFile makes it fail for existing directories, as one would expect.
    QVERIFY(!zip.open(QIODevice::WriteOnly));
#else
    QVERIFY(zip.open(QIODevice::WriteOnly));

    writeTestFilesToArchive(&zip);

    // try to add something as a file that is no file
    QVERIFY(!zip.addLocalFile(QDir::currentPath(), "bogusdir"));

    QVERIFY(!zip.close());
#endif
}

void KArchiveTest::testReadZipError()
{
    QFile brokenZip("broken.zip");
    QVERIFY(brokenZip.open(QIODevice::WriteOnly));

    // incomplete magic
    brokenZip.write(QByteArray("PK\003"));

    brokenZip.close();
    {
        KZip zip("broken.zip");

        QVERIFY(!zip.open(QIODevice::ReadOnly));

        QVERIFY(brokenZip.open(QIODevice::WriteOnly | QIODevice::Append));

        // add rest of magic, but still incomplete header
        brokenZip.write(QByteArray("\004\000\000\000\000"));

        brokenZip.close();

        QVERIFY(!zip.open(QIODevice::ReadOnly));
    }

    QVERIFY(brokenZip.remove());
}

void KArchiveTest::testReadZip()
{
    // testCreateZip must have been run first.
    KZip zip(s_zipFileName);

    QVERIFY(zip.open(QIODevice::ReadOnly));

    const KArchiveDirectory *dir = zip.directory();
    QVERIFY(dir != 0);

    // ZIP has no support for per-file user/group, so omit them from the listing
    const QStringList listing = recursiveListEntries(dir, "", 0);

#ifndef Q_OS_WIN
    QCOMPARE(listing.count(), 16);
#else
    QCOMPARE(listing.count(), 15);
#endif
    QCOMPARE(listing[ 0], QString("mode=40755 path=aaaemptydir type=dir"));
    QCOMPARE(listing[ 1], QString("mode=40777 path=dir type=dir"));
    QCOMPARE(listing[ 2], QString("mode=40777 path=dir/subdir type=dir"));
    QCOMPARE(listing[ 3], QString("mode=100644 path=dir/subdir/mediumfile2 type=file size=100"));
    QCOMPARE(listing[ 4], QString("mode=100644 path=empty type=file size=0"));
    QCOMPARE(listing[ 5], QString("mode=100644 path=hugefile type=file size=20000"));
    QCOMPARE(listing[ 6], QString("mode=100644 path=mediumfile type=file size=100"));
    QCOMPARE(listing[ 7], QString("mode=100644 path=mimetype type=file size=%1").arg(strlen(s_zipMimeType)));
    QCOMPARE(listing[ 8], QString("mode=40777 path=my type=dir"));
    QCOMPARE(listing[ 9], QString("mode=40777 path=my/dir type=dir"));
    QCOMPARE(listing[10], QString("mode=100644 path=my/dir/test3 type=file size=28"));
    QCOMPARE(listing[11], QString("mode=100440 path=test1 type=file size=5"));
    QCOMPARE(listing[12], QString("mode=100644 path=test2 type=file size=8"));
    QCOMPARE(listing[13], QString("mode=40777 path=z type=dir"));
    // This one was added with addLocalFile, so ignore mode
    QString str = listing[14];
    str.replace(QRegExp("mode.*path"), "path");
    QCOMPARE(str, QString("path=z/test3 type=file size=13"));
#ifndef Q_OS_WIN
    str = listing[15];
    str.replace(QRegExp("mode.*path"), "path");
    QCOMPARE(str, QString("path=z/test3_symlink type=file size=5 symlink=test3"));
#endif

    QVERIFY(zip.close());
}

void KArchiveTest::testZipFileData()
{
    // testCreateZip must have been run first.
    KZip zip(s_zipFileName);
    QVERIFY(zip.open(QIODevice::ReadOnly));

    testFileData(&zip);

    QVERIFY(zip.close());
}

void KArchiveTest::testZipCopyTo()
{
    // testCreateZip must have been run first.
    KZip zip(s_zipFileName);
    QVERIFY(zip.open(QIODevice::ReadOnly));

    testCopyTo(&zip);

    QVERIFY(zip.close());
}

void KArchiveTest::testZipMaxLength()
{
    KZip zip(s_zipMaxLengthFileName);

    QVERIFY(zip.open(QIODevice::WriteOnly));

    // Similar to testTarMaxLength just to make sure, but of course zip doesn't have
    // those limitations in the first place.
    for (int i = 98; i < 514; i++) {
        QString str, num;
        str.fill('a', i - 10);
        num.setNum(i);
        num = num.rightJustified(10, '0');
        zip.writeFile(str + num, "hum");
    }
    QVERIFY(zip.close());

    QVERIFY(zip.open(QIODevice::ReadOnly));

    const KArchiveDirectory *dir = zip.directory();
    QVERIFY(dir != 0);
    const QStringList listing = recursiveListEntries(dir, "", 0);

    QCOMPARE(listing[  0], QString("mode=100644 path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000098 type=file size=3"));
    QCOMPARE(listing[  3], QString("mode=100644 path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000101 type=file size=3"));
    QCOMPARE(listing[  4], QString("mode=100644 path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000102 type=file size=3"));

    QCOMPARE(listing.count(), 514 - 98);

    QVERIFY(zip.close());
}

void KArchiveTest::testZipWithNonLatinFileNames()
{
    KZip zip(s_zipLocaleFileName);

    QVERIFY(zip.open(QIODevice::WriteOnly));

    const QByteArray fileData("Test of data with a russian file name");
    const QString fileName = QString::fromUtf8("Архитектура.okular");
    const QString recodedFileName = QFile::decodeName(QFile::encodeName(fileName));
    QVERIFY(zip.writeFile(fileName, fileData));

    QVERIFY(zip.close());

    QVERIFY(zip.open(QIODevice::ReadOnly));

    const KArchiveDirectory *dir = zip.directory();
    QVERIFY(dir != 0);
    const QStringList listing = recursiveListEntries(dir, "", 0);

    QCOMPARE(listing.count(), 1);
    QCOMPARE(listing[0], QString::fromUtf8("mode=100644 path=%1 type=file size=%2").arg(recodedFileName).arg(fileData.size()));

    const KArchiveFile *fileEntry = static_cast< const KArchiveFile * >(dir->entry(dir->entries()[0]));
    QCOMPARE(fileEntry->data(), fileData);
}

void KArchiveTest::testZipWithOverwrittenFileName()
{
    KZip zip(s_zipFileName);

    QVERIFY(zip.open(QIODevice::WriteOnly));

    const QByteArray fileData1("There could be a fire, if there is smoke.");
    const QString fileName = QLatin1String("wisdom");
    QVERIFY(zip.writeFile(fileName, fileData1, 0100644, "konqi", "dragons"));

    // now overwrite it
    const QByteArray fileData2("If there is smoke, there could be a fire.");
    QVERIFY(zip.writeFile(fileName, fileData2, 0100644, "konqi", "dragons"));

    QVERIFY(zip.close());

    QVERIFY(zip.open(QIODevice::ReadOnly));

    const KArchiveDirectory *dir = zip.directory();
    QVERIFY(dir != 0);
    const QStringList listing = recursiveListEntries(dir, "", 0);

    QCOMPARE(listing.count(), 1);
    QCOMPARE(listing[0], QString::fromUtf8("mode=100644 path=%1 type=file size=%2").arg(fileName).arg(fileData2.size()));

    const KArchiveFile *fileEntry = static_cast< const KArchiveFile * >(dir->entry(dir->entries()[0]));
    QCOMPARE(fileEntry->data(), fileData2);
}

static bool writeFile(const QString &dirName, const QString &fileName, const QByteArray &data)
{
    Q_ASSERT(dirName.endsWith('/'));
    QFile file(dirName + fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(data);
    return true;
}

void KArchiveTest::testZipAddLocalDirectory()
{
    // Prepare local dir
    QTemporaryDir tmpDir;
    const QString dirName = tmpDir.path() + '/';

    const QByteArray file1Data = "Hello Shantanu";
    const QString file1 = QLatin1String("file1");
    QVERIFY(writeFile(dirName, file1, file1Data));

    {
        KZip zip(s_zipFileName);

        QVERIFY(zip.open(QIODevice::WriteOnly));
        QVERIFY(zip.addLocalDirectory(dirName, "."));
        QVERIFY(zip.close());
    }
    {
        KZip zip(s_zipFileName);

        QVERIFY(zip.open(QIODevice::ReadOnly));

        const KArchiveDirectory *dir = zip.directory();
        QVERIFY(dir != 0);

        const KArchiveEntry *e = dir->entry(file1);
        QVERIFY(e && e->isFile());
        const KArchiveFile *f = (KArchiveFile *)e;
        QCOMPARE(f->data(), file1Data);
    }
}

void KArchiveTest::testRcc()
{
    const QString rccFile = QFINDTESTDATA("runtime_resource.rcc"); // was copied from qtbase/tests/auto/corelib/io/qresourceengine
    QVERIFY(!rccFile.isEmpty());
    KRcc rcc(rccFile);
    QVERIFY(rcc.open(QIODevice::ReadOnly));
    const KArchiveDirectory *rootDir = rcc.directory();
    QVERIFY(rootDir != 0);
    const KArchiveEntry *rrEntry = rootDir->entry("runtime_resource");
    QVERIFY(rrEntry && rrEntry->isDirectory());
    const KArchiveDirectory *rrDir = static_cast<const KArchiveDirectory *>(rrEntry);
    const KArchiveEntry *fileEntry = rrDir->entry("search_file.txt");
    QVERIFY(fileEntry && fileEntry->isFile());
    const KArchiveFile *searchFile = static_cast<const KArchiveFile *>(fileEntry);
    const QByteArray fileData = searchFile->data();
    QCOMPARE(QString::fromLatin1(fileData), QString::fromLatin1("root\n"));
}

/**
 * @see QTest::cleanupTestCase()
 */
void KArchiveTest::cleanupTestCase()
{
    QFile::remove(s_zipMaxLengthFileName);
    QFile::remove(s_zipFileName);
    QFile::remove(s_zipLocaleFileName);
#ifndef Q_OS_WIN
    QFile::remove("test3_symlink");
#endif
}

///

#if HAVE_XZ_SUPPORT

/**
 * Prepares dataset for archive filter tests
 */
void KArchiveTest::setup7ZipData()
{
    QTest::addColumn<QString>("fileName");
    QTest::newRow(".7z") << "karchivetest.7z";
}

/**
 * @dataProvider testCreate7Zip_data
 */
void KArchiveTest::testCreate7Zip()
{
    QFETCH(QString, fileName);

    QBENCHMARK {

        K7Zip k7zip(fileName);
        QVERIFY(k7zip.open(QIODevice::WriteOnly));

        writeTestFilesToArchive(&k7zip);

        QVERIFY(k7zip.close());

        QFileInfo fileInfo(QFile::encodeName(fileName));
        QVERIFY(fileInfo.exists());
        //qDebug() << "fileInfo.size()" << fileInfo.size();
        // We can't check for an exact size because of the addLocalFile, whose data is system-dependent
        QVERIFY(fileInfo.size() > 390);

    }
}

/**
 * @dataProvider setupData
 */
void KArchiveTest::testRead7Zip() // testCreate7Zip must have been run first.
{
    QFETCH(QString, fileName);

    QBENCHMARK {

        K7Zip k7zip(fileName);

        QVERIFY(k7zip.open(QIODevice::ReadOnly));

        const KArchiveDirectory *dir = k7zip.directory();
        QVERIFY(dir != 0);
        const QStringList listing = recursiveListEntries(dir, "", 0);

        QFileInfo localFileData("test3");

#ifndef Q_OS_WIN
        QCOMPARE(listing.count(), 15);
#else
        QCOMPARE(listing.count(), 14);
#endif
        QCOMPARE(listing[ 0], QString("mode=40755 path=aaaemptydir type=dir"));
        QCOMPARE(listing[ 1], QString("mode=40777 path=dir type=dir"));
        QCOMPARE(listing[ 2], QString("mode=40777 path=dir/subdir type=dir"));
        QCOMPARE(listing[ 3], QString("mode=100644 path=dir/subdir/mediumfile2 type=file size=100"));
        QCOMPARE(listing[ 4], QString("mode=100644 path=empty type=file size=0"));
        QCOMPARE(listing[ 5], QString("mode=100644 path=hugefile type=file size=20000"));
        QCOMPARE(listing[ 6], QString("mode=100644 path=mediumfile type=file size=100"));
        QCOMPARE(listing[ 7], QString("mode=40777 path=my type=dir"));
        QCOMPARE(listing[ 8], QString("mode=40777 path=my/dir type=dir"));
        QCOMPARE(listing[ 9], QString("mode=100644 path=my/dir/test3 type=file size=28"));
        QCOMPARE(listing[10], QString("mode=100440 path=test1 type=file size=5"));
        QCOMPARE(listing[11], QString("mode=100644 path=test2 type=file size=8"));
        QCOMPARE(listing[12], QString("mode=40777 path=z type=dir"));
        // This one was added with addLocalFile, so ignore mode/user/group.
        QString str = listing[13];
        str.replace(QRegExp("mode.*path"), "path");
        QCOMPARE(str, QString("path=z/test3 type=file size=13"));
#ifndef Q_OS_WIN
        str = listing[14];
        str.replace(QRegExp("mode.*path"), "path");
        QCOMPARE(str, QString("path=z/test3_symlink type=file size=0 symlink=test3"));
#endif

        QVERIFY(k7zip.close());

    }
}

/**
 * @dataProvider setupData
 */
void KArchiveTest::test7ZipFileData()
{
    QFETCH(QString, fileName);

    // testCreate7Zip must have been run first.
    K7Zip k7zip(fileName);
    QVERIFY(k7zip.open(QIODevice::ReadOnly));

    testFileData(&k7zip);

    QVERIFY(k7zip.close());
}

/**
 * @dataProvider setupData
 */
void KArchiveTest::test7ZipCopyTo()
{
    QFETCH(QString, fileName);

    // testCreateTar must have been run first.
    K7Zip k7zip(fileName);
    QVERIFY(k7zip.open(QIODevice::ReadOnly));

    testCopyTo(&k7zip);

    QVERIFY(k7zip.close());
}

/**
 * @dataProvider setupData
 */
void KArchiveTest::test7ZipReadWrite()
{
    QFETCH(QString, fileName);

    // testCreate7zip must have been run first.
    K7Zip k7zip(fileName);
    QVERIFY(k7zip.open(QIODevice::ReadWrite));

    testReadWrite(&k7zip);
    testFileData(&k7zip);

    QVERIFY(k7zip.close());

    // Reopen it and check it
    {
        K7Zip k7zip(fileName);
        QVERIFY(k7zip.open(QIODevice::ReadOnly));
        testFileData(&k7zip);
        const KArchiveDirectory *dir = k7zip.directory();
        const KArchiveEntry *e = dir->entry("newfile");
        QVERIFY(e && e->isFile());
        const KArchiveFile *f = (KArchiveFile *)e;
        QCOMPARE(f->data().size(), 8);
    }

    // NOTE This is the last test for this dataset. so cleanup here
    QFile::remove(fileName);
}

/**
 * @dataProvider test7ZipMaxLength_data
 */
void KArchiveTest::test7ZipMaxLength()
{
    QFETCH(QString, fileName);

    K7Zip k7zip(fileName);

    QVERIFY(k7zip.open(QIODevice::WriteOnly));

    // Generate long filenames of each possible length bigger than 98...
    for (int i = 98; i < 514; i++) {
        QString str, num;
        str.fill('a', i - 10);
        num.setNum(i);
        num = num.rightJustified(10, '0');
        k7zip.writeFile(str + num, "hum");
    }
    QVERIFY(k7zip.close());

    QVERIFY(k7zip.open(QIODevice::ReadOnly));

    const KArchiveDirectory *dir = k7zip.directory();
    QVERIFY(dir != 0);
    const QStringList listing = recursiveListEntries(dir, "", 0);

    QCOMPARE(listing[  0], QString("mode=100644 path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000098 type=file size=3"));
    QCOMPARE(listing[  3], QString("mode=100644 path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000101 type=file size=3"));
    QCOMPARE(listing[  4], QString("mode=100644 path=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000000102 type=file size=3"));

    QCOMPARE(listing.count(), 416);

    QVERIFY(k7zip.close());

    // NOTE Cleanup here
    QFile::remove(fileName);
}
#endif
