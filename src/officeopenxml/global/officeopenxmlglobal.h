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
#ifndef OFFICEOPENXMLGLOBAL_H
#define OFFICEOPENXMLGLOBAL_H
#include <QtCore/qglobal.h>

#if !defined(QT_STATIC)
#  if defined(QT_BUILD_OFFICEOPENXML_LIB)
#    define Q_OFFICEOPENXML_EXPORT Q_DECL_EXPORT
#  else
#    define Q_OFFICEOPENXML_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_OFFICEOPENXML_EXPORT
#endif

#ifdef OFFICEOPENXML_TEST
#  define OFFICEOPENXML_AUTOTEST_EXPORT Q_OFFICEOPENXML_EXPORT
#else
#  define OFFICEOPENXML_AUTOTEST_EXPORT
#endif

#endif // OFFICEOPENXMLGLOBAL_H
