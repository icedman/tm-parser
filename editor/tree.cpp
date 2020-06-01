#include <QDebug>
#include <QDir>
#include <QFileSystemModel>

#include <iostream>

#include "mainwindow.h"
#include "tree.h"

Sidebar::Sidebar(QWidget* parent)
    : QTreeView(parent)
    , fileModel(0)
    , firstOpen(false)
{
    setHeaderHidden(true);
}

void Sidebar::setRootPath(QString path)
{
    if (fileModel) {
        return;
    }

    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(path);

    setModel(fileModel);
    for (int i = 1; i < fileModel->columnCount(); i++) {
        hideColumn(i);
    }

    QModelIndex idx = fileModel->index(fileModel->rootPath());
    setRootIndex(idx);
}

void Sidebar::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    if (!firstOpen) {
        firstOpen = true;
        return;
    }

    if (current.isValid() && previous.isValid()) {
        QString filePath = fileModel->filePath(current);
        mainWindow->openFile(filePath);
    }
}

/*
QVariant MyQFileSystemModel::data( const QModelIndex& index, int role ) const {
if( role == Qt::DecorationRole )
{
    QFileInfo info = MyQFileSystemModel::fileInfo(index);

    if(info.isFile())
    {
        if(info.suffix() == "dat")
            return QPixmap(":/icons/File_Icon.png");//I pick the icon depending on the extension
        else if(info.suffix() == "mcr")
            return QPixmap(":/icons/Region_Icon.png");
    }
}
return QFileSystemModel::data(index, role);
}
*/
