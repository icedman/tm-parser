#include <QTextBlock>
#include <QTextDocument>
#include <QtWidgets>

#include "editor.h"
#include "minimap.h"

MiniMap::MiniMap(QWidget* parent)
    : QWidget(parent)
{
}

void MiniMap::paintEvent(QPaintEvent* event)
{
    // todo render only changed!
    QPainter p(this);
    p.fillRect(event->rect(), backgroundColor);
    // p.setRenderHint(QPainter::Antialiasing);
    p.save();
    p.setOpacity(0.5);
    p.scale(0.75, 1.0);
    p.setBrush(Qt::SolidPattern);
    QTextDocument* doc = editor->document();
    QTextBlock block = doc->begin();
    while (block.isValid()) {
        HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
        if (blockData) {
            int y = block.firstLineNumber() * 2;
            for (auto span : blockData->spans) {
                int x = span.start;
                int w = span.length;
                p.setPen(QColor(span.red, span.green, span.blue));
                p.drawLine(x, y, x + w, y);
            }
        }
        block = block.next();
    }

    p.restore();
}