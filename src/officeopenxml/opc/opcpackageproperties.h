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
#ifndef QTOFFICEOPENXML_OPC_OPCPACKAGEPROPERTIES_H
#define QTOFFICEOPENXML_OPC_OPCPACKAGEPROPERTIES_H

#include "ooxmlglobal.h"

class QString;
class QDateTime;

namespace QtOfficeOpenXml {
namespace Opc {

class Q_OFFICEOPENXML_EXPORT PackageProperties
{
public:
    virtual ~PackageProperties(){}
    virtual void flush() = 0;

    //Summary information properties
    virtual QString title() const = 0;
    virtual void setTitle(const QString &title) = 0;
    virtual QString subject() const = 0;
    virtual void setSubject(const QString &subject) = 0;
    virtual QString creator() const = 0;
    virtual void setCreator(const QString &creator) = 0;
    virtual QString keywords()const = 0;
    virtual void setKeywords(const QString &keywords) = 0;
    virtual QString description()const = 0;
    virtual void setDescription(const QString &description) = 0;
    virtual QString lastModifiedBy() const = 0;
    virtual void setLastModifiedBy(const QString &user) = 0;
    virtual QString revision()const = 0;
    virtual void setRevision(const QString &revision) = 0;
    virtual QDateTime lastPrinted() const = 0;
    virtual void setLastPrinted(const QDateTime &lastPrinted) = 0;
    virtual QDateTime created() const = 0;
    virtual void setCreated(const QDateTime &created) = 0;
    virtual QDateTime modified() const = 0;
    virtual void setModified(const QDateTime &modified) = 0;

    //Document summary information
    virtual QString category() const = 0;
    virtual void setCategory(const QString &category) = 0;
    virtual QString identifier() const = 0;
    virtual void setIdentifier(const QString &identifier) = 0;
    virtual QString contentType() const = 0;
    virtual void setContentType(const QString &contentType) = 0;
    virtual QString language() const = 0;
    virtual void setLanguage(const QString &language) = 0;
    virtual QString version() const = 0;
    virtual void setVersion(const QString &version) = 0;
    virtual QString contentStatus() const = 0;
    virtual void setContentStatus(const QString &contentStatus) = 0;
};

} // namespace Opc
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_OPC_OPCPACKAGEPROPERTIES_H
