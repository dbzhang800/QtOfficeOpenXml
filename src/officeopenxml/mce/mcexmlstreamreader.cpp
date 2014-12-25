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

MceXmlElementState::MceXmlElementState()
{
    //The d pointer is initialized with a null pointer
}

MceXmlElementState::MceXmlElementState(const QSet<QString> &ignorableNss)
    :d(new MceXmlElementStateData)
{
    d->ignorableNamespaces = ignorableNss;
}

MceXmlElementState::MceXmlElementState(const MceXmlElementState &other)
    :d(other.d)
{
}

MceXmlElementState::~MceXmlElementState()
{

}

bool MceXmlElementState::isNull() const
{
    return !d;
}

QSet<QString> MceXmlElementState::ignorableNamespaces() const
{
    if (!d)
        return QSet<QString>();
    return d->ignorableNamespaces;
}

QSet<MceXmlElementName> MceXmlElementState::processContentElements() const
{
    if (!d)
        return QSet<MceXmlElementName>();
    return d->processContentNeededElements;
}

void MceXmlElementState::setIgnorableNamespaces(const QSet<QString> &nss)
{
    if (!d)
        d = new MceXmlElementStateData();
    d->ignorableNamespaces = nss;
}

void MceXmlElementState::setProcessContentElements(const QSet<MceXmlElementName> &names)
{
    if (!d)
        d = new MceXmlElementStateData();
    d->processContentNeededElements = names;
}

void MceXmlElementState::addIgnorableNamespace(const QString &ns)
{
    if (!d)
        d = new MceXmlElementStateData();
    d->ignorableNamespaces.insert(ns);
}

void MceXmlElementState::addProcessContentElement(const MceXmlElementName &name)
{
    if (!d)
        d = new MceXmlElementStateData();
    d->processContentNeededElements.insert(name);
}

void MceXmlElementState::addProcessContentElement(const QString &nsUri, const QString &name)
{
    addProcessContentElement(MceXmlElementName(nsUri, name));
}

XmlStreamReaderPrivate::XmlStreamReaderPrivate(QXmlStreamReader *reader, XmlStreamReader *q):
    reader(reader), q_ptr(q)
{
}

XmlStreamReaderPrivate::~XmlStreamReaderPrivate()
{
}

void XmlStreamReaderPrivate::pushElementState(const MceXmlElementState &state)
{
    mceElementStateStack.push(state);
    if (state.isNull())
        return;

    //Update caches.
    foreach (const QString ns, state.ignorableNamespaces()) {
        if (ignorableNamespacesCache.contains(ns))
            ignorableNamespacesCache[ns]++;
        else
            ignorableNamespacesCache.insert(ns, 1);
    }
    foreach (const MceXmlElementName mceName, state.processContentElements()) {
        if (processContentElementCache.contains(mceName))
            processContentElementCache[mceName]++;
        else
            processContentElementCache.insert(mceName, 1);
    }
}

MceXmlElementState XmlStreamReaderPrivate::popElementState()
{
    MceXmlElementState state = mceElementStateStack.pop();
    if (!state.isNull()) {
        //Update caches.
        foreach (QString ns, state.ignorableNamespaces()) {
            if (ignorableNamespacesCache[ns] == 1)
                ignorableNamespacesCache.remove(ns);
            else
                ignorableNamespacesCache[ns]--;
        }
        foreach (MceXmlElementName mceName, state.processContentElements()) {
            if (processContentElementCache[mceName] == 1)
                processContentElementCache.remove(mceName);
            else
                processContentElementCache[mceName]--;
        }
    }
    return state;
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
    d->mceElementStateStack.clear();
    d->ignorableNamespacesCache.clear();
    d->processContentElementCache.clear();
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

            //Deal with namespace of this element.
            //Todo: should we move this block of code after we parse all other
            //attributes of this element?
            const QString nsUri = d->reader->namespaceUri().toString();
            if (nsUri == QLatin1String(mcNamespace)) {

            } else if (d->mceCurrentNamespaces.contains(nsUri)
                       || d->mceObsoleteNamespaces.contains(nsUri)) {
                //Ok valid supported, nothing need to do.

            } else if (d->ignorableNamespacesCache.contains(nsUri)) {
                //Ignorable or ProcessContent.

                if (!d->processContentElementCache.isEmpty()
                        && (d->processContentElementCache.contains(MceXmlElementName(nsUri, d->reader->name().toString()))
                        ||d->processContentElementCache.contains(MceXmlElementName(nsUri, QStringLiteral("*"))))) {
                    //ProcessContent: Means just skip this start element.
                    continue;
                } else {
                    //Skip this element totally.
                    d->reader->skipCurrentElement();
                    if (d->reader->hasError())
                        break;
                    else
                        continue;
                }
            } else {
                //Error: non-understood and non-ignorable namespace.
                d->reader->raiseError(QStringLiteral("Non-understood and non-ignorable namespace: %1").arg(nsUri));
                break;
            }


            MceXmlElementState state;

            //Find non-understood namespaces.
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
                //Find out non-understood and ignorable namespaces.
                QStringRef nsPrefixString = d->reader->attributes().value(QLatin1String(mcNamespace), QLatin1String("Ignorable"));
                if (!nsPrefixString.isEmpty()) {
                    QStringList nsPrefixList = nsPrefixString.toString().split(QRegularExpression(QStringLiteral("[ \\t\\r\\n]+")));
                    foreach (QString nsPrefix, nsPrefixList) {
                        QString nsUri = namespacePrefixHash[nsPrefix];
                        if (nonUnderstoodNamespaces.contains(nsUri)) {
                            state.addIgnorableNamespace(nsUri);
                        }
                    }
                }

                //Deal with MustUnderstand attribute.
                nsPrefixString = d->reader->attributes().value(QLatin1String(mcNamespace), QLatin1String("MustUnderstand"));
                if (!nsPrefixString.isEmpty()) {
                    QStringList nsPrefixList = nsPrefixString.toString().split(QRegularExpression(QStringLiteral("[ \\t\\r\\n]+")));
                    foreach (QString nsPrefix, nsPrefixList) {
                        QString nsUri = namespacePrefixHash[nsPrefix];
                        if (!d->mceCurrentNamespaces.contains(nsUri) && !d->mceObsoleteNamespaces.contains(nsUri)) {
                            //Error
                            d->reader->raiseError(QStringLiteral("Don't understand MustUnderstand namespace %1").arg(nsUri));
                            break;
                        }
                    }
                }
                if (d->reader->hasError())
                    break;

                //Deal with ProcessContent attribute.
                QStringRef pcAttribValue = d->reader->attributes().value(QLatin1String(mcNamespace), QLatin1String("ProcessContent"));
                if (!pcAttribValue.isEmpty()) {
                    QStringList pcNameList = pcAttribValue.toString().split(QRegularExpression(QStringLiteral("[ \\t\\r\\n]+")));
                    foreach (const QString pcName, pcNameList) {
                        int idx = pcName.indexOf(QLatin1Char(':'));
                        if (idx == -1 || idx == 0 || idx == pcName.size()-1) {
                            raiseError(QStringLiteral("Invalid ProcessContent attribute value %1").arg(pcAttribValue.toString()));
                            break;
                        }
                        QString nsPrefix = pcName.left(idx);
                        if (!namespacePrefixHash.contains(nsPrefix)) {
                            raiseError(QStringLiteral("Invalid ProcessContent attribute value %1").arg(pcAttribValue.toString()));
                            break;
                        }
                        QString ns = namespacePrefixHash[nsPrefix];
                        if (!nonUnderstoodNamespaces.contains(ns))
                            continue;
                        state.addProcessContentElement(ns, pcName.mid(idx+1));
                    }
                    if (d->reader->hasError())
                        break;
                }
            }

            //Seems OK now, push the element state to stack, and update the caches.
            d->pushElementState(state);

            //Make sure all of the attributes are understood or ignorable.
            foreach (const QXmlStreamAttribute attri, d->reader->attributes()) {
                const QString ns = attri.namespaceUri().toString();
                if (!ns.isEmpty() && !d->mceCurrentNamespaces.contains(ns) && !d->mceObsoleteNamespaces.contains(ns)
                        && !d->ignorableNamespacesCache.contains(ns) && ns != QLatin1String(mcNamespace)) {
                    d->reader->raiseError(QStringLiteral("Non-understood and non-ignorable namespace %1 used in attribute %2")
                                          .arg(ns, attri.qualifiedName().toString()));
                    break;
                }
            }
        } else if (d->reader->isEndElement()) {
            //Figure out whether this end element should be skipped.
            const QString nsUri = d->reader->namespaceUri().toString();
            if (nsUri == QLatin1String(mcNamespace)) {

            } else if (d->ignorableNamespacesCache.contains(nsUri)) {
                if (!d->processContentElementCache.isEmpty()
                        && (d->processContentElementCache.contains(MceXmlElementName(nsUri, d->reader->name().toString()))
                        ||d->processContentElementCache.contains(MceXmlElementName(nsUri, QStringLiteral("*"))))) {
                    //ProcessContent: Means just skip this end element.
                    continue;
                }
            }

            //Pop up the element state from the stack.
            d->popElementState();
        }

        break;
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
    foreach (const QXmlStreamAttribute attri, d->reader->attributes()) {
        const QString ns = attri.namespaceUri().toString();
        if (!d->ignorableNamespacesCache.contains(ns) || ns == QLatin1String(mcNamespace))
            attributes.append(attri);
    }
    return attributes;
}

QString XmlStreamReader::readElementText(QXmlStreamReader::ReadElementTextBehaviour behaviour)
{
    //we can not call QXmlStreamReader::readElementText() directly here.
    //as we must call our own readNext().
    if (isStartElement()) {
        QString result;
        forever {
            switch (readNext()) {
            case QXmlStreamReader::Characters:
            case QXmlStreamReader::EntityReference:
                result += text();
                break;
            case QXmlStreamReader::EndElement:
                return result;
            case QXmlStreamReader::ProcessingInstruction:
            case QXmlStreamReader::Comment:
                break;
            case QXmlStreamReader::StartElement:
                if (behaviour == QXmlStreamReader::SkipChildElements) {
                    skipCurrentElement();
                    break;
                } else if (behaviour == QXmlStreamReader::IncludeChildElements) {
                    result += readElementText(behaviour);
                    break;
                }
                // Fall through (for ErrorOnUnexpectedElement)
            default:
                if (hasError() || behaviour == QXmlStreamReader::ErrorOnUnexpectedElement) {
                    if (hasError())
                        raiseError(QStringLiteral("Expected character data."));
                    return result;
                }
            }
        }
    }
    return QString();
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

QStringRef XmlStreamReader::text() const
{
    Q_D(const XmlStreamReader);
    return d->reader->text();
}

QXmlStreamNamespaceDeclarations XmlStreamReader::namespaceDeclarations() const
{
    Q_D(const XmlStreamReader);
    QXmlStreamNamespaceDeclarations declarations;
    foreach (const QXmlStreamNamespaceDeclaration decl, d->reader->namespaceDeclarations()) {
        const QString ns = decl.namespaceUri().toString();
        if (!d->ignorableNamespacesCache.contains(ns) || ns == QLatin1String(mcNamespace))
            declarations.append(decl);
    }

    return declarations;
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
