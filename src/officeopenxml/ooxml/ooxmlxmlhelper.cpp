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
