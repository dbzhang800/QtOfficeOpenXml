
QT += widgets officeopenxml

TARGET = opc_packages_diff
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

target.path = $$[QT_INSTALL_EXAMPLES]/officeopenxml/opc_packages_diff
INSTALLS += target
