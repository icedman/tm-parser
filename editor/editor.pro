QT += widgets
requires(qtConfig(filedialog))

HEADERS         = highlighter.h \
                  mainwindow.h
SOURCES         = highlighter.cpp \
                  mainwindow.cpp \
                  main.cpp

CONFIG += c++17

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/syntaxhighlighter
INSTALLS += target

# parser
INCPATH += ../parser/
LIBS+= ../build/parser/libparser.a

# scopes
INCPATH += ../scopes/
LIBS+= ../build/scopes/libscopes.a

# theme
INCPATH += ../theme/
LIBS+= ../build/theme/libtheme.a

# jsoncpp (build as static library)
INCPATH += ../subprojects/jsoncpp-1.8.4/include 
LIBS+= ../build/subprojects/jsoncpp-1.8.4/libjsoncpp.a

# ongimo
INCPATH += ../Onigmo
LIBS+= ../Onigmo/.libs/libonigmo.a