#include <QDebug>
#include <QDir>

#include <iostream>

#include "mainwindow.h"
#include "sidebar.h"

FileSystemModel::FileSystemModel(QObject* parent)
    : QFileSystemModel(parent)
{
    connect(this, SIGNAL(directoryLoaded(const QString&)), this, SLOT(onDirectoryLoaded(const QString&)));
}

void FileSystemModel::onDirectoryLoaded(const QString& path)
{
    QModelIndex dirIndex = index(path);

#if 0
    int rows = rowCount(dirIndex);
    for(int i=0;i<rows; i++) {
        QModelIndex rowIndex = index(i, 0, dirIndex);
        QVariant rowData = data(rowIndex);
        QString fileName = rowData.toString();
        QFileInfo info(fileName);
        QString suffix = info.suffix();
        // preload icons here (this being a separate thread?) << crashes!!!?
        // image_from_icon(icons, fileName, suffix);
    }
#endif

    emit dataChanged(dirIndex, QModelIndex());
}

QVariant FileSystemModel::data(const QModelIndex& index, int role) const
{

    if (role == Qt::DecorationRole) {
        QFileInfo info = FileSystemModel::fileInfo(index);
        if (info.isFile()) {
            // if (info.suffix() == "cpp") {
            QString fileName;
            QString suffix = info.suffix();
            return image_from_icon(mainWindow->icons, fileName, suffix, mainWindow->extensions);
            // }
        }
    }

    return QFileSystemModel::data(index, role);
}

Sidebar::Sidebar(QWidget* parent)
    : QTreeView(parent)
    , fileModel(0)
{
    setHeaderHidden(true);
    connect(&timer,SIGNAL(timeout()),this,SLOT(onSingleClick()));
}

void Sidebar::setRootPath(QString path)
{
    if (fileModel) {
        return;
    }

    Sidebar* sidebar = (Sidebar*)parent();
    MainWindow* main = (MainWindow*)sidebar->parent();

    fileModel = new FileSystemModel(this);
    fileModel->mainWindow = main;
    fileModel->setRootPath(path);

    setModel(fileModel);

    for (int i = 1; i < fileModel->columnCount(); i++) {
        hideColumn(i);
    }

    QModelIndex idx = fileModel->index(fileModel->rootPath());
    setRootIndex(idx);
}

void Sidebar::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    MainWindow* main = mainWindow;
    if (!main->settings.isObject()) {
        return;
    }

    std::cout << "ignoring.." << std::endl;

    Json::Value file_exclude_patterns = main->settings["file_exclude_patterns"];
    // std::cout << main->settings << std::endl;

    int rows = fileModel->rowCount(topLeft);
    for (int i = 0; i < rows; i++) {
        QModelIndex rowIndex = fileModel->index(i, 0, topLeft);
        QVariant rowData = fileModel->data(rowIndex);
        QString fileName = rowData.toString();
        QFileInfo info(fileName);

        QString _suffix = "*." + info.suffix();

        if (file_exclude_patterns.isArray()) {
            for (int j = 0; j < file_exclude_patterns.size(); j++) {
                QString pat = file_exclude_patterns[j].asString().c_str();
                if (_suffix == pat) {
                    // qDebug() << fileName;
                    setRowHidden(i, topLeft, true);
                    break;
                }
            }
        }
    }
}

void Sidebar::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // QModelIndexList i = selected.indexes();
    // if (i.size()) {
    //     QString filePath = fileModel->filePath(i[0]);
    //     mainWindow->openFile(filePath);
    // }
}

void Sidebar::mouseDoubleClickEvent(QMouseEvent * event)
{
  Q_UNUSED(event);
  // qDebug() << "This happens on double click";

  // open
  timer.stop();

  QTreeView::mouseDoubleClickEvent(event);
}


void Sidebar::mousePressEvent(QMouseEvent * event)
{
  Q_UNUSED(event);
  timer.start(50);

  QTreeView::mousePressEvent(event);
}

void Sidebar::onSingleClick()
{
  // qDebug() << "This happens on single click";

    QModelIndex index = currentIndex();
    if (index.isValid()) {
        QString filePath = fileModel->filePath(index);
        mainWindow->openFile(filePath);
    }

  timer.stop();
}