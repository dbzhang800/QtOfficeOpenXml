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

#include <QtOfficeOpenXml/mcexmlstreamreader.h>
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
        , extensionElements(other.extensionElements)
        , namespacePrefixes(other.namespacePrefixes)
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
    int mceParseFlags;

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
