
#include <QDebug>

#include "commands.h"
#include "mainwindow.h"

#define NO_IMPLEMENTATION(s) qDebug() << s << " not yet implemented";

static QList<QTextCursor> build_cursors(TextmateEdit *editor) {
    QList<QTextCursor> cursors;
    cursors << editor->extraCursors;
    
    QTextCursor cursor = editor->textCursor();

    bool addCurrentCursor = true;
    for (auto c : cursors) {
        if (c.position() == cursor.position()) {
            addCurrentCursor = false;
            break;
        }
        if (cursor.hasSelection() && cursor.selectionStart() == c.selectionStart() && cursor.selectedText() == c.selectedText()) {
            addCurrentCursor = false;
            break;
        }
    }

    if (addCurrentCursor) {
        cursors << cursor;
    }

    return cursors;
}

size_t count_indent_size(QString s) {
  for (int i = 0; i < s.length(); i++) {
    if (s[i] != ' ' && s[i] != " ") {
        return i;
    }  
  }
  return 0;
}

static void insertTabForCursor(Editor const* editor, QTextCursor cursor)
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

static void Commands::insertTab(Editor const* editor)
{
    QList<QTextCursor> cursors = build_cursors(editor->editor);

    for(auto cursor : cursors) {
        insertTabForCursor(editor, cursor);
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

static void toggleCommentForCursor(Editor const* editor, QTextCursor cursor)
{
    if (!editor->lang->lineComment.length()) {
        return;
    }
    
    QString singleLineComment = editor->lang->lineComment.c_str();
    singleLineComment += " ";

    // QTextCursor cursor = editor->editor->textCursor();
    if (!cursor.hasSelection()) {

        QTextBlock block = cursor.block();
        QString s = block.text();
        int commentPosition = s.indexOf(singleLineComment);
        int hasComments = commentPosition != -1;
        size_t skip = count_indent_size(s);
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::StartOfLine);
        if (!hasComments) {
            cursor.setPosition(cursor.position() + skip);
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
            cs.movePosition(QTextCursor::StartOfLine);
            block = cs.block();
            s = block.text();
            if (!s.isEmpty()) {
                if (!hasComments) {
                    size_t skip = count_indent_size(s);
                    cs.setPosition(cs.position() + skip);
                    cs.insertText(singleLineComment);
                } else {
                    commentPosition = s.indexOf(singleLineComment);
                    if (commentPosition != -1) {
                        cs.setPosition(cs.position() + commentPosition);
                        for (int i = 0; i < singleLineComment.length(); i++) {
                            cs.deleteChar();
                        }
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


static void Commands::toggleComment(Editor const* editor)
{
    QList<QTextCursor> cursors = build_cursors(editor->editor);

    for(auto cursor : cursors) {
        toggleCommentForCursor(editor, cursor);
    }
}

static void toggleBlockCommentForCursor(Editor const* editor, QTextCursor cursor)
{
    if (!editor->lang->blockCommentStart.length()) {
        return;
    }

    NO_IMPLEMENTATION("toggleBlockComment")
}

static void Commands::toggleBlockComment(Editor const* editor)
{
    QList<QTextCursor> cursors = build_cursors(editor->editor);

    for(auto cursor : cursors) {
        toggleBlockCommentForCursor(editor, cursor);
    }
}

static void toggleIndentForCursor(Editor const* editor, QTextCursor cursor)
{
    if (!cursor.hasSelection()) {
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::StartOfLine);
        insertTabForCursor(editor, cursor);
        cursor.endEditBlock();
    } else {
        size_t start = cursor.selectionStart();
        QTextCursor cs(cursor);
        cs.setPosition(start);
        cs.movePosition(QTextCursor::StartOfLine);
        cs.beginEditBlock();
        while (cs.position() <= cursor.selectionEnd()) {
            insertTabForCursor(editor, cs);
            if (!cs.movePosition(QTextCursor::Down)) {
                break;
            }
        }
        cs.endEditBlock();
    }
}

static void Commands::indent(Editor const* editor)
{
    QList<QTextCursor> cursors = build_cursors(editor->editor);

    for(auto cursor : cursors) {
        toggleIndentForCursor(editor, cursor);
    }
}

static void toggleUnindentForCursor(Editor const* editor, QTextCursor cursor)
{
    if (!cursor.hasSelection()) {
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::StartOfLine);
        Commands::removeTab(editor, cursor);
        cursor.endEditBlock();
    } else {
        size_t start = cursor.selectionStart();
        size_t end = cursor.selectionEnd();

        QTextCursor cs(cursor);
        cs.setPosition(start);
        cs.movePosition(QTextCursor::StartOfLine);
        cs.beginEditBlock();
        while (cs.position() <= cursor.selectionEnd()) {
            Commands::removeTab(editor, cs);
            if (!cs.movePosition(QTextCursor::Down)) {
                break;
            }
        }
        cs.endEditBlock();
    }
}

static void Commands::unindent(Editor const* editor)
{
    QList<QTextCursor> cursors = build_cursors(editor->editor);

    for(auto cursor : cursors) {
        toggleUnindentForCursor(editor, cursor);
    }
}

static void Commands::autoIndent(Editor const* editor)
{
    editor_settings_ptr settings = MainWindow::instance()->editor_settings;

    QTextCursor cursor = editor->editor->textCursor();
    size_t white_spaces = count_indent_size(cursor.block().text());

    QTextCursor cs(cursor);
    if (cs.movePosition(QTextCursor::Up)) {
        size_t s = count_indent_size(cs.block().text());
        if (s > white_spaces) {
            white_spaces = s;
        }
    }
    cs = cursor;
    if (cs.movePosition(QTextCursor::Down)) {
        size_t s = count_indent_size(cs.block().text());
        if (s > white_spaces) {
            white_spaces = s;
        }
    }

    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::StartOfLine);
    for(int i=0; i<white_spaces; i++) {
        if (settings->tab_to_spaces) {
            cursor.insertText(" ");
        } else {
            cursor.insertText("\t");
        }
    }
    cursor.endEditBlock();
}

static void Commands::autoClose(Editor const* editor, QString lastKey)
{
    if (!editor->lang->pairs) {
        return;
    }
    
    if (editor->editor->extraCursors.size()) {
        return;
    }

    QTextCursor cursor = editor->editor->textCursor();
    QTextCursor cs(cursor);
    cs.movePosition(QTextCursor::EndOfLine);

    if (cs.position() != cursor.position()) {
        return;
    }

    size_t pos = cursor.position();
    QString line = cursor.block().text();
    if (!lastKey.length()) {
        return;
    }
    int idx = 0;
    for (auto b : editor->lang->pairOpen) {
        if (b.find(lastKey.toStdString()) == std::string::npos) {
            idx++;
            continue;
        }
    
        // qDebug() << "?" << b.c_str();
        // qDebug() << lastKey;

        int pos = line.indexOf(b.c_str());
        if (pos == line.length()-b.length()) {
            QString close = editor->lang->pairClose.at(idx).c_str();
            cursor.beginEditBlock();
            cursor.insertText(close);
            cursor.endEditBlock();
            cursor.setPosition(cursor.position() - close.length());
            editor->editor->setTextCursor(cursor);
            return;
        }

        idx++;
    }
}

static void duplicateLineForCursor(Editor const* editor, QTextCursor cursor)
{
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

static void Commands::duplicateLine(Editor const* editor)
{
    QList<QTextCursor> cursors = build_cursors(editor->editor);

    for(auto cursor : cursors) {
        duplicateLineForCursor(editor, cursor);
    }
}

static QTextCursor expandSelectionToLineForCursor(Editor const* editor, QTextCursor cursor)
{
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::LineUnderCursor);
        return cursor;
        // editor->editor->setTextCursor(cursor);
    } else {
        QTextCursor cs(cursor);
        cs.setPosition(cursor.selectionStart());
        cs.movePosition(QTextCursor::StartOfLine);
        cs.setPosition(cursor.selectionEnd(), QTextCursor::KeepAnchor);
        cs.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        return cs;
        // editor->editor->setTextCursor(cs);
    }
}

static void Commands::expandSelectionToLine(Editor const* editor)
{
    QTextCursor res = expandSelectionToLineForCursor(editor, editor->editor->textCursor());
    editor->editor->setTextCursor(res);

    QList<QTextCursor> cursors;
    for(auto cursor : editor->editor->extraCursors) {
        cursors << expandSelectionToLineForCursor(editor, cursor);
    }

    editor->editor->extraCursors.clear();
    editor->editor->extraCursors << cursors;
}

static bool Commands::find(Editor const* editor, QString string, QString options)
{
    if (string.isEmpty()) {
        return false;
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
                return false;  
            }
        }
        return true;
    }

    QRegExp regx(string);
    if (!editor->editor->find(regx, flags)) {
        QTextCursor cursor = editor->editor->textCursor();
        QTextCursor cs(cursor);
        cs.movePosition(QTextCursor::Start);
        editor->editor->setTextCursor(cs);
        if (!editor->editor->find(regx, flags)) {
            editor->editor->setTextCursor(cursor);
            return false;   
        }
    }

    return true;
}

static bool Commands::keyPressEvent(QKeyEvent* e)
{
    if (e->modifiers() != Qt::NoModifier) {
        QString keys = QKeySequence(e->modifiers() | e->key()).toString();
        return MainWindow::instance()->processKeys(keys.toLower());
    }

    return false;
}