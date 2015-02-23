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

#include <QtOfficeOpenXml/officeopenxmlglobal.h>

class QString;
class QStringList;

namespace QtOfficeOpenXml {
namespace Opc {

#ifndef Q_QDOC
QStringList splitPartName(const QString &partName);
QString getAbsolutePartName(const QString &baseName, const QString &relativeName);
QString getRelativePartName(const QString &baseName, const QString &absoluteName);
QString getRelsPath(const QString &partName);
#endif

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
