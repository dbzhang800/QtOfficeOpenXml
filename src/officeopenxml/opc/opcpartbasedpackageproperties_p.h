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
#ifndef OPCPARTBASEDPACKAGEPROPERTIES_H
#define OPCPARTBASEDPACKAGEPROPERTIES_H

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

#include <QtOfficeOpenXml/opcpackageproperties.h>
#include <QtCore/qmap.h>

class QIODevice;

namespace QtOfficeOpenXml {
namespace Opc {
class Package;
class PackagePart;
class PartBasedPackageProperties : public PackageProperties
{
public:
    PartBasedPackageProperties(Package *package);
    ~PartBasedPackageProperties();
    void flush() Q_DECL_OVERRIDE;

    //Summary information properties
    QString title() const Q_DECL_OVERRIDE;
    void setTitle(const QString &title) Q_DECL_OVERRIDE;
    QString subject() const Q_DECL_OVERRIDE;
    void setSubject(const QString &subject) Q_DECL_OVERRIDE;
    QString creator() const Q_DECL_OVERRIDE;
    void setCreator(const QString &creator) Q_DECL_OVERRIDE;
    QString keywords()const Q_DECL_OVERRIDE;
    void setKeywords(const QString &keywords) Q_DECL_OVERRIDE;
    QString description()const Q_DECL_OVERRIDE;
    void setDescription(const QString &description) Q_DECL_OVERRIDE;
    QString lastModifiedBy() const Q_DECL_OVERRIDE;
    void setLastModifiedBy(const QString &user) Q_DECL_OVERRIDE;
    QString revision()const Q_DECL_OVERRIDE;
    void setRevision(const QString &revision) Q_DECL_OVERRIDE;
    QDateTime lastPrinted() const Q_DECL_OVERRIDE;
    void setLastPrinted(const QDateTime &lastPrinted) Q_DECL_OVERRIDE;
    QDateTime created() const Q_DECL_OVERRIDE;
    void setCreated(const QDateTime &created) Q_DECL_OVERRIDE;
    QDateTime modified() const Q_DECL_OVERRIDE;
    void setModified(const QDateTime &modified) Q_DECL_OVERRIDE;

    //Document summary information
    QString category() const Q_DECL_OVERRIDE;
    void setCategory(const QString &category) Q_DECL_OVERRIDE;
    QString identifier() const Q_DECL_OVERRIDE;
    void setIdentifier(const QString &identifier) Q_DECL_OVERRIDE;
    QString contentType() const Q_DECL_OVERRIDE;
    void setContentType(const QString &contentType) Q_DECL_OVERRIDE;
    QString language() const Q_DECL_OVERRIDE;
    void setLanguage(const QString &language) Q_DECL_OVERRIDE;
    QString version() const Q_DECL_OVERRIDE;
    void setVersion(const QString &version) Q_DECL_OVERRIDE;
    QString contentStatus() const Q_DECL_OVERRIDE;
    void setContentStatus(const QString &contentStatus) Q_DECL_OVERRIDE;

private:
    enum PropertyEnum {
        PE_Creator,
        PE_Identifier,
        PE_Title,
        PE_Subject,
        PE_Description,
        PE_Language,
        PE_Created,
        PE_Modified,
        PE_ContentType,
        PE_Keywords,
        PE_Category,
        PE_Version,
        PE_LastModifiedBy,
        PE_ContentStatus,
        PE_Revision,
        PE_LastPrinted
    };

    QString stringProperty(PropertyEnum pe) const;
    QDateTime dateTimeProperty(PropertyEnum pe) const;
    void setStringProperty(PropertyEnum pe, const QString &value);
    void setDateTimeProperty(PropertyEnum pe, const QDateTime &value);

    void readPropertiesFromPackage();
    void doLoadFromXml(QIODevice *device);
    void doSaveToXml(QIODevice *device);

    Package *m_package;
    PackagePart *m_propertyPart;
    QMap<PropertyEnum, QString> m_properties;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // OPCPARTBASEDPACKAGEPROPERTIES_H
