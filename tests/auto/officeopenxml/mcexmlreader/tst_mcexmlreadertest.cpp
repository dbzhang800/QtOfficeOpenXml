#include <QString>
#include <QtTest>
#include <QDebug>
#include <QCoreApplication>
#include "mcexmlstreamreader.h"

using namespace QtOfficeOpenXml;

class MceXmlReaderTest : public QObject
{
    Q_OBJECT

public:
    MceXmlReaderTest();

private Q_SLOTS:
    void testIgnorableAttribute();
    void testIgnorableContent();
    void testIgnorableAndProcessContentAttributes();
    void testProcessContentAndExpandedNames();
    void testMustUnderstandAttribute();
    void testAlternateContentMarkup();
    void testAlternateContentMarkupUsingNamespaces();

    void testMceFlag_allowNonUnderstoodNamespaces();
};

MceXmlReaderTest::MceXmlReaderTest()
{
}

void MceXmlReaderTest::testIgnorableAttribute()
{
    QFile file(SRCDIR"data/processing_ignorable_attribute.xml");
    const QString v1("http://schemas.openxmlformats.org/Circles/v1");
    const QString v2("http://schemas.openxmlformats.org/Circles/v2");
    const QString v3("http://schemas.openxmlformats.org/Circles/v3");

    //error will occur if the consumer doesn't support v1.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        //reader.addMceCurrentNamespace(v1);
        reader.readNextStartElement(); //Circles start
        QVERIFY(reader.tokenType() == QXmlStreamReader::Invalid);
        QVERIFY(reader.error() == QXmlStreamReader::CustomError);
        file.close();
    }

    //Both v2 and v3 attributes will be ignored when consumer only support v1.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.readNextStartElement(); //Circles start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));

        reader.readNextStartElement(); //first Circle start
        QXmlStreamAttributes attributes = reader.attributes();
        QCOMPARE(attributes.value("Center").toString(), QString("0,0"));
        QVERIFY(!attributes.hasAttribute(v2, "Opacity"));
        QVERIFY(!attributes.hasAttribute(v3, "Luminance"));
        reader.readNextStartElement(); //first Circle end

        reader.readNextStartElement(); //second Circle start
        attributes = reader.attributes();
        QCOMPARE(attributes.value("Center").toString(), QString("25,0"));
        QVERIFY(!attributes.hasAttribute(v2, "Opacity"));
        QVERIFY(!attributes.hasAttribute(v3, "Luminance"));
        reader.readNextStartElement(); //second Circle end
        file.close();
    }

    //only v3 attributes will be ignored when consumer support v1 and v2
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.addMceUnderstoodNamespace(v2);
        reader.readNextStartElement(); //Circles start

        reader.readNextStartElement(); //first Circle start
        reader.readNextStartElement(); //first Circle end

        reader.readNextStartElement(); //second Circle start
        QXmlStreamAttributes attributes = reader.attributes();
        attributes = reader.attributes();
        QCOMPARE(attributes.value("Center").toString(), QString("25,0"));
        QCOMPARE(attributes.value(v2, "Opacity").toString(), QString("0.5"));
        QVERIFY(!attributes.hasAttribute(v3, "Luminance"));
        reader.readNextStartElement(); //second Circle end
        file.close();
    }

    //no attribute will be ignored when consumer support v1, v2 and v3
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.setMceUnderstoodNamespaces(QSet<QString>()<<v1<<v2<<v3);
        reader.readNextStartElement(); //Circles start

        reader.readNextStartElement(); //first Circle start
        reader.readNextStartElement(); //first Circle end

        reader.readNextStartElement(); //second Circle start
        QXmlStreamAttributes attributes = reader.attributes();
        attributes = reader.attributes();
        QCOMPARE(attributes.value("Center").toString(), QString("25,0"));
        QCOMPARE(attributes.value(v2, "Opacity").toString(), QString("0.5"));
        QCOMPARE(attributes.value(v3, "Luminance").toString(), QString("13"));
        reader.readNextStartElement(); //second Circle end
        file.close();
    }
}

void MceXmlReaderTest::testIgnorableContent()
{
    QFile file(SRCDIR"data/processing_ignorable_content.xml");
    const QString v1("http://schemas.openxmlformats.org/Circles/v1");
    const QString a("http://schemas.openxmlformats.org/MyExtension/v1");
    const QString b(a);

    //elements in both "a" and "b" will be ignored when consumer only support v1.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.readNextStartElement(); //Circles start
        QCOMPARE(int(reader.tokenType()), int(QXmlStreamReader::StartElement));
        QCOMPARE(reader.name().toString(), QString("Circles"));
        reader.readNextStartElement(); //Circles end
        QCOMPARE(int(reader.tokenType()), int(QXmlStreamReader::EndElement));
        QCOMPARE(reader.name().toString(), QString("Circles"));
        file.close();
    }


    //no elements will be ignored when consumer support v1 and a.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.setMceUnderstoodNamespaces(QSet<QString>() << v1 << a);
        reader.readNextStartElement(); //Circles start

        reader.readNextStartElement(); //IgnoreMe start
        QCOMPARE(reader.name().toString(), QString("IgnoreMe"));
        QCOMPARE(reader.namespaceUri().toString(), a);
        reader.readNextStartElement(); //IgnoreMe end

        reader.readNextStartElement(); //IgnoreMeToo start
        QCOMPARE(reader.name().toString(), QString("IgnoreMeToo"));
        QCOMPARE(reader.namespaceUri().toString(), b);
        reader.readNextStartElement(); //IgnoreMeToo end
        file.close();
    }
}

void MceXmlReaderTest::testIgnorableAndProcessContentAttributes()
{
    QFile file(SRCDIR"data/processing_ignorable_and_processcontent_attributes.xml");
    const QString v1("http://schemas.openxmlformats.org/Circles/v1");
    const QString v2("http://schemas.openxmlformats.org/Circles/v2");

    //elements in v2:Blink will be processed directly when consumer only support v1.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.readNextStartElement(); //Circles start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));

        reader.readNextStartElement(); //yellow Circle start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circle"));

        reader.readNextStartElement(); //yellow Circle end
        reader.readNextStartElement(); //green Circle start
        reader.readNextStartElement(); //green Circle end

        reader.readNextStartElement(); //Circles end
        QVERIFY(reader.isEndElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));
        file.close();
    }

    //processContent has no effect when consumer support v1 and v2.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.addMceUnderstoodNamespace(v2);

        reader.readNextStartElement(); //Circles start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));

        reader.readNextStartElement(); // v2:Watermark start
        QCOMPARE(reader.name().toString(), QString("Watermark"));
        reader.skipCurrentElement(); //skip v2:Watermark

        reader.readNextStartElement(); // v2:Blink start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Blink"));

        file.close();
    }
}

void MceXmlReaderTest::testProcessContentAndExpandedNames()
{
    QFile file(SRCDIR"data/processcontent_and_expanded_names.xml");
    const QString v1("http://schemas.openxmlformats.org/Circles/v1");
    const QString extA("http://schemas.openxmlformats.org/Circles/extension");
    const QString extB(extA);

    //elements in extB:Blink should be processed directly when consumer only support v1.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.readNextStartElement(); //Circles start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));

        reader.readNextStartElement(); //blue Circle start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circle"));
        reader.skipCurrentElement();

        reader.readNextStartElement(); //blank Circle start
        reader.skipCurrentElement();
        reader.readNextStartElement(); //red Circle start
        reader.skipCurrentElement();

        reader.readNextStartElement(); //Circles end
        QVERIFY(reader.isEndElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));
        file.close();
    }

    //processContent has no effect when consumer support v1 and extA.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.addMceUnderstoodNamespace(extA);

        reader.readNextStartElement(); //Circles start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));

        reader.readNextStartElement(); // extB:Blink start
        QCOMPARE(reader.name().toString(), QString("Blink"));

        file.close();
    }
}

void MceXmlReaderTest::testMustUnderstandAttribute()
{
    QFile file1(SRCDIR"data/processing_an_attribute_prefixed_qualified_name.xml");
    QFile file2(SRCDIR"data/processing_mustunderstand_attribute.xml");

    const QString v1("http://schemas.openxmlformats.org/Circles/v1");
    const QString v2("http://schemas.openxmlformats.org/Circles/v2");

    //Without MustUnderstand attribute, error wil be triggered
    //when we try to access the element that contains non-understand and
    //non-ignorable attributes
    {
        file1.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file1);
        reader.addMceUnderstoodNamespace(v1);
        reader.readNextStartElement(); //Circles start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));

        while (reader.readNext() != QXmlStreamReader::Invalid);
        QVERIFY(reader.hasError());
        file1.close();
    }

    //With MustUnderstand attribute
    {
        file2.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file2);
        reader.addMceUnderstoodNamespace(v1);
        reader.readNextStartElement(); //Try Circles start
        QVERIFY(reader.hasError());
        file2.close();
    }
}

void MceXmlReaderTest::testAlternateContentMarkup()
{
    QFile file(SRCDIR"data/processing_alternatecontent_markup.xml");
    const QString v1("http://schemas.openxmlformats.org/Circles/v1");
    const QString v2("http://schemas.openxmlformats.org/Circles/v2");
    const QString v3("http://schemas.openxmlformats.org/Circles/v3");

    //Fallback will be selected when consumer only support v1.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.readNextStartElement(); //Circles start
        reader.readNextStartElement(); //LuminanceFilter start
        QCOMPARE(reader.name().toString(), QStringLiteral("LuminanceFilter"));
        reader.skipCurrentElement();

        reader.readNextStartElement(); //Circles end
        QVERIFY(reader.isEndElement());
        QCOMPARE(reader.name().toString(), QStringLiteral("Circles"));
        file.close();
    }

    //First choice will be selected when consumer support v1 and v3.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.addMceUnderstoodNamespace(v3);

        reader.readNextStartElement(); //Circles start
        reader.readNextStartElement(); //Circle start
        QCOMPARE(reader.name().toString(), QStringLiteral("Circle"));
        file.close();
    }
}

void MceXmlReaderTest::testAlternateContentMarkupUsingNamespaces()
{
    QFile file(SRCDIR"data/processing_alternatecontent_markup_using_namespaces.xml");
    const QString v1("http://schemas.openxmlformats.org/Circles/v1");
    const QString m("http://schemas.openxmlformats.org/metallicfinishes/v1");

    //Fallback will be selected when consumer only support v1.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.readNextStartElement(); //Circles start
        reader.readNextStartElement(); //Circle start
        QCOMPARE(reader.attributes().value("Fill").toString(), QStringLiteral("Gold"));
        file.close();
    }

    //The first choice will be selected when consumer support v1 and m.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        reader.addMceUnderstoodNamespace(v1);
        reader.addMceUnderstoodNamespace(m);
        reader.readNextStartElement(); //Circles start
        reader.readNextStartElement(); //Circle start
        QCOMPARE(reader.attributes().value(m, "Finish").toString(), QStringLiteral("GoldLeaf"));
        file.close();
    }
}

void MceXmlReaderTest::testMceFlag_allowNonUnderstoodNamespaces()
{
    QFile file(SRCDIR"data/processing_ignorable_attribute.xml");
    const QString v1("http://schemas.openxmlformats.org/Circles/v1");
    const QString v2("http://schemas.openxmlformats.org/Circles/v2");
    const QString v3("http://schemas.openxmlformats.org/Circles/v3");

    //non-understood and non-ignorable namespace v1 will be treated as understood.
    {
        file.open(QFile::ReadOnly);
        Mce::XmlStreamReader reader(&file);
        //reader.addMceCurrentNamespace(v1);
        reader.setMceParseFlag(Mce::PF_AllowNonUnderstoodNonIngorableNamespaces);

        reader.readNextStartElement(); //Circles start
        QVERIFY(reader.isStartElement());
        QCOMPARE(reader.name().toString(), QString("Circles"));

        reader.readNextStartElement(); //first Circle start
        QXmlStreamAttributes attributes = reader.attributes();
        QCOMPARE(attributes.value("Center").toString(), QString("0,0"));
        QVERIFY(!attributes.hasAttribute(v2, "Opacity"));
        QVERIFY(!attributes.hasAttribute(v3, "Luminance"));
        reader.readNextStartElement(); //first Circle end
    }
}

QTEST_MAIN(MceXmlReaderTest)

#include "tst_mcexmlreadertest.moc"
