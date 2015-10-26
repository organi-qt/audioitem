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

headers.files = $$HEADERS
headers.path = $$[QT_INSTALL_HEADERS]/AudioItem
INSTALLS += headers

target.path = $$[QT_INSTALL_PLUGINS]/../lib
INSTALLS    += target

# Objects Pathes
DESTDIR = bin
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
OBJECTS_DIR = build
