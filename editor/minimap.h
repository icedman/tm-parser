#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>

class SublimeTextEdit;

class MiniMap : public QWidget {
    Q_OBJECT

public:
    MiniMap(QWidget* parent = 0);

    QColor backgroundColor;
    SublimeTextEdit* editor;

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // MINIMAP_H