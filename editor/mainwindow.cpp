/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include <iostream>

#include "mainwindow.h"
#include "reader.h"

//! [0]
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    updateTimer(this)
{
    setupFileMenu();
    setupHelpMenu();
    setupEditor();

    setCentralWidget(editor);
    setWindowTitle(tr("Syntax Highlighter"));
    setMinimumSize(1200, 800);

    // connect(&updateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
}
//! [0]

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
        tr("<p>The <b>Syntax Highlighter</b> example shows how "
           "to perform simple syntax highlighting by subclassing "
           "the QSyntaxHighlighter class and describing "
           "highlighting rules using regular expressions.</p>"));
}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString& path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C, C++ Files (*.c *.cpp *.h)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            highlighter->setDeferRendering(true);
            editor->setPlainText(file.readAll());
            onUpdate();
        }
    }
}

//! [1]
void MainWindow::setupEditor()
{
    QFont font;
    //'Droid Sans Mono', 'monospace', monospace, 'Droid Sans Fallback'
    font.setFamily("Droid Sans Mono");
    // font.setFamily("Source Code Pro");

    font.setFixedPitch(true);
    font.setPointSize(12);

    editor = new QSublimeTextEdit;
    editor->setFont(font);

    // Json::Value json_theme = parse::loadJson("./light_vs.json");
    Json::Value json_theme = parse::loadJson("./dracula.json");
    theme = parse_theme(json_theme);

    //------------------
    // editor theme
    //------------------
    if (!theme->global_style.background.is_blank()) {
        QPalette p = editor->palette();
        p.setColor(QPalette::Active, QPalette::Base,
            QColor(theme->global_style.background.red * 255,
                theme->global_style.background.green * 255,
                theme->global_style.background.blue * 255, 255));
        p.setColor(QPalette::Inactive, QPalette::Base,
            QColor(theme->global_style.background.red * 255,
                theme->global_style.background.green * 255,
                theme->global_style.background.blue * 255, 255));
        editor->setPalette(p);
    }

    if (!theme->global_style.foreground.is_blank()) {
        QTextCharFormat fmt;
        fmt.setForeground(QBrush(QColor(theme->global_style.foreground.red * 255,
            theme->global_style.foreground.green * 255,
            theme->global_style.foreground.blue * 255, 255)));
        editor->mergeCurrentCharFormat(fmt);
    }

    // setup highlighter
    highlighter = new Highlighter(editor->document());
    highlighter->setTheme(theme);

    // openFile("../tests/cases/sqlite3.c");
    // openFile("../tests/cases/tinywl.c");
    openFile("../tests/cases/test.c");
}
//! [1]

void MainWindow::setupFileMenu()
{
    QMenu* fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this,
        &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction(
        tr("&Open..."),
        this, [this]() { openFile(); }, QKeySequence::Open);
    fileMenu->addAction(tr("E&xit"), qApp,
        &QApplication::quit, QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu* helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::higlightAllBlocks()
{

}

void MainWindow::onUpdate()
{
    QTextDocument *doc = editor->document();
    int lines = doc->lineCount();

    int rendered = 0;
    for(int i=0;i<lines && rendered<200; i++) {
        QTextBlock block = doc->findBlockByLineNumber(i);
        HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
        if (!blockData) {
            rendered++;
            blockData = new HighlightBlockData;
            block.setUserData(blockData);
            highlighter->rehighlightBlock(block);
        }
    }

    if (rendered > 0) {
        updateTimer.singleShot(5, this, SLOT(onUpdate()));
    } else {
        std::cout << "all rendering done" << std::endl;
        highlighter->setDeferRendering(false);
    }
}