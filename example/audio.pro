# QT Modules
QT += qml quick

# Target
TARGET = audio
TEMPLATE = app
LIBS += -laudioitem

# Include Path
HEADERS += audio.h

SOURCES += \
    main.cpp \
    audio.cpp

OTHER_FILES += \
    main.qml

RESOURCES += \
    audio.qrc

# Objects Pathes
DESTDIR = bin
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
OBJECTS_DIR = build
