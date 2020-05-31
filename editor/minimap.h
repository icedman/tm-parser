#ifndef MINIMAP_H
#define MINIMAP_H

#include <QWidget>

class Editor;

class MiniMap : public QWidget {
    Q_OBJECT

public:
    MiniMap(QWidget* parent = 0);

    QColor backgroundColor;

    Editor* editor;

    void setSizes(size_t firstVisible, int visible, size_t val, size_t max);

    size_t value;
    size_t maximum;
    size_t firstVisible;
    int visibleLines;

    float offsetY;

    QPixmap buffer;

private:
    void scrollByMouseY(float y);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // MINIMAP_H