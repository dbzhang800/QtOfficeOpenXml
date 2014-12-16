TARGET = QtOfficeOpenXml

#QMAKE_DOCS = $$PWD/doc/qtofficeopenxml.qdocconf

load(qt_module)

CONFIG += build_officeopenxml_lib

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/ooxmlglobal.h
include($$PWD/opc/opc.pri)

#Define this macro if you want to run tests, so more AIPs will get exported.
DEFINES += OFFICEOPENXML_TEST

QMAKE_TARGET_COMPANY = "Debao Zhang"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2014 Debao Zhang <hello@debao.me>"
QMAKE_TARGET_DESCRIPTION = "ooxml library for Qt5"

