
#include <QDebug>

#include "commands.h"
#include "mainwindow.h"

#define NO_IMPLEMENTATION(s) qDebug() << s << " not yet implemented";

static void Commands::insertTab(Editor const* editor, QTextCursor cursor)
{
    editor_settings_ptr settings = MainWindow::instance()->editor_settings;
    if (settings->tab_to_spaces) {
        for (int i = 0; i < settings->tab_size; i++) {
            cursor.insertText(" ");
        }
    } else {
        cursor.insertText("\t");
    }
}

static void Commands::removeTab(Editor const* editor, QTextCursor cursor)
{
    editor_settings_ptr settings = MainWindow::instance()->editor_settings;
    QTextBlock block = cursor.block();
    QString s = block.text();
    for (int i = 0; i < settings->tab_size && i < s.length(); i++) {
        if (settings->tab_to_spaces) {
            if (s[i] == '\t') {
                cursor.deleteChar();
                break;
            }   
        }
        if (s[i] == ' ' || s[i] == '\t') {
            cursor.deleteChar();
        } else {
            break;
        }
    }
}

static void Commands::toggleComment(Editor const* editor)
{
    if (!editor->lang->lineComment.length()) {
        return;
    }
    
    // qDebug() << "comment";
    QString singleLineComment = editor->lang->lineComment.c_str();

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
            for (int i = 0; i < singleLineComment.length(); i++) {
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
        while (cs.position() <= cursor.selectionEnd()) {
            if (!hasComments) {
                cs.insertText(singleLineComment);
            } else {
                block = cs.block();
                s = block.text();
                commentPosition = s.indexOf(singleLineComment);
                if (commentPosition != -1) {
                    cs.setPosition(cs.position() + commentPosition);
                    for (int i = 0; i < singleLineComment.length(); i++) {
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

static void Commands::toggleBlockComment(Editor const* editor)
{
    if (!editor->lang->blockCommentStart.length()) {
        return;
    }

    NO_IMPLEMENTATION("toggleBlockComment")
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
        while (cs.position() <= cursor.selectionEnd()) {
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
        while (cs.position() <= cursor.selectionEnd()) {
            removeTab(editor, cs);
            if (!cs.movePosition(QTextCursor::Down)) {
                break;
            }
        }
        cs.endEditBlock();
    }
}


static void Commands::duplicateLine(Editor const* editor)
{
    QTextCursor cursor = editor->editor->textCursor();
    if (!cursor.hasSelection()) {
        QTextCursor cs(cursor);
        cs.select(QTextCursor::LineUnderCursor);
        QString selectedText = cs.selectedText();
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.insertText(QString("\n") + selectedText);
        cursor.endEditBlock();
    } else {
        QString selectedText = cursor.selectedText();
        QTextCursor cs(cursor);
        cs.setPosition(cursor.selectionStart());
        cs.beginEditBlock();
        cs.insertText(selectedText);
        cs.endEditBlock();
    }
}

static void Commands::expandSelectionToLine(Editor const* editor)
{
    QTextCursor cursor = editor->editor->textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::LineUnderCursor);
        editor->editor->setTextCursor(cursor);
    } else {
        QTextCursor cs(cursor);
        cs.setPosition(cursor.selectionStart());
        cs.movePosition(QTextCursor::StartOfLine);
        cs.setPosition(cursor.selectionEnd(), QTextCursor::KeepAnchor);
        cs.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        editor->editor->setTextCursor(cs);
    }
}

static void Commands::find(Editor const* editor, QString string, QString options)
{
    if (string.isEmpty()) {
        return;

    }

    bool regex = options.indexOf("regular_") != -1;
    int flags = 0;
    if (options.indexOf("case_") != -1) {
        flags = QTextDocument::FindCaseSensitively;
    }
    if (options.indexOf("whole_") != -1) {
        flags |= QTextDocument::FindWholeWords;
    }

    if (!regex) {
        if (!editor->editor->find(string, flags)) {
            QTextCursor cursor = editor->editor->textCursor();
            QTextCursor cs(cursor);
            cs.movePosition(QTextCursor::Start);
            editor->editor->setTextCursor(cs);
            if (!editor->editor->find(string, flags)) {
                editor->editor->setTextCursor(cursor);     
            }
        }
        return;
    }

    QRegExp regx(string);
    if (!editor->editor->find(regx, flags)) {
        QTextCursor cursor = editor->editor->textCursor();
        QTextCursor cs(cursor);
        cs.movePosition(QTextCursor::Start);
        editor->editor->setTextCursor(cs);
        if (!editor->editor->find(regx, flags)) {
            editor->editor->setTextCursor(cursor);     
        }
    }
}

static bool Commands::keyPressEvent(QKeyEvent* e)
{
    if (e->modifiers() != Qt::NoModifier) {
        QString keys = QKeySequence(e->modifiers() | e->key()).toString();
        return MainWindow::instance()->processKeys(keys.toLower());
    }

    return false;
}