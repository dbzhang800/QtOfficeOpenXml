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
#ifndef QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H
#define QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H

#include "ooxmlglobal.h"
#include <QString>

namespace QtOfficeOpenXml {
namespace Opc {

enum TargetMode
{
    External,
    Internal
};

class PackageRelationshipPrivate;
class Q_OFFICEOPENXML_EXPORT PackageRelationship
{
    Q_DECLARE_PRIVATE(PackageRelationship)
public:
    PackageRelationship(const QString &id, const QString &type, const QString &source, const QString &target, TargetMode targetMode=Internal);
    ~PackageRelationship();

    QString id() const;
    QString relationshipType() const;
    QString source() const;
    QString target() const;
    TargetMode targetMode() const;

private:
    PackageRelationshipPrivate *d_ptr;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OPC_OPCPACKAGERELATIONSHIP_H
