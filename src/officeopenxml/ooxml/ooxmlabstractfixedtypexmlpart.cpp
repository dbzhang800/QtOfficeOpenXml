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

namespace QtOfficeOpenXml {
namespace Ooxml {

/*!
 * \internal
 * \class QtOfficeOpenXml::Ooxml::AbstractFixedTypeXmlPart
 *
 * Interface class for all Known type xml parts in the opc package.
 */

AbstractFixedTypeXmlPart::AbstractFixedTypeXmlPart()
{
}

AbstractFixedTypeXmlPart::~AbstractFixedTypeXmlPart()
{
}

QString AbstractFixedTypeXmlPart::partName() const
{
    return m_partName;
}

QString AbstractFixedTypeXmlPart::contentType() const
{
    return m_contentType;
}

bool AbstractFixedTypeXmlPart::loadFromPackagePart(Opc::PackagePart *part)
{
    QIODevice *device = part->getDevice();
    bool ret = doLoadFromXml(device);
    part->releaseDevice();
    return ret;
}

bool AbstractFixedTypeXmlPart::saveToPackagePart(Opc::PackagePart *part, SchameType schameType) const
{
    QIODevice *device = part->getDevice();
    bool ret = doSaveToXml(device, schameType);
    part->releaseDevice();
    return ret;
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
