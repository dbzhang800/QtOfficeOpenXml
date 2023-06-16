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
#ifndef OPCPACKAGEPART_P_H
#define OPCPACKAGEPART_P_H

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

#include <QtOfficeOpenXml/opcpackagepart.h>
#include <QtCore/qmap.h>

namespace QtOfficeOpenXml {
namespace Opc {
class PackageRelationshipHelper;
class OFFICEOPENXML_AUTOTEST_EXPORT PackagePartPrivate
{
    Q_DECLARE_PUBLIC(PackagePart)

public:
    PackagePartPrivate(const QString &partName, const QString &type, PackagePart *q, Package *package);
    virtual ~PackagePartPrivate();
    void ensureRelationship() const;
    bool isRelationshipPart() const;
    void flushRelationships();

    PackagePart *q_ptr;
    Package *package;
    QString partName;
    QString contentType;
    PackageRelationshipHelper *relationshipHelper;
};

} // namespace Opc
} // namespace QtOfficeOpenXml
#endif // OPCPACKAGEPART_P_H
