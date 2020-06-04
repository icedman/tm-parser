#ifndef COMMANDS_H
#define COMMANDS_H

#include <QTextCursor>
#include <QKeyEvent>

#include "editor.h"

class Commands {
public:
    static void insertTab(Editor const* editor, QTextCursor cursor);
    static void removeTab(Editor const* editor, QTextCursor cursor);
    static void toggleComment(Editor const* editor);
    static void toggleBlockComment(Editor const* editor);
    static void indent(Editor const* editor);
    static void unindent(Editor const* editor);
    static void duplicateLine(Editor const* editor);
    static void expandSelectionToLine(Editor const* editor);

    static bool keyPressEvent(QKeyEvent* e);
};

#endif // COMMANDS_H
