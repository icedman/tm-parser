#include <QDebug>
#include <QtWidgets>

#include "panel.h"

Panel::Panel(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(0, 80);
    layout = new QVBoxLayout(this);
    setLayout(layout);
}

QHBoxLayout* Panel::hbox(QBoxLayout *parent)
{
    if (!parent) {
        parent = layout;
    }
    QHBoxLayout* hbox = new QHBoxLayout();
    parent->addLayout(hbox);
    return hbox;
}

QVBoxLayout* Panel::vbox(QBoxLayout *parent)
{
    if (!parent) {
        parent = layout;
    }
    QVBoxLayout* vbox = new QVBoxLayout();
    parent->addLayout(vbox);
    return vbox;
}

QPushButton* Panel::addButton(QString text, QBoxLayout *parent)
{
    if (!parent) {
        parent = layout;
    }
    QPushButton *widget = new QPushButton();
    widget->setText(text);
    parent->addWidget(widget);
    return widget;
}

QLineEdit* Panel::addInputText(QString text, QBoxLayout *parent)
{
    if (!parent) {
        parent = layout;
    }
    QLineEdit *widget = new QLineEdit();
    widget->setText(text);
    parent->addWidget(widget);
    return widget;
}

QLabel* Panel::addLabel(QString text, QBoxLayout *parent)
{
    if (!parent) {
        parent = layout;
    }
    QLabel *widget = new QLabel();
    widget->setText(text);
    parent->addWidget(widget);
    return widget;
}
