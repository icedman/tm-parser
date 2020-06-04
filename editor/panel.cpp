#include <QDebug>
#include <QQmlComponent>
#include <QBoxLayout>
#include <QPushButton>

#include "panel.h"

Panel::Panel(QWidget* parent)
    : QWidget(parent)
{
    // setMinimumSize(0, 80);
    // setMaximumSize(0, 600);
    setMinimumSize(200,100);

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
    QPushButton *btn = new QPushButton();
    btn->setText(text);
    parent->addWidget(btn);
    return btn;
}