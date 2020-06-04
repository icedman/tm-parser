#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QLineEdit;
class QLabel;

class Panel : public QWidget {
    Q_OBJECT
    
public:
    Panel(QWidget* parent = 0);

public:
    QVBoxLayout* vbox(QBoxLayout *layout);
    QHBoxLayout* hbox(QBoxLayout *layout);
    QPushButton* addButton(QString text, QBoxLayout *layout = 0);
    QLineEdit* addInputText(QString value, QBoxLayout *layout = 0);
    QLabel* addLabel(QString text, QBoxLayout *layout = 0);

private:
    QVBoxLayout *layout;
};

#endif // PANEL_H