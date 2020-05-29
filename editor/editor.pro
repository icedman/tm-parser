QT += widgets
requires(qtConfig(filedialog))

HEADERS         = mainwindow.h \
                  editor.h \
                  highlighter.h \
                  gutter.h \
                  minimap.h \
                  extension.h

SOURCES         = mainwindow.cpp \
                  editor.cpp \
                  highlighter.cpp \
                  gutter.cpp \
                  minimap.cpp \
                  extension.cpp \
                  main.cpp

CONFIG += c++17

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/syntaxhighlighter
INSTALLS += target

# parser
INCPATH += ../textmate/parser/
LIBS+= ../build/textmate/parser/libparser.a

# scopes
INCPATH += ../textmate/scopes/
LIBS+= ../build/textmate/scopes/libscopes.a

# theme
INCPATH += ../textmate/theme/
LIBS+= ../build/textmate/theme/libtheme.a

# jsoncpp (build as static library)
INCPATH += ../subprojects/jsoncpp-1.8.4/include 
LIBS+= ../build/subprojects/jsoncpp-1.8.4/libjsoncpp.a

# ongimo
INCPATH += ../Onigmo
LIBS+= ../Onigmo/.libs/libonigmo.a