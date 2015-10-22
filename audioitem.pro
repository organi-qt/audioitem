# Target
TEMPLATE = lib
CONFIG += plugin
TARGET = audioitem
LIBS += -lasound -lmpg123

# Include Path
INCLUDEPATH += src
HEADERS += \
    src/audioitem.h \
    src/mpgdecoder.h

SOURCES += \
    src/audioitem.cpp \
    src/mpgdecoder.cpp

# Objects Pathes
DESTDIR = bin
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
OBJECTS_DIR = build
