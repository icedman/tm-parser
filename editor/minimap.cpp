#include "minimap.h"
#include "editor.h"

#include <QtWidgets>
#include <QTextDocument>
#include <QTextBlock>

MiniMap::MiniMap(QWidget* parent)
    : QWidget(parent)
{
}

void MiniMap::paintEvent(QPaintEvent *event)
{
    // QPainter p(this);
    // p.fillRect(event->rect(), backgroundColor);

    // QTextDocument *doc = editor->document();
    // QTextBlock block = doc->begin();
    // while(block.isValid()) {
    //     HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
    //     if (blockData) {
    //         // int y = 0 = block.firstLineNumber();
    //         // p.drawLine()
    //     }
    //     block = block.next();
    // }
}