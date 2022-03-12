QT       += officeopenxml

TARGET = sml_create_document
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/officeopenxml/sml_create_document
INSTALLS += target
