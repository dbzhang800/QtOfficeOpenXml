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

#include <QtOfficeOpenXml/opczippackage.h>
#include <private/opcpackage_p.h>

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
    bool dirty;
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
