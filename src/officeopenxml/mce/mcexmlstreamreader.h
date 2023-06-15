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
#ifndef QTOFFICEOPENXML_MCE_XMLSTREAMREADER_H
#define QTOFFICEOPENXML_MCE_XMLSTREAMREADER_H

#include <QtOfficeOpenXml/officeopenxmlglobal.h>
#include <QtCore/qxmlstream.h>
#include <QtCore/qhash.h>
#include <QtCore/qset.h>

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
#if (QT_VERSION >= QT_VERSION_CHECK(5,13,0))
    QStringView documentVersion() const;
    QStringView documentEncoding() const;
#else
    QStringRef documentVersion() const;
    QStringRef documentEncoding() const;
#endif
    qint64 lineNumber() const;
    qint64 columnNumber() const;
    qint64 characterOffset() const;

    QXmlStreamAttributes attributes() const;

    QString readElementText(QXmlStreamReader::ReadElementTextBehaviour behaviour = QXmlStreamReader::ErrorOnUnexpectedElement);
#if (QT_VERSION >= QT_VERSION_CHECK(5,13,0))
    QStringView name() const;
    QStringView namespaceUri() const;
    QStringView qualifiedName() const;
    QStringView prefix() const;
    QStringView processingInstructionTarget() const;
    QStringView processingInstructionData() const;
    QStringView text() const;
#else
    QStringRef name() const;
    QStringRef namespaceUri() const;
    QStringRef qualifiedName() const;
    QStringRef prefix() const;
    QStringRef processingInstructionTarget() const;
    QStringRef processingInstructionData() const;
    QStringRef text() const;
#endif
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
void Q_OFFICEOPENXML_EXPORT writeCurrentToken(QXmlStreamWriter &writer, const QXmlStreamReader &reader);

} // namespace Mce
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_MCE_XMLSTREAMREADER_H
