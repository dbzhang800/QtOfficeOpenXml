QT       += officeopenxml

TARGET = opc_create_package
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/officeopenxml/opc_create_package
INSTALLS += target
