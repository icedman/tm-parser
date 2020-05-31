#ifndef TREE_H
#define TREE_H

#include <QModelIndex>
#include <QTreeView>

class MainWindow;
class QFileSystemModel;

class Sidebar : public QTreeView {
    Q_OBJECT
public:
    Sidebar(QWidget* parent = 0);

    void setRootPath(QString path);
    void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;

    MainWindow* mainWindow;
    QFileSystemModel* fileModel;

    bool firstOpen;
};

#endif // TREE_H