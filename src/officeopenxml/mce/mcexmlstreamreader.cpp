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
#include "mcexmlstreamreader_p.h"
#include <QtCore/qstringlist.h>
#include <QtCore/qregularexpression.h>
#include <QtCore/qdebug.h>

namespace QtOfficeOpenXml {
namespace Mce {

static const char * const mcNamespace = "http://schemas.openxmlformats.org/markup-compatibility/2006";

XmlStreamReaderPrivate::XmlStreamReaderPrivate(QXmlStreamReader *reader, XmlStreamReader *q):
    reader(reader), q_ptr(q)
{
}

XmlStreamReaderPrivate::~XmlStreamReaderPrivate()
{
}

/*!
 * \class XmlStreamReader
 */

XmlStreamReader::XmlStreamReader(QIODevice *device):
    d_ptr(new XmlStreamReaderPrivate(new QXmlStreamReader(device), this))
{
}

XmlStreamReader::~XmlStreamReader()
{
    delete d_ptr->reader;
    delete d_ptr;
}

QSet<QString> XmlStreamReader::mceCurrentNamespaces() const
{
    Q_D(const XmlStreamReader);
    return d->mceCurrentNamespaces;
}

QHash<QString, QString> XmlStreamReader::mceObsoleteNamespaces() const
{
    Q_D(const XmlStreamReader);
    return d->mceObsoleteNamespaces;
}

void XmlStreamReader::setMceCurrentNamespaces(const QSet<QString> &nsList)
{
    Q_D(XmlStreamReader);
    d->mceCurrentNamespaces = nsList;
}

void XmlStreamReader::setMceObsoleteNamespaces(const QHash<QString, QString> &nsMap)
{
    Q_D(XmlStreamReader);
    d->mceObsoleteNamespaces = nsMap;
}

void XmlStreamReader::addMceCurrentNamespace(const QString &ns)
{
    Q_D(XmlStreamReader);
    d->mceCurrentNamespaces.insert(ns);
}

void XmlStreamReader::addMceObsoleteNamespace(const QString &obsoleteNs, const QString &currentNs)
{
    Q_D(XmlStreamReader);
    d->mceObsoleteNamespaces.insert(obsoleteNs, currentNs);
}

void XmlStreamReader::setDevice(QIODevice *device)
{
    Q_D(XmlStreamReader);
    d->nonUnderstoodNamespacesStack.clear();
    d->nonUnderstoodNamespacesCache.clear();
    d->reader->setDevice(device);
}

QIODevice *XmlStreamReader::device() const
{
    Q_D(const XmlStreamReader);
    return d->reader->device();
}

bool XmlStreamReader::atEnd() const
{
    Q_D(const XmlStreamReader);
    return d->reader->atEnd();
}

QXmlStreamReader::TokenType XmlStreamReader::readNext()
{
    Q_D(XmlStreamReader);
    while (!d->reader->atEnd() && !d->reader->hasError()) {
        d->reader->readNext();

        if (d->reader->isStartElement()) {
            //Figure out whether this element should be skiped
            if (d->nonUnderstoodNamespacesCache.contains(d->reader->namespaceUri().toString())) {
                d->reader->skipCurrentElement();
                if (d->reader->hasError())
                    break;
                else
                    continue;
            }

            //Find non understood namespaces.
            //Note that, more than one prefixs may point to the same one namespace.
            QHash<QString, QString> namespacePrefixHash;
            QSet<QString> nonUnderstoodNamespaces;
            foreach (QXmlStreamNamespaceDeclaration decl, d->reader->namespaceDeclarations()) {
                const QString uri = decl.namespaceUri().toString();
                namespacePrefixHash.insert(decl.prefix().toString(), uri);
                if (uri != QLatin1String(mcNamespace) && !d->mceCurrentNamespaces.contains(uri)
                        && !d->mceObsoleteNamespaces.contains(uri)) {
                    nonUnderstoodNamespaces.insert(uri);
                }
            }

            if (!nonUnderstoodNamespaces.isEmpty()) {
                //vertify whether all the non-understood namespaces are are ignorable.
                QString nsPrefixString = d->reader->attributes().value(QLatin1String(mcNamespace), QLatin1String("Ignorable")).toString();
                if (nsPrefixString.isEmpty()) {
                    raiseError(QStringLiteral("Non understood namespace found %1").arg(QStringList(namespacePrefixHash.values()).join(QLatin1Char(' '))));
                } else {
                    QStringList nsPrefixList = nsPrefixString.split(QRegularExpression(QStringLiteral("[ \\t\\r\\n]+")));
                    QSet<QString> ignorableNamespaces;
                    foreach (QString nsPrefix, nsPrefixList)
                        ignorableNamespaces.insert(namespacePrefixHash[nsPrefix]);

                    QSetIterator<QString> it(nonUnderstoodNamespaces);
                    while (it.hasNext()) {
                        const QString &ns = it.next();
                        if(!ignorableNamespaces.contains(ns)) {
                            raiseError(QStringLiteral("Non understood namespace found %1").arg(ns));
                            break;
                        }
                    }
                }
                if (d->reader->hasError())
                    break;

                //Push all the non-understood and ignorable ns to the stack.
                d->nonUnderstoodNamespacesStack.push(nonUnderstoodNamespaces);
                foreach (const QString ns, nonUnderstoodNamespaces) {
                    if (d->nonUnderstoodNamespacesCache.contains(ns))
                        d->nonUnderstoodNamespacesCache[ns]++;
                    else
                        d->nonUnderstoodNamespacesCache.insert(ns, 1);
                }
            } else {
                d->nonUnderstoodNamespacesStack.push(QSet<QString>());
            }
            break;
        } else if (d->reader->isEndElement()) {
            //Pop up the non-understood and ignorable ns from the stack.
            QSet<QString> nonUnderstoodNamespaces = d->nonUnderstoodNamespacesStack.pop();
            foreach (QString ns, nonUnderstoodNamespaces) {
                if (d->nonUnderstoodNamespacesCache[ns] == 1)
                    d->nonUnderstoodNamespacesCache.remove(ns);
                else
                    d->nonUnderstoodNamespacesCache[ns]--;
            }
            break;
        } else {
            break;
        }
    }

    return d->reader->tokenType();
}

bool XmlStreamReader::readNextStartElement()
{
    //we can not call QXmlStreamReader::readNextStartElement() directly here.
    //as we must call our own readNext().
    while (readNext() != QXmlStreamReader::Invalid) {
        if (isEndElement())
            return false;
        else if (isStartElement())
            return true;
    }
    return false;
}

void XmlStreamReader::skipCurrentElement()
{
    //we can not call QXmlStreamReader::skipCurrentElement() directly here.
    //as we must call our own readNext().
    int depth = 1;
    while (depth && readNext() != QXmlStreamReader::Invalid) {
        if (isEndElement())
            --depth;
        else if (isStartElement())
            ++depth;
    }
}

QXmlStreamReader::TokenType XmlStreamReader::tokenType() const
{
    Q_D(const XmlStreamReader);
    return d->reader->tokenType();
}

QXmlStreamAttributes XmlStreamReader::attributes() const
{
    Q_D(const XmlStreamReader);
    QXmlStreamAttributes attributes;
    //Remove all the non-understood and ignorable attributes.
    foreach (QXmlStreamAttribute attri, d->reader->attributes()) {
        if (!d->nonUnderstoodNamespacesCache.contains(attri.namespaceUri().toString()))
            attributes.append(attri);
    }
    return attributes;
}

QString XmlStreamReader::readElementText(QXmlStreamReader::ReadElementTextBehaviour behaviour)
{
    Q_D(XmlStreamReader);
    return d->reader->readElementText(behaviour);
}

QStringRef XmlStreamReader::name() const
{
    Q_D(const XmlStreamReader);
    return d->reader->name();
}

QStringRef XmlStreamReader::namespaceUri() const
{
    Q_D(const XmlStreamReader);
    return d->reader->namespaceUri();
}

QStringRef XmlStreamReader::qualifiedName() const
{
    Q_D(const XmlStreamReader);
    return d->reader->qualifiedName();
}

QStringRef XmlStreamReader::prefix() const
{
    Q_D(const XmlStreamReader);
    return d->reader->prefix();
}

QXmlStreamNamespaceDeclarations XmlStreamReader::namespaceDeclarations() const
{
    Q_D(const XmlStreamReader);
    return d->reader->namespaceDeclarations();
}

void XmlStreamReader::raiseError(const QString& message)
{
    Q_D(XmlStreamReader);
    return d->reader->raiseError(message);
}

QString XmlStreamReader::errorString() const
{
    Q_D(const XmlStreamReader);
    return d->reader->errorString();
}

QXmlStreamReader::Error XmlStreamReader::error() const
{
    Q_D(const XmlStreamReader);
    return d->reader->error();
}

} // namespace Mce
} // namespace QtOfficeOpenXml
