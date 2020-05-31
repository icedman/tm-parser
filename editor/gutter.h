#ifndef GUTTER_H
#define GUTTER_H

#include <QWidget>

struct BlockInfo {
    int position;
    int number;
    bool foldable : 1;
    bool folded : 1;
};

class Editor;

class Gutter : public QWidget {
    Q_OBJECT

public:
    Gutter(QWidget* parent = 0);

    QVector<BlockInfo> lineNumbers;
    QColor lineNumberColor;
    QColor backgroundColor;
    QFont font;

    Editor* editor;

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // GUTTERH