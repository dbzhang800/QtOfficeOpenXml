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
