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

#include "opcpackagepart.h"
#include <QMap>

namespace QtOfficeOpenXml {
namespace Opc {

class OFFICEOPENXML_AUTOTEST_EXPORT PackagePartPrivate
{
    Q_DECLARE_PUBLIC(PackagePart)

public:
    PackagePartPrivate(const QString &partName, const QString type, PackagePart *q, Package *package);
    virtual ~PackagePartPrivate();

    PackagePart *q_ptr;
    Package *package;
    QString partName;
    QString contentType;
    QMap<QString, PackageRelationship *> relationships;
};

} // namespace Opc
} // namespace QtOfficeOpenXml
#endif // OPCPACKAGEPART_P_H
