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
#ifndef OPCPACKAGE_P_H
#define OPCPACKAGE_P_H

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

#include <QtOfficeOpenXml/opcpackage.h>
#include <QtCore/qmap.h>

namespace QtOfficeOpenXml {
namespace Opc {
class PackageRelationshipHelper;
class OFFICEOPENXML_AUTOTEST_EXPORT PackagePrivate
{
    Q_DECLARE_PUBLIC(Package)

public:
    PackagePrivate(const QString &packageName, QIODevice *device, Package *q);

    virtual ~PackagePrivate();
    void ensureRelationship() const;

    Package *q_ptr;

    QString fileName;
    QIODevice *device;
    QIODevice::OpenMode mode;
    QMap<QString, PackagePart *> parts;
    PackageRelationshipHelper *relationshipHelper;
    PackageProperties *packageProperties;
};

} // namespace Opc
} // namespace QtOfficeOpenXml
#endif // OPCPACKAGE_P_H
