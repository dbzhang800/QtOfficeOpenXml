/****************************************************************************
**
** Copyright (C) 2014-2015 Debao Zhang <hello@debao.me>
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
#include <private/ooxmldocpropsapppart_p.h>
#include <private/ooxmlabstractdocument_p.h>
#include <QtOfficeOpenXml/ooxmlschames.h>
#include <QtOfficeOpenXml/mcexmlstreamreader.h>

#include <QtCore/qvariant.h>

namespace QtOfficeOpenXml {
namespace Ooxml {

/* Internal class used to load/save docProps/App part.
 *
 * This class Operator on the AbstractDocument data directly.
 */

DocPropsAppPart::DocPropsAppPart(AbstractDocumentPrivate *documentData) :
    m_documentData(documentData)
{
}

bool DocPropsAppPart::doLoadFromXml(QIODevice *part)
{
    Mce::XmlStreamReader reader(part);
    //Todo, Add all the understood names used by this part here
    reader.setMceParseFlag(Mce::PF_AllowNonUnderstoodNonIngorableNamespaces);
    reader.setMceParseFlag(Mce::PF_SkipExtensionElements);

    while (!reader.atEnd()) {
         QXmlStreamReader::TokenType token = reader.readNext();
         if (token == QXmlStreamReader::StartElement) {
             if (reader.name() == QLatin1String("Properties"))
                 continue;

             if (reader.name() == QLatin1String("Manager")) {
                 m_documentData->documentPropertyHash[AbstractDocument::DP_Manager] = reader.readElementText();
             } else if (reader.name() == QLatin1String("Company")) {
                 m_documentData->documentPropertyHash[AbstractDocument::DP_Company] = reader.readElementText();
             } else if (reader.name() == QLatin1String("Application")) {
                 m_documentData->documentPropertyHash[AbstractDocument::DP_Application] = reader.readElementText();
             } else if (reader.name() == QLatin1String("AppVersion")) {
                 m_documentData->documentPropertyHash[AbstractDocument::DP_AppVersion] = reader.readElementText();
             }
         }

         if (reader.hasError()) {
             qWarning("Error when read doc props app file.");
         }
    }
    return true;
}

bool DocPropsAppPart::doSaveToXml(QIODevice *part, SchameType schameType) const
{
    QXmlStreamWriter writer(part);
    QString vt = Schames::namespaceUri(NS_OfficeDocument_DocPropsVTypes, schameType);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("Properties"));
    writer.writeDefaultNamespace(Schames::namespaceUri(NS_OfficeDocument_ExtendedProperties, schameType));
    writer.writeNamespace(vt, Schames::namespacePrefix(vt));
    writer.writeTextElement(QStringLiteral("Application"), getApplicationName());
    writer.writeTextElement(QStringLiteral("DocSecurity"), QStringLiteral("0"));
    writer.writeTextElement(QStringLiteral("ScaleCrop"), QStringLiteral("false"));

#if 0
    //Todo: We should make AbstractDocumentPrivate provides these information.
    writer.writeStartElement(QStringLiteral("HeadingPairs"));
    writer.writeStartElement(vt, QStringLiteral("vector"));
    writer.writeAttribute(QStringLiteral("size"), QString::number(m_headingPairsList.size()*2));
    writer.writeAttribute(QStringLiteral("baseType"), QStringLiteral("variant"));
    typedef QPair<QString,int> PairType; //Make foreach happy
    foreach (PairType pair, m_headingPairsList) {
        writer.writeStartElement(vt, QStringLiteral("variant"));
        writer.writeTextElement(vt, QStringLiteral("lpstr"), pair.first);
        writer.writeEndElement(); //vt:variant
        writer.writeStartElement(vt, QStringLiteral("variant"));
        writer.writeTextElement(vt, QStringLiteral("i4"), QString::number(pair.second));
        writer.writeEndElement(); //vt:variant
    }
    writer.writeEndElement();//vt:vector
    writer.writeEndElement();//HeadingPairs

    writer.writeStartElement(QStringLiteral("TitlesOfParts"));
    writer.writeStartElement(vt, QStringLiteral("vector"));
    writer.writeAttribute(QStringLiteral("size"), QString::number(m_titlesOfPartsList.size()));
    writer.writeAttribute(QStringLiteral("baseType"), QStringLiteral("lpstr"));
    foreach (QString title, m_titlesOfPartsList)
        writer.writeTextElement(vt, QStringLiteral("lpstr"), title);
    writer.writeEndElement();//vt:vector
    writer.writeEndElement();//TitlesOfParts
#endif

    if (m_documentData->documentPropertyHash.contains(AbstractDocument::DP_Manager))
        writer.writeTextElement(QStringLiteral("Manager"), m_documentData->documentPropertyHash[AbstractDocument::DP_Manager].toString());
    //Not like "manager", "company" always exists for Excel generated file.
    writer.writeTextElement(QStringLiteral("Company"), getCompany());
    writer.writeTextElement(QStringLiteral("LinksUpToDate"), QStringLiteral("false"));
    writer.writeTextElement(QStringLiteral("SharedDoc"), QStringLiteral("false"));
    writer.writeTextElement(QStringLiteral("HyperlinksChanged"), QStringLiteral("false"));
    writer.writeTextElement(QStringLiteral("AppVersion"), getApplicationVersion());

    writer.writeEndElement(); //Properties
    writer.writeEndDocument();
    return true;
}

QString DocPropsAppPart::getApplicationName() const
{
    if (m_documentData->documentPropertyHash.contains(AbstractDocument::DP_Application))
        return m_documentData->documentPropertyHash[AbstractDocument::DP_Application].toString();
    return QStringLiteral("QtOfficeOpenXml Library");
}

/*! \internal
 *  Must in the format XX.XXXX
 */
QString DocPropsAppPart::getApplicationVersion() const
{
    if (m_documentData->documentPropertyHash.contains(AbstractDocument::DP_AppVersion)) {
        QVariant v = m_documentData->documentPropertyHash[AbstractDocument::DP_AppVersion];
        if (v.userType() == QMetaType::QString) {
            QString s = v.toString();
            //For valid version in string format, return directly.
            if (s.length() == 7 && s[2]==QLatin1Char('.'))
                return s;
        }

        //Try Convert to an valid string format if it's a number.
        bool ok = true;
        double dVal = v.toDouble(&ok);
        if (ok && (dVal >=0 && dVal < 100))
            return QString("%1").arg(dVal, 7, 'f', 4, QLatin1Char('0'));
    }

    //Should we return the current version of QtOfficeOpenXml here?
    return QStringLiteral("00.0000");
}

QString DocPropsAppPart::getCompany() const
{
    if (m_documentData->documentPropertyHash.contains(AbstractDocument::DP_Company))
        return m_documentData->documentPropertyHash[AbstractDocument::DP_Company].toString();

    return QString();
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
