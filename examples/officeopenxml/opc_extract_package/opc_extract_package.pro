QT += widgets officeopenxml

TARGET = opc_create_package
CONFIG   += console
CONFIG   -= app_bundle

SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/officeopenxml/opc_extract_package
INSTALLS += target
