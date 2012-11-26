#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>

class QMenu;
class QAction;
class QtGui;
class QtIcon;
class QTextEdit;
class QLineEdit;
class QDocWidget;
class QTreeView;
class QFileSystemModel;
class QDirModel;
class QModelIndex;
class QStatusBar;
class QDateTime;
class QMessageBox;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

protected slots:
    void searchFunction();
    void findReplaceFunction();
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void exitFile();
    void cutFile();
    void copyFile();
    void pasteFile();
    void findFile();
    void findReplaceFile();
    void about();

    void openFileAtIndex(const QModelIndex &index);

private:
    void createActions();
    void createMenus();
    void setUpDockWidget();
    void setStatus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QStatusBar *status;
    QMessageBox * msgBox;
    QTextEdit *text;
    QDirModel *model;
    QTreeView *tree;
    QDateTime dateTime;
    QLineEdit *searchString1;
    QLineEdit *searchString2;
    QLineEdit *replaceString;
    QString FilePath;
    QString stringPath;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *findAct;
    QAction *findReplaceAct;
    QAction *aboutAct;

};

#endif // MAINWINDOW_H
