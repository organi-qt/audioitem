INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/audioitem.h

PRIVATE_HEADERS += \
    $$PWD/mpgdecoder.h

SOURCES += \
    $$PWD/audioitem.cpp \
    $$PWD/mpgdecoder.cpp

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
