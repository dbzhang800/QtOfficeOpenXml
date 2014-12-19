#include <QString>
#include <QtTest>
#include <QTemporaryFile>

#include "opczippackage.h"
#include "opcpackagepart.h"
#include "opcpackageproperties.h"

using namespace QtOfficeOpenXml;

class ZippackageTest : public QObject
{
    Q_OBJECT

public:
    ZippackageTest();

private Q_SLOTS:
    void testPackageRead();
    void testPackageWrite();
};

ZippackageTest::ZippackageTest()
{
}

void ZippackageTest::testPackageRead()
{
    Opc::ZipPackage package(SRCDIR"test.xlsx");
    package.open(QIODevice::ReadOnly);

    QCOMPARE(package.parts().size(), 12);
    QVERIFY(package.part("/_rels/.rels"));
    QVERIFY(package.part("/xl/_rels/workbook.xml.rels"));
    QVERIFY(package.part("/xl/workbook.xml"));
    QVERIFY(package.part("/xl/theme/theme1.xml"));
    QCOMPARE(package.part("/xl/workbook.xml")->contentType(), QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml"));

    Opc::PackagePart *part = package.part("/xl/styles.xml");
    QIODevice *stream = part->getDevice();
    QVERIFY(stream);
    QVERIFY(stream->readAll().startsWith("<?xml version="));
    part->releaseDevice();

    Opc::PackageProperties *properties = package.packageProperties();
    QCOMPARE(properties->creator(), QStringLiteral("Debao Zhang"));
    QCOMPARE(properties->created(), QDateTime(QDate(2006, 9, 16), QTime(), Qt::UTC));

    QList<Opc::PackageRelationship *> rootRelations = package.relationships();
    QCOMPARE(rootRelations.size(), 3);
}

void ZippackageTest::testPackageWrite()
{
    QTemporaryFile tempFile;
    tempFile.open();
    //tempFile.setAutoRemove(false);
    //qDebug()<<tempFile.fileName();

    //Write some things to generate a package.
    Opc::ZipPackage package1(&tempFile);
    package1.open(QIODevice::WriteOnly);
    Opc::PackageProperties *props = package1.packageProperties();
    props->setDescription("Test only!");
    props->setModified(QDateTime(QDate(2014,1,1), QTime(), Qt::UTC));
    Opc::PackagePart *p1p1 = package1.createPart("/p1/part1.par", "application/par");
    p1p1->getDevice()->write("Hello Qt Office Open Xml!!");
    p1p1->releaseDevice();
    Opc::PackagePart *p1p2 = package1.createPart("/p2/part2.par", "application/par");
    p1p2->getDevice()->write("Hi");
    p1p2->releaseDevice();
    package1.createRelationship(p1p1->partName(), Opc::Internal, "http://type1");
    p1p1->createRelationship(p1p2->partName(), Opc::Internal, "http://type2");
    package1.close();

    //Load the package to vertify whether it's right.
    tempFile.open();
    Opc::ZipPackage package2(&tempFile);
    package2.open(QIODevice::ReadOnly);

    QCOMPARE(package2.packageProperties()->description(), QStringLiteral("Test only!"));
    QCOMPARE(package2.packageProperties()->modified(), QDateTime(QDate(2014,1,1), QTime(), Qt::UTC));

    QCOMPARE(package2.relationships().size(), 2);
    QCOMPARE(package2.parts().size(), 5);
    Opc::PackagePart *p2p1 = package2.part("/p1/part1.par");
    QVERIFY(p2p1);
    QCOMPARE(p2p1->contentType(), QStringLiteral("application/par"));
    QCOMPARE(p2p1->getDevice()->readAll(), QByteArray("Hello Qt Office Open Xml!!"));
    p2p1->releaseDevice();
    QCOMPARE(p2p1->relationships().size(), 1);

    Opc::PackagePart *p2p2 = package2.part("/p2/part2.par");
    QVERIFY(p2p2);
    QCOMPARE(p2p2->getDevice()->readAll(), QByteArray("Hi"));
    p2p2->releaseDevice();
}

QTEST_APPLESS_MAIN(ZippackageTest)

#include "tst_zippackagetest.moc"
