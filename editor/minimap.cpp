#include <QTextBlock>
#include <QTextDocument>
#include <QtWidgets>

#include <iostream>

#include "editor.h"
#include "minimap.h"

MiniMap::MiniMap(QWidget* parent)
    : QWidget(parent)
{
}

void MiniMap::paintEvent(QPaintEvent* event)
{
    QTextDocument* doc = editor->document();
    int lines = doc->lineCount() + 1;

    float advanceY = 2.0;
    float scaleX = 0.75;
    
    offsetY = 0;

    float currentHeight = height();

    // calculate
    if (maximum > 0) {
        float visibleHeight = visibleLines * advanceY;
        float totalHeight = lines * advanceY;
        if (totalHeight > currentHeight) {
            // compute offset
            float scrollSpace = totalHeight - (visibleHeight * 8);
            if (scrollSpace > 0) {
                offsetY = -2 + (scrollSpace * value / maximum);
            }
        }
    }
    
    // todo render only changed!
    QPainter p(this);

    QColor bg = backgroundColor.darker(105);
    QColor bgLighter = backgroundColor.lighter(120);

    p.fillRect(event->rect(), bg);
    p.setRenderHint(QPainter::Antialiasing);

    QTextBlock firstVisibleBlock = doc->findBlockByNumber(firstVisible);
    int n = firstVisibleBlock.firstLineNumber();
    int y = n * advanceY;
    int vh = (visibleLines + 2) * advanceY;

    int renderLines = (currentHeight * 4) / advanceY;

    // std::cout << vh << std::endl;
    p.fillRect(0, y - offsetY, width(), vh, bgLighter);

    int fv = firstVisible - 500;
    if (fv < 0) {
        fv = 0;
    }

    p.save();
    p.setOpacity(0.5);
    p.scale(scaleX, 1.0);
    p.setBrush(Qt::SolidPattern);

    int idx = 0;
    QTextBlock block = doc->findBlockByNumber(fv);
    while (block.isValid()) {
        HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
        if (blockData) {
            n = block.firstLineNumber();
            y = n * advanceY;
            for (auto span : blockData->spans) {
                int x = span.start;
                int w = span.length;
                p.setPen(QColor(span.red, span.green, span.blue));
                p.drawLine(x, y - offsetY, x + w, y - offsetY);
            }
        }
        idx++;
        if (idx > renderLines) {
            break;
        }
        block = block.next();
    }

    p.restore();
}

void MiniMap::setSizes(size_t first, int visible, size_t val, size_t max)
{
    firstVisible = first;
    visibleLines = visible;
    value = val;
    maximum = max;
}

void MiniMap::mouseMoveEvent(QMouseEvent *event) {
    QPointF pos = event->localPos();    
    scrollByMouseY(pos.y());
}

void MiniMap::mousePressEvent(QMouseEvent *event) {
    QPointF pos = event->localPos();    
    scrollByMouseY(pos.y());
}

void MiniMap::scrollByMouseY(float y)
{
    float advanceY = 2.0;
    float totalHeight = visibleLines * advanceY;

    float lineY = (y - 20) / advanceY;
    lineY += (offsetY / advanceY);
    if (lineY < 0) {
        lineY = 0;
    }
    editor->verticalScrollBar()->setValue((int)lineY);
}
