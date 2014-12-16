
QT       += testlib officeopenxml
QT       -= gui

CONFIG += testcase

TARGET = tst_zippackagetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_zippackagetest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
