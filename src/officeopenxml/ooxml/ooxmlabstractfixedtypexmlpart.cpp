/****************************************************************************
**
** Copyright (C) 2014-2015 Debao Zhang <hello@debao.me>
**
** This library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Library General Public License version
** 2.1 or version 3 as published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public License
** along with this library; see the file LICENSE.LGPLv21 and LICENSE.LGPLv3
** included in the packaging of this file. If not, write to the Free
** Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
** 02110-1301, USA.
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
    partUri(partName), package(package), part(0)
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

bool AbstractFixedTypeXmlPart::loadFromPackage()
{
    if (!packagePart())
        return false;
    QIODevice *device = part->getDevice();
    bool ret = doLoadFromXml(device);
    part->releaseDevice();
    return ret;
}

bool AbstractFixedTypeXmlPart::saveToPackage(SchameType schameType) const
{
    if (!packagePart())
        return false;
    QIODevice *device = part->getDevice();
    bool ret = doSaveToXml(device, schameType);
    part->releaseDevice();
    return ret;
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
