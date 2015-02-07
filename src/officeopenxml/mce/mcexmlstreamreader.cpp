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
#include "mcexmlstreamreader_p.h"
#include <QtCore/qstringlist.h>
#include <QtCore/qregularexpression.h>
#include <QtCore/qdebug.h>

namespace QtOfficeOpenXml {
namespace Mce {

static const char * const mcNamespace = "http://schemas.openxmlformats.org/markup-compatibility/2006";

/*!
 * \class QtOfficeOpenXml::Mce::MceXmlElementState
 * \internal
 */

/*!
 * \class QtOfficeOpenXml::Mce::MceXmlElementStateData
 * \internal
 */

/*!
 * \class QtOfficeOpenXml::Mce::MceXmlElementName
 * \internal
 */

/*!
 * \class QtOfficeOpenXml::Mce::XmlStreamReaderPrivate
 * \internal
 */

/*!
 * \fn bool QtOfficeOpenXml::Mce::operator==(const MceXmlElementName &lhs, const MceXmlElementName &rhs)
 * \internal
 */

/*!
 * \fn bool QtOfficeOpenXml::Mce::operator!=(const MceXmlElementName &lhs, const MceXmlElementName &rhs)
 * \internal
 */

/*!
 * \fn uint QtOfficeOpenXml::Mce::qHash(const MceXmlElementName &key, uint seed)
 * \internal
 */

MceXmlElementState::MceXmlElementState()
{
    //The d pointer is initialized with a null pointer
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

QSet<MceXmlElementName> MceXmlElementState::extensionElements() const
{
    if (!d)
        return QSet<MceXmlElementName>();
    return d->extensionElements;
}

QHash<QString, QString> MceXmlElementState::namespacePrefixes() const
{
    if (!d)
        return QHash<QString, QString>();
    return d->namespacePrefixes;
}

bool MceXmlElementState::hasNamespacePrefix(const QString &prefix) const
{
    if (!d)
        return false;
    return d->namespacePrefixes.contains(prefix);
}

QString MceXmlElementState::getNamespaceByPrefix(const QString &prefix) const
{
    if (!d || !d->namespacePrefixes.contains(prefix))
        return QString();
    return d->namespacePrefixes[prefix];
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

void MceXmlElementState::addExtensionElement(const MceXmlElementName &name)
{
    if (!d)
        d = new MceXmlElementStateData();
    d->extensionElements.insert(name);
}

void MceXmlElementState::addExtensionElement(const QString &nsUri, const QString &name)
{
    addExtensionElement(MceXmlElementName(nsUri, name));
}

void MceXmlElementState::addNamespacePrefix(const QString &prefix, const QString &ns)
{
    if (!d)
        d = new MceXmlElementStateData();
    d->namespacePrefixes.insert(prefix, ns);
}

XmlStreamReaderPrivate::XmlStreamReaderPrivate(QXmlStreamReader *reader, XmlStreamReader *q):
    mceParseFlags(0), hasFoundRootElement(false), reader(reader), q_ptr(q)
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
    foreach (const MceXmlElementName mceName, state.extensionElements()) {
        if (extensionElementsCache.contains(mceName))
            extensionElementsCache[mceName]++;
        else
            extensionElementsCache.insert(mceName, 1);
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
        foreach (MceXmlElementName mceName, state.extensionElements()) {
            if (extensionElementsCache[mceName] == 1)
                extensionElementsCache.remove(mceName);
            else
                extensionElementsCache[mceName]--;
        }
    }
    return state;
}

QString XmlStreamReaderPrivate::getNamespaceByPrefix(const QString &prefix) const
{
    QVectorIterator<MceXmlElementState> it(mceElementStateStack);
    it.toBack();
    while(it.hasPrevious()) {
        const MceXmlElementState &state = it.previous();
        if (state.hasNamespacePrefix(prefix))
            return state.getNamespaceByPrefix(prefix);
    }
    return QString();
}

/*
 * Should we do this?
 */
void XmlStreamReaderPrivate::tryInitExtensionElementsCache(const QString &rootNsUri)
{
    if (!hasFoundRootElement) {
        hasFoundRootElement = true;
        if (rootNsUri.startsWith(QLatin1String("http://schemas.openxmlformats.org/"))) {
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/chart"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/chartDrawing"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/main"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://schemas.openxmlformats.org/presentationml/2006/main"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://schemas.openxmlformats.org/spreadsheetml/2006/main"), QStringLiteral("ext")), 1);
        } else if (rootNsUri.startsWith(QLatin1String("http://purl.oclc.org/ooxml/"))) {
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://purl.oclc.org/ooxml/drawingml/chart"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://purl.oclc.org/ooxml/drawingml/chartDrawing"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://purl.oclc.org/ooxml/drawingml/main"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://purl.oclc.org/ooxml/drawingml/spreadsheetDrawing"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://purl.oclc.org/ooxml/presentationml/main"), QStringLiteral("ext")), 1);
            extensionElementsCache.insert(MceXmlElementName(QStringLiteral("http://purl.oclc.org/ooxml/spreadsheetml/main"), QStringLiteral("ext")), 1);
        }
    }
}

void XmlStreamReaderPrivate::doSkipCurrentElemenet_1()
{
    reader->skipCurrentElement();
    if (reader->tokenType() != QXmlStreamReader::Invalid) {
        //we must pop the state element as the end element has been skipped.
        popElementState();
    }
}

/*!
 * \enum QtOfficeOpenXml::Mce::ParseFlag
 *
 * \brief Behavior of the XmlStreamReader.
 *
 * \value PF_AllowNonUnderstoodNonIngorableNamespaces Don't report an error when non-understood and non-ignorable namespace encountered.
 * \value PF_SkipExtensionElements Skip all the application defined extension elements.
 */

/*!
 * \class QtOfficeOpenXml::Mce::XmlStreamReader
 * \inmodule QtOfficeOpenXml
 *
 * \brief XmlStreamReader works more or less like a QXmlStreamReader.
 */

/*! Creates a new stream reader that reads from \a device.
 *
 * \sa setDevice()
 */
XmlStreamReader::XmlStreamReader(QIODevice *device):
    d_ptr(new XmlStreamReaderPrivate(new QXmlStreamReader(device), this))
{
}

/*!
 * Creates a new stream reader that reads from \a data.
 *
 * \sa setDevice()
 */
XmlStreamReader::XmlStreamReader(const QByteArray &data):
    d_ptr(new XmlStreamReaderPrivate(new QXmlStreamReader(data), this))
{

}

/*!
 * Destructs the reader.
 */
XmlStreamReader::~XmlStreamReader()
{
    delete d_ptr->reader;
    delete d_ptr;
}

/*!
 * Returns understood namespaces.
 */
QSet<QString> XmlStreamReader::mceUnderstoodNamespaces() const
{
    Q_D(const XmlStreamReader);
    return d->mceUnderstoodNamespaces;
}

/*!
 * Set understood namespaces to \a nsList.
 */
void XmlStreamReader::setMceUnderstoodNamespaces(const QSet<QString> &nsList)
{
    Q_D(XmlStreamReader);
    d->mceUnderstoodNamespaces = nsList;
}

/*!
 * Add an understood namespace \a ns.
 */
void XmlStreamReader::addMceUnderstoodNamespace(const QString &ns)
{
    Q_D(XmlStreamReader);
    d->mceUnderstoodNamespaces.insert(ns);
}

/*!
 * Enabled or disable the \a flag. \a enabled default to true.
 */
void XmlStreamReader::setMceParseFlag(ParseFlag flag, bool enabled)
{
    Q_D(XmlStreamReader);
    if (enabled)
        d->mceParseFlags |= flag;
    else
        d->mceParseFlags &= ~flag;
}

/*!
    Sets the current device to \a device. Setting the device resets
    the stream to its initial state.

    \sa device()
*/
void XmlStreamReader::setDevice(QIODevice *device)
{
    Q_D(XmlStreamReader);
    d->mceElementStateStack.clear();
    d->ignorableNamespacesCache.clear();
    d->processContentElementCache.clear();
    d->extensionElementsCache.clear();
    d->alternateContentState.clear();
    d->extensionElementState.clear();
    d->hasFoundRootElement = false;
    d->reader->setDevice(device);
}

/*!
    Returns the current device associated with the QXmlStreamReader,
    or 0 if no device has been assigned.

    \sa setDevice()
*/
QIODevice *XmlStreamReader::device() const
{
    Q_D(const XmlStreamReader);
    return d->reader->device();
}

/*!
    Returns \c true if the reader has read until the end of the XML
    document, or if an error() has occurred and reading has been
    aborted. Otherwise, it returns \c false.
*/
bool XmlStreamReader::atEnd() const
{
    Q_D(const XmlStreamReader);
    return d->reader->atEnd();
}

/*!
  Reads the next token and returns its type.

  With one exception, once an error() is reported by readNext(),
  further reading of the XML stream is not possible. Then atEnd()
  returns \c true, hasError() returns \c true, and this function returns
  QXmlStreamReader::Invalid.
*/
QXmlStreamReader::TokenType XmlStreamReader::readNext()
{
    Q_D(XmlStreamReader);
    while (d->doReadNext_1() != QXmlStreamReader::Invalid) {
        //Nothing else should do for extension elements.
        if (d->extensionElementState.inEE)
            break;

        //Deal with AlternateContent
        if (d->reader->isStartElement()) {
            if (d->reader->namespaceUri() == QLatin1String(mcNamespace)) {
                if (d->reader->name() == QLatin1String("AlternateContent")) {
                    if (d->alternateContentState.inAC) {
                        d->reader->raiseError(QStringLiteral("AlternateContent can not live in another AlternateContent"));
                        break;
                    }
                    d->alternateContentState.inAC = true;
                    //Skip this start element only
                    continue;
                } else if (d->reader->name() == QLatin1String("Choice")) {
                    if (!d->alternateContentState.inAC) {
                        d->reader->raiseError(QStringLiteral("Choice cannot be used outside AlternateContent"));
                        break;
                    }
                    if (d->alternateContentState.selected) {
                        //Choice has been selected. Skip this element totally.
                        d->doSkipCurrentElemenet_1();
                        continue;
                    }

                    //Figure out whether this element's requires is satisfied.
                    bool ok = true;
                    QStringRef nsPrefixString = d->reader->attributes().value(QLatin1String("Requires"));
                    if (!nsPrefixString.isEmpty()) {
                        QStringList nsPrefixList = nsPrefixString.toString().split(QRegularExpression(QStringLiteral("[ \\t\\r\\n]+")));
                        foreach (QString nsPrefix, nsPrefixList) {
                            const QString ns = d->getNamespaceByPrefix(nsPrefix);
                            if (ns.isEmpty()) {
                                ok = false;
                                break;
                            }
                            if (!(d->mceUnderstoodNamespaces.contains(ns))) {
                                ok = false;
                                break;
                            }
                        }
                    }

                    if (!ok) {
                        //Skip this element totally.
                        d->doSkipCurrentElemenet_1();
                        continue;
                    }

                    //Select this Choice element. Skip this start element only.
                    d->alternateContentState.selected = true;
                    continue;

                } else if (d->reader->name() == QLatin1String("Fallback")) {
                    if (!d->alternateContentState.inAC) {
                        d->reader->raiseError(QStringLiteral("Choice cannot be used outside AlternateContent"));
                        break;
                    }
                    if (d->alternateContentState.selected) {
                        //Choice has been selected. Skip this element totally.
                        d->doSkipCurrentElemenet_1();
                        continue;
                    }
                    //No Choice has been selected, select this Fallback element.
                    //Skip this start element only.
                    continue;
                }
            }
        } else if (d->reader->isEndElement()) {
            if (d->reader->namespaceUri() == QLatin1String(mcNamespace)) {
                if (d->reader->name() == QLatin1String("AlternateContent"))
                    d->alternateContentState.clear();

                //Just skip the end element.
                continue;
            }
        }

        break;
    }

    return d->reader->tokenType();
}

/*
 * Deal with Ignorable/MustUnderstand/ProcessContent.
 */
QXmlStreamReader::TokenType XmlStreamReaderPrivate::doReadNext_1()
{
    while (reader->readNext() != QXmlStreamReader::Invalid) {

        //When try to read next token just after one Extension Element's end token, clear flags.
        if (extensionElementState.inEE && extensionElementState.depth == 0)
            extensionElementState.clear();

        if (reader->isStartElement()) {
            const QString nsUri = reader->namespaceUri().toString();

            if (!hasFoundRootElement)
                tryInitExtensionElementsCache(nsUri);

            if (extensionElementState.inEE) {
                extensionElementState.depth++;
                break;
            } else {
                //Figure out whether this is an Extension Element or not
                if (extensionElementsCache.contains(MceXmlElementName(nsUri, reader->name().toString()))) {
                    if (mceParseFlags & PF_SkipExtensionElements) {
                        reader->skipCurrentElement();
                        continue;
                    } else {
                        extensionElementState.inEE = true;
                        extensionElementState.depth = 1;
                        break;
                    }
                }
            }

            //Deal with namespace of this element.
            //Todo: should we move this block of code after we parse all other
            //attributes of this element?
            if (nsUri == QLatin1String(mcNamespace) || mceUnderstoodNamespaces.contains(nsUri)) {
                //Ok, for understood namespaces, nothing else need to do.
            } else if (ignorableNamespacesCache.contains(nsUri)) {
                //Ignorable element,
                //Figure out whether we should unwarp it(ProcessContent) or skip it.

                if (!processContentElementCache.isEmpty()
                        && (processContentElementCache.contains(MceXmlElementName(nsUri, reader->name().toString()))
                        ||processContentElementCache.contains(MceXmlElementName(nsUri, QStringLiteral("*"))))) {
                    //ProcessContent: Means just skip this start element.
                    continue;
                } else {
                    //Skip this element totally.
                    reader->skipCurrentElement();
                    if (reader->hasError())
                        break;
                    else
                        continue;
                }
            } else {
                //Error: non-understood and non-ignorable namespace.
                if (mceParseFlags & PF_AllowNonUnderstoodNonIngorableNamespaces) {
                    //Nothing need to do
                } else {
                    reader->raiseError(QStringLiteral("Non-understood and non-ignorable namespace: %1").arg(nsUri));
                    break;
                }
            }

            MceXmlElementState state;

            //Find non-understood namespaces.
            //Note that, more than one prefixs may point to the same one namespace.
            QSet<QString> nonUnderstoodNamespaces;
            foreach (QXmlStreamNamespaceDeclaration decl, reader->namespaceDeclarations()) {
                const QString uri = decl.namespaceUri().toString();
                state.addNamespacePrefix(decl.prefix().toString(), uri);
                if (uri != QLatin1String(mcNamespace) && !mceUnderstoodNamespaces.contains(uri)) {
                    nonUnderstoodNamespaces.insert(uri);
                }
            }

            if (!nonUnderstoodNamespaces.isEmpty()) {
                //Find out non-understood and ignorable namespaces.
                QStringRef nsPrefixString = reader->attributes().value(QLatin1String(mcNamespace), QLatin1String("Ignorable"));
                if (!nsPrefixString.isEmpty()) {
                    QStringList nsPrefixList = nsPrefixString.toString().split(QRegularExpression(QStringLiteral("[ \\t\\r\\n]+")));
                    foreach (QString nsPrefix, nsPrefixList) {
                        QString nsUri = state.getNamespaceByPrefix(nsPrefix);
                        if (nonUnderstoodNamespaces.contains(nsUri)) {
                            state.addIgnorableNamespace(nsUri);
                        }
                    }
                }

                //Deal with MustUnderstand attribute.
                nsPrefixString = reader->attributes().value(QLatin1String(mcNamespace), QLatin1String("MustUnderstand"));
                if (!nsPrefixString.isEmpty()) {
                    QStringList nsPrefixList = nsPrefixString.toString().split(QRegularExpression(QStringLiteral("[ \\t\\r\\n]+")));
                    foreach (QString nsPrefix, nsPrefixList) {
                        QString nsUri = state.getNamespaceByPrefix(nsPrefix);
                        if (!mceUnderstoodNamespaces.contains(nsUri)) {
                            //Error
                            reader->raiseError(QStringLiteral("Don't understand MustUnderstand namespace %1").arg(nsUri));
                            break;
                        }
                    }
                }
                if (reader->hasError())
                    break;

                //Deal with ProcessContent attribute.
                QStringRef pcAttribValue = reader->attributes().value(QLatin1String(mcNamespace), QLatin1String("ProcessContent"));
                if (!pcAttribValue.isEmpty()) {
                    QStringList pcNameList = pcAttribValue.toString().split(QRegularExpression(QStringLiteral("[ \\t\\r\\n]+")));
                    foreach (const QString pcName, pcNameList) {
                        int idx = pcName.indexOf(QLatin1Char(':'));
                        if (idx == -1 || idx == 0 || idx == pcName.size()-1) {
                            reader->raiseError(QStringLiteral("Invalid ProcessContent attribute value %1").arg(pcAttribValue.toString()));
                            break;
                        }
                        QString nsPrefix = pcName.left(idx);
                        if (!state.hasNamespacePrefix(nsPrefix)) {
                            reader->raiseError(QStringLiteral("Invalid ProcessContent attribute value %1").arg(pcAttribValue.toString()));
                            break;
                        }
                        QString ns = state.getNamespaceByPrefix(nsPrefix);
                        if (!nonUnderstoodNamespaces.contains(ns))
                            continue;
                        state.addProcessContentElement(ns, pcName.mid(idx+1));
                    }
                    if (reader->hasError())
                        break;
                }
            }

            //Seems OK now, push the element state to stack, and update the caches.
            pushElementState(state);

            if (!(mceParseFlags & PF_AllowNonUnderstoodNonIngorableNamespaces)) {
                //Make sure all of the attributes are understood or ignorable.
                foreach (const QXmlStreamAttribute attri, reader->attributes()) {
                    const QString ns = attri.namespaceUri().toString();
                    if (!ns.isEmpty() && !mceUnderstoodNamespaces.contains(ns)
                            && !ignorableNamespacesCache.contains(ns) && ns != QLatin1String(mcNamespace)) {
                        reader->raiseError(QStringLiteral("Non-understood and non-ignorable namespace %1 used in attribute %2")
                                           .arg(ns, attri.qualifiedName().toString()));
                        break;
                    }
                }
            }
        } else if (reader->isEndElement()) {
            if (extensionElementState.inEE) {
                extensionElementState.depth--;
                //Don't clear the inEE flag even when depth == 0.
                //clean the flag when try to access next element just after this.
                break;
            }

            //Figure out whether this end element should be skipped.
            const QString nsUri = reader->namespaceUri().toString();
            if (nsUri == QLatin1String(mcNamespace)) {

            } else if (ignorableNamespacesCache.contains(nsUri)) {
                if (!processContentElementCache.isEmpty()
                        && (processContentElementCache.contains(MceXmlElementName(nsUri, reader->name().toString()))
                        ||processContentElementCache.contains(MceXmlElementName(nsUri, QStringLiteral("*"))))) {
                    //ProcessContent: Means just skip this end element.
                    continue;
                }
            }

            //Pop up the element state from the stack.
            popElementState();
        }

        break;
    }
    return reader->tokenType();
}

/*!
  Reads until the next start element within the current element. Returns \c true
  when a start element was reached. When the end element was reached, or when
  an error occurred, false is returned.
*/
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

/*!
  Reads until the end of the current element, skipping any child nodes.
  This function is useful for skipping unknown elements.
*/
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

/*!
  Returns the type of the current token.
 */
QXmlStreamReader::TokenType XmlStreamReader::tokenType() const
{
    Q_D(const XmlStreamReader);
    return d->reader->tokenType();
}

/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isStartDocument() const
  Returns \c true if tokenType() equals StartDocument; otherwise returns \c false.
*/
/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isEndDocument() const
  Returns \c true if tokenType() equals EndDocument; otherwise returns \c false.
*/
/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isStartElement() const
  Returns \c true if tokenType() equals StartElement; otherwise returns \c false.
*/
/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isEndElement() const
  Returns \c true if tokenType() equals EndElement; otherwise returns \c false.
*/
/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isCharacters() const
  Returns \c true if tokenType() equals Characters; otherwise returns \c false.

  \sa isWhitespace(), isCDATA()
*/
/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isComment() const
  Returns \c true if tokenType() equals Comment; otherwise returns \c false.
*/
/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isDTD() const
  Returns \c true if tokenType() equals DTD; otherwise returns \c false.
*/
/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isEntityReference() const
  Returns \c true if tokenType() equals EntityReference; otherwise returns \c false.
*/
/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::isProcessingInstruction() const
  Returns \c true if tokenType() equals ProcessingInstruction; otherwise returns \c false.
*/

/*!  Returns \c true if the reader reports characters that only consist
  of white-space; otherwise returns \c false.

  \sa isCharacters(), text()
*/
bool XmlStreamReader::isWhitespace() const
{
    Q_D(const XmlStreamReader);
    return d->reader->isWhitespace();
}

/*!  Returns \c true if the reader reports characters that stem from a
  CDATA section; otherwise returns \c false.

  \sa isCharacters(), text()
*/
bool XmlStreamReader::isCDATA() const
{
    Q_D(const XmlStreamReader);
    return d->reader->isCDATA();
}

/*!
  Returns \c true if this document has been declared standalone in the
  XML declaration; otherwise returns \c false.

  If no XML declaration has been parsed, this function returns \c false.
 */
bool XmlStreamReader::isStandaloneDocument() const
{
    Q_D(const XmlStreamReader);
    return d->reader->isStandaloneDocument();
}

/*!  If the tokenType() is StartDocument, this function returns the
     version string as specified in the XML declaration.
     Otherwise an empty string is returned.
 */
QStringRef XmlStreamReader::documentVersion() const
{
    Q_D(const XmlStreamReader);
    return d->reader->documentVersion();
}

/*!  If the tokenType() is StartDocument, this function returns the
     encoding string as specified in the XML declaration.
     Otherwise an empty string is returned.
 */
QStringRef XmlStreamReader::documentEncoding() const
{
    Q_D(const XmlStreamReader);
    return d->reader->documentEncoding();
}

/*! Returns the current line number, starting with 1.

\sa columnNumber(), characterOffset()
 */
qint64 XmlStreamReader::lineNumber() const
{
    Q_D(const XmlStreamReader);
    return d->reader->lineNumber();
}

/*! Returns the current column number, starting with 0.

\sa lineNumber(), characterOffset()
 */
qint64 XmlStreamReader::columnNumber() const
{
    Q_D(const XmlStreamReader);
    return d->reader->columnNumber();
}

/*! Returns the current character offset, starting with 0.

\sa lineNumber(), columnNumber()
*/
qint64 XmlStreamReader::characterOffset() const
{
    Q_D(const XmlStreamReader);
    return d->reader->characterOffset();
}

/*!
  Returns the attributes of a StartElement.
 */
QXmlStreamAttributes XmlStreamReader::attributes() const
{
    Q_D(const XmlStreamReader);
    if (d->extensionElementState.inEE)
        return d->reader->attributes();

    QXmlStreamAttributes attributes;
    //Remove all the non-understood and ignorable attributes.
    foreach (const QXmlStreamAttribute attri, d->reader->attributes()) {
        const QString ns = attri.namespaceUri().toString();
        if (!d->ignorableNamespacesCache.contains(ns) || ns == QLatin1String(mcNamespace))
            attributes.append(attri);
    }
    return attributes;
}

/*!  Convenience function to be called in case a StartElement was
  read. Reads until the corresponding EndElement and returns all text
  in-between.

  The \a behaviour defines what happens in case anything else is
  read before reaching EndElement. The function can include the text from
  child elements (useful for example for HTML), ignore child elements, or
  raise an UnexpectedElementError and return what was read so far (default).
 */
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

/*!
  Returns the local name of a StartElement, EndElement, or an EntityReference.

  \sa namespaceUri(), qualifiedName()
 */
QStringRef XmlStreamReader::name() const
{
    Q_D(const XmlStreamReader);
    return d->reader->name();
}

/*!
  Returns the namespaceUri of a StartElement or EndElement.

  \sa name(), qualifiedName()
 */
QStringRef XmlStreamReader::namespaceUri() const
{
    Q_D(const XmlStreamReader);
    return d->reader->namespaceUri();
}

/*!
  Returns the qualified name of a StartElement or EndElement;
*/
QStringRef XmlStreamReader::qualifiedName() const
{
    Q_D(const XmlStreamReader);
    return d->reader->qualifiedName();
}

/*! Returns the prefix of a StartElement or EndElement.

\sa name(), qualifiedName()
*/
QStringRef XmlStreamReader::prefix() const
{
    Q_D(const XmlStreamReader);
    return d->reader->prefix();
}

/*!
  Returns the data of a ProcessingInstruction.
 */
QStringRef XmlStreamReader::processingInstructionData() const
{
    Q_D(const XmlStreamReader);
    return d->reader->processingInstructionData();
}

/*!
  Returns the target of a ProcessingInstruction.
 */
QStringRef XmlStreamReader::processingInstructionTarget() const
{
    Q_D(const XmlStreamReader);
    return d->reader->processingInstructionTarget();
}

/*!  Returns the text of Characters, Comment, DTD, or
  EntityReference.
 */
QStringRef XmlStreamReader::text() const
{
    Q_D(const XmlStreamReader);
    return d->reader->text();
}

/*!  If the tokenType() is StartElement, this function returns the
  element's namespace declarations. Otherwise an empty vector is
  returned.
 */
QXmlStreamNamespaceDeclarations XmlStreamReader::namespaceDeclarations() const
{
    Q_D(const XmlStreamReader);
    if (d->extensionElementState.inEE)
        return d->reader->namespaceDeclarations();

    QXmlStreamNamespaceDeclarations declarations;
    foreach (const QXmlStreamNamespaceDeclaration decl, d->reader->namespaceDeclarations()) {
        const QString ns = decl.namespaceUri().toString();
        if (!d->ignorableNamespacesCache.contains(ns) || ns == QLatin1String(mcNamespace))
            declarations.append(decl);
    }

    return declarations;
}


/*! \fn bool QtOfficeOpenXml::Mce::XmlStreamReader::hasError() const
    Returns \c true if an error has occurred, otherwise \c false.

    \sa errorString(), error()
 */

/*!  Raises a custom error with an optional error \a message.

  \sa error(), errorString()
 */
void XmlStreamReader::raiseError(const QString& message)
{
    Q_D(XmlStreamReader);
    return d->reader->raiseError(message);
}

/*!
  Returns the error message that was set with raiseError().

  \sa error(), lineNumber(), columnNumber(), characterOffset()
 */
QString XmlStreamReader::errorString() const
{
    Q_D(const XmlStreamReader);
    return d->reader->errorString();
}

/*!  Returns the type of the current error, or QXmlStreamReader::NoError if no error occurred.

  \sa errorString(), raiseError()
 */
QXmlStreamReader::Error XmlStreamReader::error() const
{
    Q_D(const XmlStreamReader);
    return d->reader->error();
}

/*!
  \internal
 */
template<class Reader>
void writeCurrentToken(QXmlStreamWriter &writer, const Reader &reader)
{
    //Copy from the source code of Qt5's QXmlStreamWriter.
    switch (reader.tokenType()) {
    case QXmlStreamReader::NoToken:
        break;
    case QXmlStreamReader::StartDocument:
        writer.writeStartDocument(reader.documentVersion().toString(), reader.isStandaloneDocument());
        break;
    case QXmlStreamReader::EndDocument:
        writer.writeEndDocument();
        break;
    case QXmlStreamReader::StartElement: {
        //How to write start element ??
        //1. First write Namespaces then write StartElement(with namespace) ?
        //2. First write StartElement(with namespace) then write Namespaces ?
        //3. First write StartElement(with qualified prefix) then write Namespaces ?
        //4. ??
        QString nsUri = reader.namespaceUri().toString();
        QXmlStreamNamespaceDeclarations namespaceDeclarations = reader.namespaceDeclarations();

        bool nsDeclaredInCurrentElement = false;
        for (int i = 0; i < namespaceDeclarations.size(); ++i) {
            const QXmlStreamNamespaceDeclaration &namespaceDeclaration = namespaceDeclarations.at(i);
            if (namespaceDeclaration.namespaceUri() == nsUri) {
                QString prefix = namespaceDeclaration.prefix().toString();
                if (prefix.isEmpty())
                    writer.writeStartElement(reader.name().toString());
                else
                    writer.writeStartElement(QStringLiteral("%1:%2").arg(prefix, reader.name().toString()));
                nsDeclaredInCurrentElement = true;
                break;
            }
        }
        if (!nsDeclaredInCurrentElement)
            writer.writeStartElement(reader.namespaceUri().toString(), reader.name().toString());
        for (int i = 0; i < namespaceDeclarations.size(); ++i) {
            const QXmlStreamNamespaceDeclaration &namespaceDeclaration = namespaceDeclarations.at(i);
            if (namespaceDeclaration.prefix().isEmpty()) {
                writer.writeDefaultNamespace(namespaceDeclaration.namespaceUri().toString());
            } else {
                writer.writeNamespace(namespaceDeclaration.namespaceUri().toString(),
                                      namespaceDeclaration.prefix().toString());
            }
        }
        writer.writeAttributes(reader.attributes());
             } break;
    case QXmlStreamReader::EndElement:
        writer.writeEndElement();
        break;
    case QXmlStreamReader::Characters:
        if (reader.isCDATA())
            writer.writeCDATA(reader.text().toString());
        else
            writer.writeCharacters(reader.text().toString());
        break;
    case QXmlStreamReader::Comment:
        writer.writeComment(reader.text().toString());
        break;
    case QXmlStreamReader::DTD:
        writer.writeDTD(reader.text().toString());
        break;
    case QXmlStreamReader::EntityReference:
        writer.writeEntityReference(reader.name().toString());
        break;
    case QXmlStreamReader::ProcessingInstruction:
        writer.writeProcessingInstruction(reader.processingInstructionTarget().toString(),
                                   reader.processingInstructionData().toString());
        break;
    default:
        Q_ASSERT(reader.tokenType() != QXmlStreamReader::Invalid);
        qWarning("Mce::writeCurrentToken() with invalid state.");
        break;
    }
}

/*!
  \relates QtOfficeOpenXml::Mce::XmlStreamReader

  Writes the current state of the \a reader. All possible valid
  states are supported.

  The purpose of this function is to support chained processing of XML data.

  \sa XmlStreamReader::tokenType()
 */
void writeCurrentToken(QXmlStreamWriter &writer, const XmlStreamReader &reader)
{
    writeCurrentToken<>(writer, reader);
}

void writeCurrentToken(QXmlStreamWriter &writer, const QXmlStreamReader &reader)
{
    writeCurrentToken<>(writer, reader);
}

} // namespace Mce
} // namespace QtOfficeOpenXml
