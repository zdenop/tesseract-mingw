QT          =

TARGET      = ApiExample
TEMPLATE    = app

OBJECTS_DIR += temp
MOC_DIR     += temp
UI_DIR      += temp
RCC_DIR     += temp

SOURCES += ApiExample.cpp
LIBS    += -llept -ltesseract

win32: {
    CONFIG  += console
    DESTDIR = ./bin
    INCLUDEPATH += ./include/
    LIBS += -lws2_32 -L./lib
    DEFINES += WINDOWS __BLOB_T_DEFINED
}
