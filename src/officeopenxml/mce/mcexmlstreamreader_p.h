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
#ifndef MCEXMLSTREAMREADER_P_H
#define MCEXMLSTREAMREADER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Office Open Xml API.  It exists for
// the convenience of the Qt Office Open Xml.  This header file may
// change from version to version without notice, or even be removed.
//
// We mean it.
//

#include "mcexmlstreamreader.h"
#include <QtCore/qstringlist.h>
#include <QtCore/qstack.h>
#include <QtCore/qshareddata.h>

namespace QtOfficeOpenXml {
namespace Mce {

class MceXmlElementName {
public:
    MceXmlElementName() {}
    MceXmlElementName(const QString &nsUri, const QString &name)
        :nsUri(nsUri), name(name)
    {}
    MceXmlElementName(const MceXmlElementName &other)
        :nsUri(other.nsUri), name(other.name)
    {}
    ~MceXmlElementName() {}
    MceXmlElementName &operator = (const MceXmlElementName &other)
    {
        if (this != &other) {
            nsUri = other.nsUri;
            name = other.name;
        }
        return *this;
    }

    QString nsUri;
    QString name;
};

inline bool operator==(const MceXmlElementName &lhs, const MceXmlElementName &rhs)
{
    return lhs.nsUri == rhs.nsUri && lhs.name == rhs.name;
}

inline bool operator!=(const MceXmlElementName &lhs, const MceXmlElementName &rhs)
{
    return lhs.nsUri != rhs.nsUri || lhs.name != rhs.name;
}

inline uint qHash(const MceXmlElementName &key, uint seed)
{
    return qHash(QStringLiteral("%1\n%2").arg(key.nsUri, key.name), seed);
}


class MceXmlElementStateData : public QSharedData
{
public:
    MceXmlElementStateData() {}
    MceXmlElementStateData(const MceXmlElementStateData &other)
        :QSharedData(other), ignorableNamespaces(other.ignorableNamespaces)
        , processContentNeededElements(other.processContentNeededElements)
    {}
    ~MceXmlElementStateData(){}

    QSet<QString> ignorableNamespaces;
    QSet<MceXmlElementName> processContentNeededElements;
    QSet<MceXmlElementName> extensionElements;
    QHash<QString, QString> namespacePrefixes;
};

class MceXmlElementState
{
public:
    MceXmlElementState();
    MceXmlElementState(const MceXmlElementState &other);
    ~MceXmlElementState();

    bool isNull() const;
    QSet<QString> ignorableNamespaces() const;
    QSet<MceXmlElementName> processContentElements() const;
    QSet<MceXmlElementName> extensionElements() const;
    QHash<QString, QString> namespacePrefixes() const;
    bool hasNamespacePrefix(const QString &prefix) const;
    QString getNamespaceByPrefix(const QString &prefix) const;
    void setIgnorableNamespaces(const QSet<QString> &nss);
    void setProcessContentElements(const QSet<MceXmlElementName> &names);
    void addIgnorableNamespace(const QString &ns);
    void addProcessContentElement(const MceXmlElementName &name);
    void addProcessContentElement(const QString &nsUri, const QString &name);
    void addExtensionElement(const MceXmlElementName &name);
    void addExtensionElement(const QString &nsUri, const QString &name);
    void addNamespacePrefix(const QString &prefix, const QString &ns);

private:
    QSharedDataPointer<MceXmlElementStateData> d;
};

class XmlStreamReaderPrivate
{
    Q_DECLARE_PUBLIC(XmlStreamReader)
public:
    XmlStreamReaderPrivate(QXmlStreamReader *reader, XmlStreamReader *q);
    ~XmlStreamReaderPrivate();

    QXmlStreamReader::TokenType doReadNext_1();
    void doSkipCurrentElemenet_1();
    void pushElementState(const MceXmlElementState &state);
    MceXmlElementState popElementState();
    QString getNamespaceByPrefix(const QString &prefix) const;
    void tryInitExtensionElementsCache(const QString &rootNsUri);

    QSet<QString> mceUnderstoodNamespaces;

    QHash<QString, int> ignorableNamespacesCache;
    QHash<MceXmlElementName, int> processContentElementCache;
    QHash<MceXmlElementName, int> extensionElementsCache;
    QStack<MceXmlElementState> mceElementStateStack;

    //Ecma Office Open Xml 10.2.1
    //"An AlternateContent element shall not be the child of an AlternateContent element."
    struct AlternateContentState
    {
        AlternateContentState():inAC(false), selected(false) {}
        void clear()
        {
            inAC = false;
            selected = false;
        }

        bool inAC;
        bool selected;

    } alternateContentState;

    //in application-defined extension element or not
    struct ExtensionElementState
    {
        ExtensionElementState():inEE(false), depth(0) {}
        void clear()
        {
            inEE = false;
            depth = 0;
        }

        bool inEE;
        int depth;
    } extensionElementState;

    int extensionElementDepth;
    bool hasFoundRootElement;

    QXmlStreamReader *reader;
    XmlStreamReader *q_ptr;
};

} // namespace Mce
} // namespace QtOfficeOpenXml

#endif // MCEXMLSTREAMREADER_P_H
