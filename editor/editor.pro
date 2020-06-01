QT += widgets
requires(qtConfig(filedialog))

HEADERS         = editor.h \
                  extension.h \
                  gutter.h \
                  highlighter.h \
                  icons.h \
                  mainwindow.h \
                  minimap.h \
                  settings.h \
                  sidebar.h

SOURCES         = editor.cpp \
                  extension.cpp \
                  gutter.cpp \
                  highlighter.cpp \
                  icons.cpp \
                  mainwindow.cpp \
                  minimap.cpp \
                  settings.cpp \
                  sidebar.cpp \
                  main.cpp

QMAKE_CXXFLAGS += -fpermissive
CONFIG += c++17

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/syntaxhighlighter
INSTALLS += target

INCPATH += ../textmate/parser/
INCPATH += ../textmate/scopes/
INCPATH += ../textmate/theme/

LIBS+= ../build/textmate/libtextmate.a


# jsoncpp (build as static library)
INCPATH += ../subprojects/jsoncpp-1.8.4/include 
LIBS+= ../build/subprojects/jsoncpp-1.8.4/libjsoncpp.a

# ongimo
INCPATH += ../Onigmo
LIBS+= ../Onigmo/.libs/libonigmo.a