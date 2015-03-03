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
#include "ooxmlabstractfixedtypexmlpart_p.h"
#include "opcpackagepart.h"
#include "opcpackage.h"

namespace QtOfficeOpenXml {
namespace Ooxml {

/*!
 * \internal
 * \class QtOfficeOpenXml::Ooxml::AbstractFixedTypeXmlPart
 *
 * Interface class for all Known type xml parts in the opc package.
 */

AbstractFixedTypeXmlPart::AbstractFixedTypeXmlPart(const QString &partName, Opc::Package *package) :
    partUri(partName), package(package), part(0), hasCalled(false)
{
}

AbstractFixedTypeXmlPart::~AbstractFixedTypeXmlPart()
{
}

QString AbstractFixedTypeXmlPart::partName() const
{
    return partUri;
}

Opc::PackagePart *AbstractFixedTypeXmlPart::packagePart() const
{
    if (!part) {
        if (package->mode() == QIODevice::WriteOnly)
            const_cast<AbstractFixedTypeXmlPart *>(this)->part = package->createPart(partUri, contentType());
        else
            const_cast<AbstractFixedTypeXmlPart *>(this)->part = package->part(partUri);
    }
    return part;
}

bool AbstractFixedTypeXmlPart::loadFromPackage(SchameType schameType)
{
    if (hasCalled || !packagePart())
        return false;
    QIODevice *device = part->getDevice(QIODevice::ReadOnly);
    bool ret = doLoadFromXml(device, schameType);
    part->releaseDevice();
    hasCalled = true;
    return ret;
}

bool AbstractFixedTypeXmlPart::saveToPackage(SchameType schameType)
{
    if (hasCalled || !packagePart())
        return false;
    QIODevice *device = part->getDevice(QIODevice::WriteOnly);
    bool ret = doSaveToXml(device, schameType);
    part->releaseDevice();
    hasCalled = true;
    return ret;
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
