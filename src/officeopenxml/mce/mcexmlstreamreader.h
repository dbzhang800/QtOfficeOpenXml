/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang <hello@debao.me>
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
#ifndef QTOFFICEOPENXML_MCE_XMLSTREAMREADER_H
#define QTOFFICEOPENXML_MCE_XMLSTREAMREADER_H

#include <QtOfficeOpenXml/ooxmlglobal.h>
#include <QtCore/qxmlstream.h>
#include <QtCore/qhash.h>
#include <QtCore/qset.h>

class QXmlStreamWriter;

namespace QtOfficeOpenXml {
namespace Mce {

enum ParseFlag
{
    PF_AllowNonUnderstoodNonIngorableNamespaces = 0x01,
    PF_SkipExtensionElements = 0x02
};

class XmlStreamReaderPrivate;
class Q_OFFICEOPENXML_EXPORT XmlStreamReader
{
    Q_DECLARE_PRIVATE(XmlStreamReader)
public:
    explicit XmlStreamReader(QIODevice *device);
    explicit XmlStreamReader(const QByteArray &data);

    ~XmlStreamReader();

    QSet<QString> mceUnderstoodNamespaces() const;
    void setMceUnderstoodNamespaces(const QSet<QString> &nsList);
    void addMceUnderstoodNamespace(const QString &ns);
    void setMceParseFlag(ParseFlag flag, bool enabled = true);

    void setDevice(QIODevice *device);
    QIODevice *device() const;

    bool atEnd() const;
    QXmlStreamReader::TokenType readNext();

    bool readNextStartElement();
    void skipCurrentElement();

    QXmlStreamReader::TokenType tokenType() const;

    inline bool isStartDocument() const { return tokenType() == QXmlStreamReader::StartDocument; }
    inline bool isEndDocument() const { return tokenType() == QXmlStreamReader::EndDocument; }
    inline bool isStartElement() const { return tokenType() == QXmlStreamReader::StartElement; }
    inline bool isEndElement() const { return tokenType() == QXmlStreamReader::EndElement; }
    inline bool isCharacters() const { return tokenType() == QXmlStreamReader::Characters; }
    bool isWhitespace() const;
    bool isCDATA() const;
    inline bool isComment() const { return tokenType() == QXmlStreamReader::Comment; }
    inline bool isDTD() const { return tokenType() == QXmlStreamReader::DTD; }
    inline bool isEntityReference() const { return tokenType() == QXmlStreamReader::EntityReference; }
    inline bool isProcessingInstruction() const { return tokenType() == QXmlStreamReader::ProcessingInstruction; }

    bool isStandaloneDocument() const;
    QStringRef documentVersion() const;
    QStringRef documentEncoding() const;

    qint64 lineNumber() const;
    qint64 columnNumber() const;
    qint64 characterOffset() const;

    QXmlStreamAttributes attributes() const;

    QString readElementText(QXmlStreamReader::ReadElementTextBehaviour behaviour = QXmlStreamReader::ErrorOnUnexpectedElement);

    QStringRef name() const;
    QStringRef namespaceUri() const;
    QStringRef qualifiedName() const;
    QStringRef prefix() const;
    QStringRef processingInstructionTarget() const;
    QStringRef processingInstructionData() const;
    QStringRef text() const;

    QXmlStreamNamespaceDeclarations namespaceDeclarations() const;

    void raiseError(const QString& message = QString());
    QString errorString() const;
    QXmlStreamReader::Error error() const;

    inline bool hasError() const
    {
        return error() != QXmlStreamReader::NoError;
    }

private:
    XmlStreamReaderPrivate *d_ptr;
};

void Q_OFFICEOPENXML_EXPORT writeCurrentToken(QXmlStreamWriter &writer, const XmlStreamReader &reader);

} // namespace Mce
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_MCE_XMLSTREAMREADER_H
