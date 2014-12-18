include(../../3rdParty/karchive/karchive.pri)

HEADERS += \
    $$PWD/opcpackage.h \
    $$PWD/opcpackage_p.h \
    $$PWD/opczippackage.h \
    $$PWD/opczippackage_p.h \
    $$PWD/opcpackagepart.h \
    $$PWD/opcpackagerelationship.h \
    $$PWD/opcpackageproperties.h \
    $$PWD/opcpackagepart_p.h \
    $$PWD/opczippackagepart.h \
    $$PWD/opczippackagepart_p.h \
    $$PWD/opcpackagerelationship_p.h \
    $$PWD/opcutils_p.h \
    $$PWD/opcpartbasedpackageproperties_p.h

SOURCES += \
    $$PWD/opcpackage.cpp \
    $$PWD/opczippackage.cpp \
    $$PWD/opcpackagepart.cpp \
    $$PWD/opcpackagerelationship.cpp \
    $$PWD/opcpackageproperties.cpp \
    $$PWD/opczippackagepart.cpp \
    $$PWD/opcutils.cpp \
    $$PWD/opcpartbasedpackageproperties.cpp
