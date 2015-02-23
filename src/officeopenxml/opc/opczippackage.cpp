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
#include "opczippackage.h"
#include "opczippackage_p.h"
#include "opczippackagepart.h"
#include "opczippackagepart_p.h"
#include "opcutils_p.h"
#include "mcexmlstreamreader.h"

#include <kzip.h>

#include <QXmlStreamWriter>
#include <QDebug>
#include <QMap>

namespace QtOfficeOpenXml {
namespace Opc {

/*!
 * \class QtOfficeOpenXml::Opc::ZipPackagePrivate
 * \internal
 */
ZipPackagePrivate::ZipPackagePrivate(const QString &packageName, QIODevice *device, ZipPackage *q)
    :PackagePrivate(packageName, device, q), zipArchive(0)
{
}

ZipPackagePrivate::~ZipPackagePrivate()
{
}

/*!
 * \class QtOfficeOpenXml::Opc::ZipPackage
 * \inmodule QtOfficeOpenXml
 */
ZipPackage::ZipPackage(const QString &fileName)
    :Package(new ZipPackagePrivate(fileName, 0, this))
{
    d_func()->zipArchive = new KZip(fileName);
}

ZipPackage::ZipPackage(QIODevice *device)
    :Package(new ZipPackagePrivate(QString(), device, this))
{
    d_func()->zipArchive = new KZip(device);
}

ZipPackage::~ZipPackage()
{
    delete d_func()->zipArchive;
}

bool ZipPackage::doOpenPackage(QIODevice::OpenMode mode)
{
    Q_D(ZipPackage);
    //We only support readOnly and writeOnly at present.
    if (mode != QIODevice::ReadOnly && mode != QIODevice::WriteOnly)
        return false;

    if (d->zipArchive->open(mode)) {
        if (mode == QIODevice::ReadOnly && !d->doLoadPackage()) {
            //Not a valid OPC package, return false.
            d->zipArchive->close();
            return false;
        }
        return true;
    }

    return false;
}

bool ZipPackage::doClosePackage()
{
    Q_D(ZipPackage);
    if (mode() == QIODevice::WriteOnly)
        d->doSavePackage();
    return d->zipArchive->close();
}

static QStringList extractZipFilePaths(const KArchiveDirectory *dir, const QString &dirPath)
{
    QStringList paths;
    foreach (QString entryName, dir->entries()) {
        const KArchiveEntry *entry = dir->entry(entryName);
        const QString entryPath = QStringLiteral("%1/%2").arg(dirPath, entryName);
        if (entry->isDirectory()) {
            paths.append(extractZipFilePaths(static_cast<const KArchiveDirectory *>(entry), entryPath));
        } else {
            paths.append(entryPath);
        }
    }
    return paths;
}
static QString findZipFilePath(const QString &partName, const QStringList &zipFilePaths)
{
    if (zipFilePaths.contains(partName))
        return partName;

    foreach (QString realPath, zipFilePaths) {
        if (realPath.toUpper() == partName.toUpper())
            return realPath;
    }

    return QString();
}

bool ZipPackagePrivate::doLoadPackage()
{
    Q_Q(ZipPackage);

    const KArchiveDirectory *root = zipArchive->directory();
    QStringList zipFilePaths = extractZipFilePaths(root, QString());

    //Try load "[Content_Types].xml", which is not a part of OPC package.
    QString contentTypeFilePath = findZipFilePath(QStringLiteral("/[Content_Types].xml"), zipFilePaths);
    if (contentTypeFilePath.isEmpty())
        return false;
    QIODevice *contentTypeStream = root->file(contentTypeFilePath)->createDevice();
    ContentTypeHelper contentTypeHelper;
    contentTypeHelper.loadFromStream(contentTypeStream);
    delete contentTypeStream;

    //Load parts
    foreach (QString name, zipFilePaths) {
        //Skip the "[Content_Types].xml" file.
        if (name == contentTypeFilePath)
            continue;

        //Load part
        ZipPackagePart *part = new ZipPackagePart(name, contentTypeHelper.contentType(name), q);
        part->d_func()->zipArchive = zipArchive;
        part->d_func()->zipFileEntry = static_cast<const KZipFileEntry *>(root->file(name));
        parts.insert(name.toUpper(), part);
    }
    return true;
}

/*
  Will be called in QIODevice::WriteOnly mode.
*/
bool ZipPackagePrivate::doSavePackage()
{
    //Generate the "[Content_Types].xml" file
    ContentTypeHelper contentTypeHelper;
    foreach (PackagePart *part, parts)
        contentTypeHelper.addContentType(part->partName(), part->contentType());
    ZipPartWriteDevice *contentTypeStream = new ZipPartWriteDevice(zipArchive, QStringLiteral("/[Content_Types].xml"));
    contentTypeHelper.saveToStream(contentTypeStream);
    delete contentTypeStream;

    return true;
}

PackagePart *ZipPackage::doCreatePart(const QString &partName, const QString &contentType)
{
    Q_D(ZipPackage);
    ZipPackagePart *part = new ZipPackagePart(partName, contentType, this);
    part->d_func()->zipArchive = d->zipArchive;

    return part;
}

bool ZipPackage::doDeletePart(const QString &partName)
{
    delete part(partName);
    return true;
}

/*!
 * \class QtOfficeOpenXml::Opc::ContentTypeHelper
 * \internal
 *
 * \brief Internal class used by ZipPackage
 */
ContentTypeHelper::ContentTypeHelper()
{

}

void ContentTypeHelper::loadFromStream(QIODevice *device)
{
    defaults.clear();
    overrides.clear();

    Mce::XmlStreamReader reader(device);
    reader.addMceUnderstoodNamespace(QString::fromLatin1(NamespaceIds::contentTypes));
    reader.setMceParseFlag(Mce::PF_SkipExtensionElements);

    while (!reader.atEnd()) {
        if (reader.readNextStartElement()) {
            if (reader.name() == QLatin1String("Default")) {
                QXmlStreamAttributes attrs = reader.attributes();
                QString extension = attrs.value(QLatin1String("Extension")).toString();
                QString type = attrs.value(QLatin1String("ContentType")).toString();
                defaults.insert(extension.toUpper(), DefaultData(extension, type));
            } else if (reader.name() == QLatin1String("Override")) {
                QXmlStreamAttributes attrs = reader.attributes();
                QString partName = attrs.value(QLatin1String("PartName")).toString();
                QString type = attrs.value(QLatin1String("ContentType")).toString();
                overrides.insert(partName.toUpper(), OverrideData(partName, type));
            }
        }
    }
    if (reader.hasError() && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        qWarning()<<reader.errorString();
}

void ContentTypeHelper::saveToStream(QIODevice *device)
{
    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("Types"));
    writer.writeDefaultNamespace(QString::fromLatin1(NamespaceIds::contentTypes));

    foreach (DefaultData data, defaults) {
        writer.writeStartElement(QStringLiteral("Default"));
        writer.writeAttribute(QStringLiteral("Extension"), data.extension);
        writer.writeAttribute(QStringLiteral("ContentType"), data.contentType);
        writer.writeEndElement();//Default
    }

    foreach (OverrideData data, overrides) {
        writer.writeStartElement(QStringLiteral("Override"));
        writer.writeAttribute(QStringLiteral("PartName"), data.partName);
        writer.writeAttribute(QStringLiteral("ContentType"), data.contentType);
        writer.writeEndElement(); //Override
    }

    writer.writeEndElement();//Types
    writer.writeEndDocument();
}

/*
  Adds the Default entry if it is the first time we come across
  the extension for the partName, does nothing if the content type
  corresponding to the default entry for the extension matches or
  adds a override corresponding to this part and content type.
  This call is made when a new part is being added to the package.

  Note, partName is case-insensitive.
*/
void ContentTypeHelper::addContentType(const QString &partName, const QString &contentType)
{
   const QString name = partName.toUpper();
   int idx = name.lastIndexOf(QLatin1Char('.'));
   if (idx != -1) {
       const QString ext = name.mid(idx+1);
       if (!defaults.contains(ext)) {
           defaults.insert(ext, DefaultData(partName.mid(idx+1), contentType));
           return;
       }
   }

   overrides.insert(name, OverrideData(partName, contentType));
}

QString ContentTypeHelper::contentType(const QString &partName)
{
    const QString name = partName.toUpper();
    if (overrides.contains(name))
        return overrides[name].contentType;

    int idx = name.lastIndexOf(QLatin1Char('.'));
    if (idx != -1) {
        const QString ext = name.mid(idx+1);
        if (defaults.contains(ext))
            return defaults[ext].contentType;
    }
    return QString();
}

} // namespace Opc
} // namespace QtOfficeOpenXml
