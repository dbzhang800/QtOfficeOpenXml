/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang
** Contact: hello@debao.me
**
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/
#include "opczippackage.h"
#include "opczippackage_p.h"
#include "opczippackagepart.h"
#include "opczippackagepart_p.h"
#include "opcpackagerelationship_p.h"
#include "opcutils_p.h"

#include <kzip.h>

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QMap>

namespace QtOfficeOpenXml {
namespace Opc {

ZipPackagePrivate::ZipPackagePrivate(const QString &packageName, QIODevice *device, ZipPackage *q)
    :PackagePrivate(packageName, device, q), zipArchive(0)
{
}

ZipPackagePrivate::~ZipPackagePrivate()
{
}

/*!
 * \class ZipPackage
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

    bool ret = true;
    if (mode == QIODevice::ReadOnly || mode == QIODevice::WriteOnly) {
        if ((ret = d->zipArchive->open(mode)))
            d->mode = mode;
    }

    if (ret && mode == QIODevice::ReadOnly)
        d->doLoadPackage();

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

    //Try load "[Content_Types].xml"
    QString contentTypeFilePath = findZipFilePath(QStringLiteral("/[Content_Types].xml"), zipFilePaths);
    if (contentTypeFilePath.isEmpty())
        return false;
    QIODevice *contentTypeStream = root->file(contentTypeFilePath)->createDevice();
    ContentTypeHelper contentTypeHelper;
    contentTypeHelper.loadFromStream(contentTypeStream);
    delete contentTypeStream;

    //Try load "_rels/.rels"
    QString rootRelsFilePath = findZipFilePath(QStringLiteral("/_rels/.rels"), zipFilePaths);
    if (rootRelsFilePath.isEmpty())
        return false;
    QIODevice *rootRelsStream = root->file(rootRelsFilePath)->createDevice();
    relationships = PackageRelationshipHelper::loadRelationshipFromStream(rootRelsStream, QStringLiteral("/"));
    delete rootRelsStream;

    //Load other parts
    foreach (QString name, zipFilePaths) {
        //Skip the "[Content_Types].xml" file.
        if (name == contentTypeFilePath)
            continue;

        //Skip all the .rels files.
        const QString type = contentTypeHelper.contentType(name);
        if (type == QStringLiteral("application/vnd.openxmlformats-package.relationships+xml"))
            continue;

        //Load part
        ZipPackagePart *part = new ZipPackagePart(name, type, q);
        part->d_func()->zipArchive = zipArchive;
        part->d_func()->zipFileEntry = static_cast<const KZipFileEntry *>(root->file(name));
        parts.insert(name.toUpper(), part);

        //Find whether the part has a rels
        QString relsFilePath = findZipFilePath(getRelsPath(name), zipFilePaths);
        if (relsFilePath.isEmpty())
            continue;
        QIODevice *relsStream = root->file(relsFilePath)->createDevice();
        part->d_func()->relationships = PackageRelationshipHelper::loadRelationshipFromStream(relsStream, name);
        delete relsStream;
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
    contentTypeHelper.addContentType(QStringLiteral(".rels"), QStringLiteral("application/vnd.openxmlformats-package.relationships+xml"));
    foreach (PackagePart *part, parts)
        contentTypeHelper.addContentType(part->partName(), part->contentType());
    ZipPartWriteDevice *contentTypeStream = new ZipPartWriteDevice(zipArchive, QStringLiteral("/[Content_Types].xml"));
    contentTypeHelper.saveToStream(contentTypeStream);
    delete contentTypeStream;

    //Write root .rels file.
    ZipPartWriteDevice *rootRelsStream = new ZipPartWriteDevice(zipArchive, QStringLiteral("/_rels/.rels"));
    PackageRelationshipHelper::saveRelationshipToStream(rootRelsStream, relationships.values());
    delete rootRelsStream;

    //Write .rels for all the parts.
    foreach (PackagePart *part, parts) {
        if (!part->relationships().isEmpty()) {
            ZipPartWriteDevice *relsStream = new ZipPartWriteDevice(zipArchive, getRelsPath(part->partName()));
            PackageRelationshipHelper::saveRelationshipToStream(relsStream, part->relationships());
            delete relsStream;
        }
    }
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
 * \class ContentTypeHelper
 *
 * \internal
 * Internal class used by ZipPackage
 */
ContentTypeHelper::ContentTypeHelper()
{

}

void ContentTypeHelper::loadFromStream(QIODevice *device)
{
    defaults.clear();
    overrides.clear();

    QXmlStreamReader reader(device);
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
}

void ContentTypeHelper::saveToStream(QIODevice *device)
{
    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("Types"));
    writer.writeAttribute(QStringLiteral("xmlns"), QStringLiteral("http://schemas.openxmlformats.org/package/2006/content-types"));

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
