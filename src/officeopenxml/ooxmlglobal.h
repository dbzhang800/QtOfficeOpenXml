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
#ifndef OFFICEOPENXMLGLOBAL_H
#define OFFICEOPENXMLGLOBAL_H
#include <QtGlobal>

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
