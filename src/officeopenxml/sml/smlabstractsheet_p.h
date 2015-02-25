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
#include <QtCore/qstring.h>

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
