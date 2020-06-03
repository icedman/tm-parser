#include "commands.h"
#include <QDebug>

static void Commands::insertTab(Editor const* editor, QTextCursor cursor)
{
    for(int i=0; i<4; i++) {
        cursor.insertText(" ");
    }
}

static void Commands::toggleComment(Editor const* editor)
{
    qDebug() << "comment";
}

static void Commands::indent(Editor const* editor)
{
    qDebug() << "indent";
    QTextCursor cursor = editor->editor->textCursor();
    cursor.beginEditBlock();
    size_t pos = cursor.position() - cursor.positionInBlock();
    cursor.setPosition(pos);
    insertTab(editor, cursor);
    cursor.endEditBlock();
}

static void Commands::unindent(Editor const* editor)
{
    qDebug() << "unindent";
    QTextCursor cursor = editor->editor->textCursor();
    cursor.beginEditBlock();
    size_t pos = cursor.position() - cursor.positionInBlock();
    cursor.setPosition(pos);

    QTextBlock block = cursor.block();
    QString s = block.text();
    for(int i=0; i<4 && i<s.length(); i++) {
        if (s[i] == ' ') {
            cursor.deleteChar();
        }
    }

    cursor.endEditBlock();
}