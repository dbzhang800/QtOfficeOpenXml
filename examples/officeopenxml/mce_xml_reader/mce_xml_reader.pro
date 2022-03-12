
QT += widgets officeopenxml

TARGET = mce_xml_reader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

target.path = $$[QT_INSTALL_EXAMPLES]/officeopenxml/mce_xml_reader
INSTALLS += target
