TEMPLATE = lib
LANGUAGE = c++
CONFIG += qt warn_off thread exceptions
TARGET = formula

DEFINES += FORMULA_DLL
INCLUDEPATH += \
    ../include \

HEADERS       = \
    hformulaexport.h \
    hformuladlg.h \
    hformulapi.h \
    hformulaex.h

SOURCES       = \
    hformuladlg.cpp \
    hformula.cpp \
    hformulaex.cpp

RESOURCES     = \
    styleqss.qrc
    

FORMS += \
    formula.ui \
    #dialog.ui

unix{

    #LIBS += -L ../lib/ -lH5IconGui
}

win32{
   LIBS += \
   #      ../lib/H5IconGui.lib
}
QT += widgets



