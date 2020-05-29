#include "gutter.h"

#include <QtWidgets>

Gutter::Gutter(QWidget* parent)
    : QWidget(parent)
{
}

void Gutter::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(event->rect(), backgroundColor);
    p.setPen(lineNumberColor);
    p.setFont(font);
    int fh = QFontMetrics(font).height();
    int fw = QFontMetrics(font).width('w');
    foreach (BlockInfo ln, lineNumbers) {
        p.drawText(0, ln.position, width() - 4 - (fw*1), fh, Qt::AlignRight, QString::number(ln.number));
    }
}