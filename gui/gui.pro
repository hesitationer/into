TEMPLATE        = lib
TARGET          = piigui
HEADERS         = *.h
SOURCES         = *.cc

INTODIR = ..
include($$INTODIR/base.pri)
VERSION         = $$INTO_LIB_VERSION

qt5: QT += widgets

FORMS += ui/*.ui
RESOURCES += resources.qrc
DEFINES += PII_BUILD_GUI
LIBS += -lpiicore$$LIBVER -lpiiydin$$LIBVER
# PENDING ydin is needed only because of PiiClassInfoRegisterer
# Resource database should be moved to core.