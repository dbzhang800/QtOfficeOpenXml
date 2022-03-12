QT += widgets officeopenxml

TARGET = opc_package_xml_edit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    recentfiles.cpp

HEADERS  += mainwindow.h \
    recentfiles.h

FORMS    += mainwindow.ui

target.path = $$[QT_INSTALL_EXAMPLES]/officeopenxml/opc_package_xml_edit
INSTALLS += target
