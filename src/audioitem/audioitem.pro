QT = core

include($$PWD/audioitem-lib.pri)

load(qt_build_config)

TARGET = AudioItem

load(qt_module)

LIBS += -lasound -lmpg123
