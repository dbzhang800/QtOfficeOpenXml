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
