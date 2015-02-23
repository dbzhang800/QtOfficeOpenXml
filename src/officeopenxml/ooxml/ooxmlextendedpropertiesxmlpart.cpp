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
#include <private/ooxmlextendedpropertiesxmlpart_p.h>
#include <QtOfficeOpenXml/ooxmlschames.h>
#include <QtOfficeOpenXml/ooxmlabstractdocument.h>
#include <QtOfficeOpenXml/mcexmlstreamreader.h>

#include <QtCore/qvariant.h>

namespace QtOfficeOpenXml {
namespace Ooxml {

/* Internal class used to load/save docProps/App part.
 *
 * This class Operator on the AbstractDocument data directly.
 */

ExtendedPropertiesXmlPart::ExtendedPropertiesXmlPart(ExtendedProperties *data, const QString &partName, QtOfficeOpenXml::Opc::Package *package) :
    AbstractFixedTypeXmlPart(partName, package), m_data(data)
{
}

QString ExtendedPropertiesXmlPart::contentType() const
{
    return QStringLiteral("application/vnd.openxmlformats-officedocument.extended-properties+xml");
}

bool ExtendedPropertiesXmlPart::doLoadFromXml(QIODevice *device, SchameType /*schameType*/)
{
    Mce::XmlStreamReader reader(device);
    //Todo, Add all the understood names used by this part here
    reader.setMceParseFlag(Mce::PF_AllowNonUnderstoodNonIngorableNamespaces);
    reader.setMceParseFlag(Mce::PF_SkipExtensionElements);

    while (!reader.atEnd()) {
         QXmlStreamReader::TokenType token = reader.readNext();
         if (token == QXmlStreamReader::StartElement) {
             if (reader.name() == QLatin1String("Properties"))
                 continue;

             if (reader.name() == QLatin1String("Manager")) {
                 m_data->propertyHash[AbstractDocument::DP_Manager] = reader.readElementText();
             } else if (reader.name() == QLatin1String("Company")) {
                 m_data->propertyHash[AbstractDocument::DP_Company] = reader.readElementText();
             } else if (reader.name() == QLatin1String("Application")) {
                 m_data->propertyHash[AbstractDocument::DP_Application] = reader.readElementText();
             } else if (reader.name() == QLatin1String("AppVersion")) {
                 m_data->propertyHash[AbstractDocument::DP_AppVersion] = reader.readElementText();
             }
         }

         if (reader.hasError()) {
             qWarning("Error when read doc props app file.");
         }
    }
    return true;
}

bool ExtendedPropertiesXmlPart::doSaveToXml(QIODevice *device, SchameType schameType) const
{
    QXmlStreamWriter writer(device);
    QString vt = Schames::namespaceUri(NS_OfficeDocument_DocPropsVTypes, schameType);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("Properties"));
    writer.writeDefaultNamespace(Schames::namespaceUri(NS_OfficeDocument_ExtendedProperties, schameType));
    writer.writeNamespace(vt, Schames::namespacePrefix(vt));
    writer.writeTextElement(QStringLiteral("Application"), getApplicationName());
    writer.writeTextElement(QStringLiteral("DocSecurity"), QStringLiteral("0"));
    writer.writeTextElement(QStringLiteral("ScaleCrop"), QStringLiteral("false"));

#if 0
    //Todo: We should make ExtendedProperties provides these information.
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

    if (m_data->propertyHash.contains(AbstractDocument::DP_Manager))
        writer.writeTextElement(QStringLiteral("Manager"), m_data->propertyHash[AbstractDocument::DP_Manager].toString());
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

QString ExtendedPropertiesXmlPart::getApplicationName() const
{
    if (m_data->propertyHash.contains(AbstractDocument::DP_Application))
        return m_data->propertyHash[AbstractDocument::DP_Application].toString();
    return QStringLiteral("QtOfficeOpenXml Library");
}

/*! \internal
 *  Must in the format XX.XXXX
 */
QString ExtendedPropertiesXmlPart::getApplicationVersion() const
{
    if (m_data->propertyHash.contains(AbstractDocument::DP_AppVersion)) {
        QVariant v = m_data->propertyHash[AbstractDocument::DP_AppVersion];
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

QString ExtendedPropertiesXmlPart::getCompany() const
{
    if (m_data->propertyHash.contains(AbstractDocument::DP_Company))
        return m_data->propertyHash[AbstractDocument::DP_Company].toString();

    return QString();
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
