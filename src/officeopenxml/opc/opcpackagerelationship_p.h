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
#ifndef OPCPACKAGERELATIONSHIP_P_H
#define OPCPACKAGERELATIONSHIP_P_H

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

#include "opcpackagerelationship.h"
#include <QMap>

class QIODevice;

namespace QtOfficeOpenXml {
namespace Opc {
class PackageRelationshipPrivate
{
public:
    PackageRelationshipPrivate(const QString &id, const QString &type, const QString &source, const QString &target, TargetMode targetMode)
        :id(id), relationshipType(type), source(source), target(target), targetMode(targetMode)
    {
    }

    QString id;
    QString relationshipType;
    QString source;
    QString target;
    TargetMode targetMode;
};

class PackageRelationshipHelper
{
public:
    static QMap<QString, PackageRelationship*> loadRelationshipFromStream(QIODevice *device, const QString &partName);
    static void saveRelationshipToStream(QIODevice *device, const QList<PackageRelationship*> &relationshipList);
};


} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // OPCPACKAGERELATIONSHIP_P_H
