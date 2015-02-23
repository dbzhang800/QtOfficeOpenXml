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
#ifndef OOXMLSCHAMES_P_H
#define OOXMLSCHAMES_P_H

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

#include <QtOfficeOpenXml/ooxmlschames.h>
#include <QtCore/qhash.h>
#include <QtCore/qsharedpointer.h>

namespace QtOfficeOpenXml {
namespace Ooxml {

struct OoxmlSchamesData
{
    OoxmlSchamesData(int id, const QString transitional, const QString &strict=QString(), const QString &prefix=QString()) :
        id(id), prefix(prefix), strict(strict), transitional(transitional)
    {}

    int id;
    QString prefix;
    QString strict;
    QString transitional;
};

class SchamesPrivate
{
public:
    SchamesPrivate();
    ~SchamesPrivate();

    void addNamespace(int id, const QString &transitional, const QString &strict=QString(), const QString &prefix=QString());
    void addRelationship(int id, const QString &transitional, const QString &strict=QString());

    QHash<int, QSharedPointer<OoxmlSchamesData> > namespaceSchameHash;
    QHash<QString, QSharedPointer<OoxmlSchamesData> > namespaceSchameHash_strict;
    QHash<QString, QSharedPointer<OoxmlSchamesData> > namespaceSchameHash_transitional;
    QHash<QString, QSharedPointer<OoxmlSchamesData> > namespaceSchameHash_normal;

    QHash<int, QSharedPointer<OoxmlSchamesData> > relationshipSchameHash;
    QHash<QString, QSharedPointer<OoxmlSchamesData> > relationshipSchameHash_strict;
    QHash<QString, QSharedPointer<OoxmlSchamesData> > relationshipSchameHash_transitional;
    QHash<QString, QSharedPointer<OoxmlSchamesData> > relationshipSchameHash_normal;
};

Q_GLOBAL_STATIC(QtOfficeOpenXml::Ooxml::SchamesPrivate, schamesPrivateInstance)

} // namespace Ooxml
} // namespace QtOfficeOpenXml
#endif // OOXMLSCHAMES_P_H
