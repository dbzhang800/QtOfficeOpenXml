QT += widgets officeopenxml

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    binedit.cpp \
    imagewidget.cpp \
    mcexmlwidget.cpp

FORMS += \
    mainwindow.ui \
    mcexmlwidget.ui

HEADERS += \
    mainwindow.h \
    binedit.h \
    imagewidget.h \
    mcexmlwidget.h

target.path = $$[QT_INSTALL_EXAMPLES]/officeopenxml/opc_package_viewer
INSTALLS += target
