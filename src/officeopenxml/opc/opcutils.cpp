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

#include "opcutils_p.h"
#include <QDir>
#include <QStringList>

namespace QtOfficeOpenXml {
namespace Opc {

/*!
  \class QtOfficeOpenXml::Opc::ContentTypes
  \internal
 */

/*!
  \class QtOfficeOpenXml::Opc::RelationshipTypes
  \internal
 */

/*!
  \class QtOfficeOpenXml::Opc::NamespaceIds
  \internal
 */

/*!
 * \internal
 */
QStringList splitPartName(const QString &partName)
{
    int idx = partName.lastIndexOf(QLatin1Char('/'));
    if (idx == -1)
        return QStringList()<<QStringLiteral(".")<<partName;

    if (idx == 0)
        return QStringList()<<partName<<QString();

    return QStringList()<<partName.left(idx)<<partName.mid(idx+1);
}

/*!
 * \internal
 */
QString getAbsolutePartName(const QString &baseName, const QString &relativeName)
{
    QString path = QDir(splitPartName(baseName)[0]).absoluteFilePath(relativeName);
    return QDir::cleanPath(path);
}

/*!
 * \internal
 */
QString getRelativePartName(const QString &baseName, const QString &absoluteName)
{
    QString path = QDir(splitPartName(baseName)[0]).relativeFilePath(absoluteName);
    return QDir::cleanPath(path);
}

/*!
   \internal
   Return the .rel file path based on partName
 */
QString getRelsPath(const QString &partName)
{
    int idx = partName.lastIndexOf(QLatin1Char('/'));
    if (idx == -1)
        return QString();

    return QString(partName.left(idx) + QLatin1String("/_rels/")
                   + partName.mid(idx+1) + QLatin1String(".rels"));
}

const char * const ContentTypes::coreProperties = "application/vnd.openxmlformats-package.core-properties+xml";
const char * const ContentTypes::relationships = "application/vnd.openxmlformats-package.relationships+xml";
const char * const ContentTypes::digitalSignatureCertificate = "application/vnd.openxmlformats-package.digital-signaturecertificate";
const char * const ContentTypes::digitalSignatureOrigin = "application/vnd.openxmlformats-package.digital-signature-origin";
const char * const ContentTypes::digitalSignatureXMLSignature = "application/vnd.openxmlformats-package.digital-signaturexmlsignature+xml";

const char * const RelationshipTypes::coreProperties = "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties";
const char * const RelationshipTypes::digitalSignature = "http://schemas.openxmlformats.org/package/2006/relationships/digitalsignature/signature";
const char * const RelationshipTypes::digitalSignatureCertificate = "http://schemas.openxmlformats.org/package/2006/relationships/digitalsignature/certificate";
const char * const RelationshipTypes::digitalSignatureOrigin = "http://schemas.openxmlformats.org/package/2006/relationships/digitalsignature/origin";
const char * const RelationshipTypes::thumbnail = "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail";

const char * const NamespaceIds::contentTypes = "http://schemas.openxmlformats.org/package/2006/content-types";
const char * const NamespaceIds::coreProperties = "http://schemas.openxmlformats.org/package/2006/metadata/core-properties";
const char * const NamespaceIds::digitalSignatures = "http://schemas.openxmlformats.org/package/2006/digital-signature";
const char * const NamespaceIds::relationships = "http://schemas.openxmlformats.org/package/2006/relationships";
const char * const NamespaceIds::markupCompatibility = "http://schemas.openxmlformats.org/markup-compatibility/2006";

const char * const NamespaceIds::xmlSchema = "http://www.w3.org/2001/XMLSchema";
const char * const NamespaceIds::xsi = "http://www.w3.org/2001/XMLSchema-instance";
const char * const NamespaceIds::dc = "http://purl.org/dc/elements/1.1/";
const char * const NamespaceIds::dcterms = "http://purl.org/dc/terms/";
const char * const NamespaceIds::dcmitype = "http://purl.org/dc/dcmitype/";

} // namespace Opc
} // namespace QtOfficeOpenXml
