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
#include "ooxmlxmlhelper_p.h"
#include <QtOfficeOpenXml/ooxmlschames.h>

#include <QtCore/qdebug.h>

namespace QtOfficeOpenXml {
namespace Ooxml {

XmlHelper::XmlHelper()
{
}

/*!
 * \internal
 * Note: The prefix of the qualifiedAttribute must be the prefix used
 * in Ooxml::Schames for the same namespaceUri.
 */
QHash<QString, QString> XmlHelper::xmlAttributesToHash(const QXmlStreamAttributes &attrs)
{
    QHash<QString, QString> attrsHash;
    foreach (QXmlStreamAttribute attr, attrs) {
        QString qualifiedName = attr.qualifiedName().toString();
        if (!attr.prefix().isEmpty()) {
            //qualifiedAttribute
            if (attr.namespaceUri().isEmpty()) {
                qWarning()<<QString::fromLatin1("XmlHelper Error: the namespace of attribute %1 is empty."
                                                "Broken file or bug of QtOfficeOpenXml")
                            .arg(attr.qualifiedName().toString());
            } else {
                QString prefix = Schames::namespacePrefix(attr.namespaceUri().toString());
                if (prefix.isEmpty()) {
                    qWarning()<<QString::fromLatin1("XmlHelper Error: The prefix of namespace %1 is emtpy."
                                                    "Broken file or bug of QtOfficeOpenXml")
                                .arg(attr.qualifiedName().toString());
                } else if (prefix != attr.prefix()){
                    qualifiedName = QStringLiteral("%1:%2").arg(prefix, attr.name().toString());
                }
            }
        }
        attrsHash.insert(qualifiedName, attr.value().toString());
    }
    return attrsHash;
}

QXmlStreamAttributes XmlHelper::xmlAttributesFromHash(const QHash<QString, QString> &hash, const QStringList &orderedNames)
{
    QXmlStreamAttributes attrs;
    foreach (QString name, orderedNames) {
        if (hash.contains(name))
            attrs.append(name, hash[name]);
    }
    if (hash.size() != attrs.size()) {
        //For debug.
        foreach (QString name, hash.keys()) {
            if (!orderedNames.contains(name))
                qWarning() << QString::fromLatin1("XmlHelper Warning: Unexprected attribute %1 found, "
                                                  "expected attributes are %2")
                              .arg(name).arg(orderedNames.join(QLatin1Char(' ')));
        }
    }
    return attrs;
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
