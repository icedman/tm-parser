#include "commands.h"
#include <QDebug>

static void Commands::insertTab(Editor const* editor, QTextCursor cursor)
{
    for(int i=0; i<4; i++) {
        cursor.insertText(" ");
    }
}

static void Commands::removeTab(Editor const* editor, QTextCursor cursor)
{
    QTextBlock block = cursor.block();
    QString s = block.text();
    for(int i=0; i<4 && i<s.length(); i++) {
        if (s[i] == ' ') {
            cursor.deleteChar();
        } else {
            break;
        }
    }
}

static void Commands::toggleComment(Editor const* editor)
{
    // qDebug() << "comment";
    QString singleLineComment = "//";

    QTextCursor cursor = editor->editor->textCursor();
    if (!cursor.hasSelection()) {

        QTextBlock block = cursor.block();
        QString s = block.text();
        int commentPosition = s.indexOf(singleLineComment); 
        int hasComments = commentPosition != -1;

        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::StartOfLine);
        if (!hasComments) {
            cursor.insertText(singleLineComment);
        } else {
            cursor.setPosition(cursor.position() + commentPosition);
            for(int i=0;i<singleLineComment.length();i++) {
                cursor.deleteChar();
            }
        }
        cursor.endEditBlock();
    } else {
        size_t start = cursor.selectionStart();
        QTextCursor cs(cursor);
        cs.setPosition(start);
        cs.movePosition(QTextCursor::StartOfLine);

        QTextBlock block = cs.block();
        QString s = block.text();
        int commentPosition = s.indexOf(singleLineComment); 
        bool hasComments = commentPosition != -1;

        cs.beginEditBlock();
        while(cs.position() <= cursor.selectionEnd()) {
            if (!hasComments) {
                cs.insertText(singleLineComment);
            } else {
                block = cs.block();
                s = block.text();
                commentPosition = s.indexOf(singleLineComment);
                if (commentPosition != -1) {
                    cs.setPosition(cs.position() + commentPosition);
                    for(int i=0;i<singleLineComment.length();i++) {
                        cs.deleteChar();
                    }
                }
            }
            if (!cs.movePosition(QTextCursor::Down)) {
                break;
            }
        }
        cs.endEditBlock();
    }
}

static void Commands::indent(Editor const* editor)
{
    // qDebug() << "indent";
    QTextCursor cursor = editor->editor->textCursor();
    if (!cursor.hasSelection()) {
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::StartOfLine);
        insertTab(editor, cursor);
        cursor.endEditBlock();
    } else {
        size_t start = cursor.selectionStart();
        QTextCursor cs(cursor);
        cs.setPosition(start);
        cs.movePosition(QTextCursor::StartOfLine);
        cs.beginEditBlock();
        while(cs.position() <= cursor.selectionEnd()) {
            insertTab(editor, cs);
            if (!cs.movePosition(QTextCursor::Down)) {
                break;
            }
        }
        cs.endEditBlock();
    }
}

static void Commands::unindent(Editor const* editor)
{
    // qDebug() << "unindent";
    QTextCursor cursor = editor->editor->textCursor();
    if (!cursor.hasSelection()) {
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::StartOfLine);
        removeTab(editor, cursor);
        cursor.endEditBlock();
    } else {
        size_t start = cursor.selectionStart();
        size_t end = cursor.selectionEnd();

        QTextCursor cs(cursor);
        cs.setPosition(start);
        cs.movePosition(QTextCursor::StartOfLine);
        cs.beginEditBlock();
        while(cs.position() <= cursor.selectionEnd()) {
            removeTab(editor, cs);
            if (!cs.movePosition(QTextCursor::Down)) {
                break;
            }
        }
        cs.endEditBlock();
    }
}