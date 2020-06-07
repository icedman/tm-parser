QT += widgets qml svg
requires(qtConfig(filedialog))

HEADERS         = commands.h \
                  editor.h \
                  extension.h \
                  gutter.h \
                  highlighter.h \
                  icons.h \
                  js.h \
                  mainwindow.h \
                  minimap.h \
                  panel.h \
                  sidebar.h \
                  settings.h \
                  tabs.h

SOURCES         = commands.cpp \
                  editor.cpp \
                  extension.cpp \
                  gutter.cpp \
                  highlighter.cpp \
                  icons.cpp \
                  js.cpp \
                  mainwindow.cpp \
                  minimap.cpp \
                  panel.cpp \
                  sidebar.cpp \
                  settings.cpp \
                  tabs.cpp \
                  main.cpp \
                  ../textmate/parser/grammar.cpp \
                  ../textmate/parser/reader.cpp \
                  ../textmate/parser/pattern.cpp \
                  ../textmate/parser/parser.cpp \
                  ../textmate/scopes/scope.cpp \
                  ../textmate/scopes/types.cpp \
                  ../textmate/scopes/parse.cpp \
                  ../textmate/scopes/match.cpp \
                  ../textmate/theme/theme.cpp \
                  ../textmate/theme/util.cpp

QMAKE_CXXFLAGS += -fpermissive
CONFIG += c++17
#CONFIG += c++14

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/syntaxhighlighter
INSTALLS += target

INCPATH += ../textmate/parser/
INCPATH += ../textmate/scopes/
INCPATH += ../textmate/theme/

# LIBS+= ../build/textmate/libtextmate.a

# jsoncpp (build as static library)
INCPATH += ../subprojects/jsoncpp-1.8.4/include 
LIBS+= ../build/subprojects/jsoncpp-1.8.4/libjsoncpp.a

# ongimo
INCPATH += ../Onigmo
LIBS+= ../Onigmo/.libs/libonigmo.a