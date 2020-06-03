#ifndef COMMANDS_H
#define COMMANDS_H

#include <QTextCursor>

#include "editor.h"

class Commands {
public:
    static void insertTab(Editor const* editor, QTextCursor cursor);
    static void toggleComment(Editor const* editor);
    static void indent(Editor const* editor);
    static void unindent(Editor const* editor);
};

#endif // COMMANDS_H