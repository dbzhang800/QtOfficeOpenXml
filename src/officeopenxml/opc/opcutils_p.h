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
#ifndef QTOFFICEOPENXML_OPC_OPCUTILS_H
#define QTOFFICEOPENXML_OPC_OPCUTILS_H

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

#include "ooxmlglobal.h"

class QString;
class QStringList;

namespace QtOfficeOpenXml {
namespace Opc {

QStringList splitPartName(const QString &partName);
QString getAbsolutePartName(const QString &baseName, const QString &relativeName);
QString getRelativePartName(const QString &baseName, const QString &absoluteName);
QString getRelsPath(const QString &partName);

class ContentTypes
{
public:
    static const char * const coreProperties;
    static const char * const digitalSignatureCertificate;
    static const char * const digitalSignatureOrigin;
    static const char * const digitalSignatureXMLSignature;
    static const char * const relationships;
};

class RelationshipTypes
{
public:
    static const char * const coreProperties;
    static const char * const digitalSignature;
    static const char * const digitalSignatureCertificate;
    static const char * const digitalSignatureOrigin;
    static const char * const thumbnail;
};

class NamespaceIds
{
public:
    static const char * const contentTypes;
    static const char * const coreProperties;
    static const char * const digitalSignatures;
    static const char * const relationships;
    static const char * const markupCompatibility;

    static const char * const xmlSchema;
    static const char * const xsi;
    static const char * const dc;
    static const char * const dcterms;
    static const char * const dcmitype;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OPC_OPCUTILS_H
