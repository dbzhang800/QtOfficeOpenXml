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
#ifndef QTOFFICEOPENXML_SML_SMLWORKBOOK_P_H
#define QTOFFICEOPENXML_SML_SMLWORKBOOK_P_H

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

#include <QtOfficeOpenXml/smlglobal.h>
#include <QtCore/qhash.h>
#include <QtCore/qlist.h>

namespace QtOfficeOpenXml {
namespace Sml {

class DefinedName
{
public:
    DefinedName() {}
    DefinedName(const QHash<QString, QString> &attrs, const QString &formula) :
        attrs_raw(attrs), formula(formula)
    {}

    //name, comment, customMenu, description, help, statusBar, localSheetId,
    //hidden, function, vbProcedure, xlm, functionGroupId, shortcutKey,
    //publishToServer, workbookParameter
    QHash<QString, QString> attrs_raw;
    QString formula;
};

class Workbook
{
public:
    Workbook();

    QString bookView(const QString &attribute) const;
    void setBookView(const QString &attribute, const QString &val);
    int activeTab() const;
    void setActiveTab(int index);

    //appName, lastEdited, lowestEdited, rupBuild, codeName
    QHash<QString, QString> fileVersion_raw;

    //readOnlyRecommended, userName, reservationPassword
    QHash<QString, QString> fileSharing_raw;

    //date1904, showObjects, showBorderUnselectedTables, filterPrivacy,
    //promptedSolutions, showInkAnnotation, backupFile, saveExternalLinkValues,
    //updateLinks, codeName, hidePivotFieldList, showPivotChartFilter,
    //allowRefreshQuery, publishItems, checkCompatibility, autoCompressPictures,
    //refreshAllConnections, defaultThemeVersion
    QHash<QString, QString> workbookPr_raw;

    //workbookPassword, revisionsPassword, lockStructure, lockWindows, lockRevision
    QHash<QString, QString> workbookProtection_raw;

    //xWindow, yWindow, windowWidth, windowHeight,
    //visibility, minimized, showHorizontalScroll, showVerticalScroll,
    //showSheetTabs, tabRatio, firstSheet, activeTab, autoFilterDateGrouping,
    QList<QHash<QString, QString> > bookViews_raw;

    QList<DefinedName> definedNames;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_SMLWORKBOOK_P_H
