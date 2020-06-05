#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QtWidgets>

#include <iostream>

#include "editor.h"
#include "minimap.h"
// #include "mainwindow.h"
// #include "settings.h"

MiniMap::MiniMap(QWidget* parent)
    : QWidget(parent)
{
}

static int renderOneLine(QPainter& p, QTextBlock& block, int offsetY, float advanceY)
{
    if (!block.isValid()) {
        return -1;
    }

    HighlightBlockData* blockData = reinterpret_cast<HighlightBlockData*>(block.userData());
    if (blockData) {
        int n = block.firstLineNumber();
        int y = n * advanceY;
        for (auto span : blockData->spans) {
            int x = span.start;
            int w = span.length;
            p.setPen(QColor(span.red, span.green, span.blue));
            p.drawLine(x, y - offsetY, x + w, y - offsetY);
        }

        return y - offsetY;
    }

    return -1;
}

void MiniMap::paintEvent(QPaintEvent* event)
{
    float advanceY = 2.0;

    if (editor->highlighter->isDirty() || buffer.height() != height()) {
        buffer = QPixmap();
    }

    if (buffer.width() > 0) {
        QPainter pt(this);
        pt.drawPixmap(rect(), buffer, buffer.rect());
        QTextCursor cursor = editor->editor->textCursor();
        QTextBlock block = cursor.block();
        renderOneLine(pt, block, offsetY, advanceY);
        return;
    }

    // std::cout << "minimap generate" << std::endl;

    QTextDocument* doc = editor->editor->document();
    int lines = doc->lineCount() + 1;
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

    buffer = QPixmap(width(), height());
    QPainter p(&buffer);

    // bool isDark = theme_is_dark(MainWindow::instance()->theme);
    QColor bg = backgroundColor.darker(105);
    QColor bgLighter = backgroundColor.lighter(120);
    // if (!isDark) {
    //     bg = backgroundColor.lighter(105);
    //     bgLighter = backgroundColor.darker(120);
    // }

    p.fillRect(event->rect(), bg);
    p.setRenderHint(QPainter::Antialiasing);

    QTextBlock firstVisibleBlock = doc->findBlockByNumber(firstVisible);
    int n = firstVisibleBlock.firstLineNumber();
    int y = n * advanceY;
    int vh = (visibleLines + 2) * advanceY;

    int renderLines = (currentHeight * 4) / advanceY;

    // highlighted block
    p.fillRect(0, y - offsetY - (advanceY * 2), width(), vh, bgLighter);

    // int start = n - (offsetY / advanceY) - 4;

    int start = (offsetY - currentHeight) / advanceY;
    if (start < 0) {
        start = 0;
    }

    p.save();
    p.setOpacity(0.5);
    p.scale(scaleX, 1.0);
    p.setBrush(Qt::SolidPattern);

    int idx = 0;
    QTextBlock block = doc->findBlockByNumber(start);
    while (block.isValid()) {
        int y = renderOneLine(p, block, offsetY, advanceY);
        if (y - offsetY > height()) {
            break;
        }
        idx++;
        block = block.next();
    }

    p.restore();

    QPainter pt(this);
    pt.drawPixmap(rect(), buffer, buffer.rect());
}

void MiniMap::setSizes(size_t first, int visible, size_t val, size_t max)
{
    if (firstVisible != first || visibleLines != visible || value != val || maximum != max) {
        buffer = QPixmap();
    }

    firstVisible = first;
    visibleLines = visible;
    value = val;
    maximum = max;
}

void MiniMap::mouseMoveEvent(QMouseEvent* event)
{
    QPointF pos = event->localPos();
    scrollByMouseY(pos.y());
}

void MiniMap::mousePressEvent(QMouseEvent* event)
{
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

    editor->editor->verticalScrollBar()->setValue((int)lineY);
}
