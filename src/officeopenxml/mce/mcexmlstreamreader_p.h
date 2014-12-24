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
        :QSharedData(other), nonUnderstoodNamespaces(other.nonUnderstoodNamespaces)
        , processContentNeededElements(other.processContentNeededElements)
    {}
    ~MceXmlElementStateData(){}

    QSet<QString> nonUnderstoodNamespaces;
    QSet<MceXmlElementName> processContentNeededElements;
};

class MceXmlElementState
{
public:
    MceXmlElementState();
    MceXmlElementState(const QSet<QString> &nonUnderstoodNsSet);
    MceXmlElementState(const MceXmlElementState &other);
    ~MceXmlElementState();

    bool isNull() const;
    QSet<QString> nonUnderstoodNamespaces() const;
    QSet<MceXmlElementName> processContentNeededElements() const;
    void setNonUnderstoodNamespaces(const QSet<QString> &nss);
    void setProcessContentNeededElements(const QSet<MceXmlElementName> &names);
    void addNonUnderstoodNamespace(const QString &ns);
    void addProcessContentNeededElement(const MceXmlElementName &name);
    void addProcessContentNeededElement(const QString &nsUri, const QString &name);

private:
    QSharedDataPointer<MceXmlElementStateData> d;
};

class XmlStreamReaderPrivate
{
    Q_DECLARE_PUBLIC(XmlStreamReader)
public:
    XmlStreamReaderPrivate(QXmlStreamReader *reader, XmlStreamReader *q);
    ~XmlStreamReaderPrivate();

    QSet<QString> mceCurrentNamespaces;
    QHash<QString, QString> mceObsoleteNamespaces;
    QHash<QString, int> nonUnderstoodNamespacesCache;
    QStack<MceXmlElementState> mceElementStateStack;

    QXmlStreamReader *reader;
    XmlStreamReader *q_ptr;
};

} // namespace Mce
} // namespace QtOfficeOpenXml

#endif // MCEXMLSTREAMREADER_P_H
