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
#ifndef QTOFFICEOPENXML_SML_ABSTRACTSHEET_P_H
#define QTOFFICEOPENXML_SML_ABSTRACTSHEET_P_H

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

#include <QtOfficeOpenXml/smlabstractsheet.h>

#include <QtCore/qhash.h>

namespace QtOfficeOpenXml {
namespace Sml {

class SheetPr
{
public:
    SheetPr(){}

    bool isEmpty() const {
        return attrs_raw.isEmpty() && tabColor_raw.isEmpty()
                && outlinePr_raw.isEmpty() && pageSetUpPr_raw.isEmpty();
    }

    //syncHorizontal, syncVertical, syncRef, transitionEvaluation, transitionEntry,
    //published, codeName, filterMode, enableFormatConditionsCalculation
    //Note, Chartsheet has only "published" and "codeName"
    QHash<QString, QString> attrs_raw;

    //auto, indexed, rgb, theme, tint
    QHash<QString, QString> tabColor_raw;

    //applyStyles, summaryBelow, summaryRight, showOutlineSymbols
    //Note, Chartsheet doesn't have this element.
    QHash<QString, QString> outlinePr_raw;

    //autoPageBreaks, fitToPage
    //Note, Chartsheet doesn't have this element.
    QHash<QString, QString> pageSetUpPr_raw;
};

class SheetView
{
public:
    SheetView(){}

    //Worksheet:
    //  windowProtection, showFormulas, showGridLines, showRowColHeaders, showZeros,
    //  rightToLeft, tabSelected, showRuler, showOutlineSymbols, defaultGridColor,
    //  showWhiteSpace, view, topLeftCell, colorId, zoomScale, zoomScaleNormal,
    //  zoomScaleSheetLayoutView, zoomScalePageLayoutView, workbookViewId,
    //Chartsheet:
    //  tabSelected, zoomScale, workbookViewId, zoomToFit
    QHash<QString, QString> attrs_raw;

    //xSplit, ySplit, topLeftCell, activePane, state
    QHash<QString, QString> pane_raw;
};

class AbstractSheetPrivate
{
    Q_DECLARE_PUBLIC(AbstractSheet)
public:
    AbstractSheetPrivate(const QString &name, int id, SheetState state, AbstractSheet *q);
    virtual ~AbstractSheetPrivate();

    //QSharedPointer<Drawing> drawing;

    QString name;
    int id;
    SheetState sheetState;

    SheetPr sheetPr;
    QList<SheetView> sheetViews;

    AbstractSheet *q_ptr;
};

} // namespace Sml
} // namespace QtOfficeOpenXml

#endif // QTOFFICEOPENXML_SML_ABSTRACTSHEET_P_H
