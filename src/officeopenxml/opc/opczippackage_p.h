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
#ifndef OOXMLZIPPACKAGE_P_H
#define OOXMLZIPPACKAGE_P_H

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

#include "opczippackage.h"
#include "opcpackage_p.h"

class KZip;

namespace QtOfficeOpenXml {
namespace Opc {
class OFFICEOPENXML_AUTOTEST_EXPORT ZipPackagePrivate : public PackagePrivate
{
    Q_DECLARE_PUBLIC(ZipPackage)

public:
    ZipPackagePrivate(const QString &packageName, QIODevice *device, ZipPackage *q);
    virtual ~ZipPackagePrivate();

    bool doLoadPackage();
    bool doSavePackage();

    KZip *zipArchive;
};

class ContentTypeHelper
{
public:
    ContentTypeHelper();
    void loadFromStream(QIODevice *device);
    void saveToStream(QIODevice *device);

    QString contentType(const QString &partName);
    void addContentType(const QString &partName, const QString &contentType);

private:
    struct DefaultData {
        DefaultData() {}
        DefaultData(const QString &ext, const QString &type)
            :extension(ext), contentType(type) {}
        QString extension;
        QString contentType;
    };
    struct OverrideData {
        OverrideData() {}
        OverrideData(const QString &name, const QString &type)
            :partName(name), contentType(type) {}
        QString partName;
        QString contentType;
    };

    QMap<QString, DefaultData> defaults;
    QMap<QString, OverrideData> overrides;
};

} // namespace Opc
} // namespace QtOfficeOpenXml
#endif // OOXMLZIPPACKAGE_P_H
