#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QtGui>
#include <QFileDialog>
#include <QPixmap>
#include <QToolBar>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <QDebug>
#include <QDockWidget>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
    createActions();
    createMenus();
    setUpDockWidget();
    setStatus();

    text = new QTextEdit(this);
    setCentralWidget(text);
}

void MainWindow::createActions()
{
    QToolBar * bar = addToolBar("Action Toolbar");
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setIcon(QIcon(":/icons/document-new.png"));
    newAct->setStatusTip(tr("Create a new file"));
    bar->addAction(newAct);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setIcon(QIcon(":/icons/document-open.png"));
    openAct->setStatusTip(tr("Open a file"));
    bar->addAction(openAct);
    connect(openAct, SIGNAL(triggered()),this , SLOT(openFile()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setIcon(QIcon(":/icons/document-save.png"));
    saveAct->setStatusTip(tr("Save"));
    bar->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()),this , SLOT(saveFile()));

    saveAsAct = new QAction(tr("&SaveAs"), this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save a file"));
    connect(saveAsAct, SIGNAL(triggered()),this , SLOT(saveAsFile()));

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcut(QKeySequence::Close);
    exitAct->setIcon(QIcon(":/icons/window-close.png"));
    exitAct->setStatusTip(tr("Exit a file"));
    bar->addAction(exitAct);
    connect(exitAct, SIGNAL(triggered()),this , SLOT(exitFile()));

    cutAct = new QAction(tr("&Cut"), this);
    cutAct->setShortcut(QKeySequence::Cut);
    cutAct->setIcon(QIcon(":/icons/cut.png"));
    cutAct->setStatusTip(tr("Cut"));
    bar->addAction(cutAct);
    connect(cutAct, SIGNAL(triggered()),this , SLOT(cutFile()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setIcon(QIcon(":/icons/copy.png"));
    copyAct->setStatusTip(tr("Copy"));
    bar->addAction(copyAct);
    connect(copyAct, SIGNAL(triggered()),this , SLOT(copyFile()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcut(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste"));
    pasteAct->setIcon(QIcon(":/icons/paste.png"));
    bar->addAction(pasteAct);
    connect(pasteAct, SIGNAL(triggered()),this , SLOT(pasteFile()));

    findAct = new QAction(tr("&Find All"), this);
    findAct->setShortcut(QKeySequence::Find);
    findAct->setStatusTip(tr("Find"));
    findAct->setIcon(QIcon(":/icons/find.png"));
    bar->addAction(findAct);
    connect(findAct, SIGNAL(triggered()),this , SLOT(findFile()));

    findReplaceAct = new QAction(tr("&Find & Replace All"), this);
    findReplaceAct->setShortcut(QKeySequence::Replace);
    findReplaceAct->setStatusTip(tr("Find and Replace"));
    connect(findReplaceAct, SIGNAL(triggered()),this , SLOT(findReplaceFile()));

    aboutAct = new QAction(tr("&About"),this);
    aboutAct->setStatusTip(tr("About"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(findAct);
    editMenu->addAction(findReplaceAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::setUpDockWidget()
{
    QDockWidget *dock = new QDockWidget(tr("Directory View"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    tree = new QTreeView(dock);
    dock->setWidget(tree);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    model = new QDirModel(dock);
    tree->setModel(model);
    tree->setRootIndex(model->index(QDir::homePath()));
    connect(tree, SIGNAL(clicked(QModelIndex)), this, SLOT(openFileAtIndex(QModelIndex)));
    tree->showColumn(0);
    tree->hideColumn(1);
    tree->hideColumn(2);
    tree->hideColumn(3);
    tree->hideColumn(4);
}
void MainWindow::setStatus()
{
    status = new QStatusBar(this);
    //status->setGeometry(0, 670, 1200, 30);
    dateTime = QDateTime::currentDateTime();
    QString str = dateTime.toString();
    status->showMessage(str);
    this->setStatusBar(status);
}

void MainWindow::openFileAtIndex(const QModelIndex &index)
{
    QString filename = model->fileInfo(index).filePath();
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        text->setPlainText(QString::fromUtf8(file.readAll()));
    }
    status->clearMessage();
    status->showMessage(filename);

}
void MainWindow::newFile()
{
    if (text->document()->isEmpty())
        return ;
    else
    {
        msgBox = new QMessageBox;
        msgBox->setText("Do you want to save changes");
        QPushButton *yesButton = msgBox->addButton(QMessageBox::Yes);
        QPushButton *noButton = msgBox->addButton(QMessageBox::No);
        msgBox->exec();
        if (msgBox->clickedButton() == yesButton) {
            if (stringPath.isEmpty())
            {
                MainWindow::saveAsFile();
                text->document()->clear();
            }
            else
            {
                MainWindow::saveFile();
                text->document()->clear();
                return ;
            }
        } else if (msgBox->clickedButton() == noButton) {
            text->document()->clear();
        }
    }

}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),QDir::homePath());
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) //If file has nothing to read
    {
        text->setPlainText(QString::fromUtf8(file.readAll()));
    }
    status->clearMessage();
    status->showMessage(filename);


}

void MainWindow::saveFile()
{
    QFile file(stringPath);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        file.write(text->toPlainText().toUtf8());
    }
    status->showMessage(stringPath);
}

void MainWindow::saveAsFile()
{
    QString FilePath = QFileDialog::getSaveFileName(this);
    stringPath = FilePath;
    if(stringPath.isEmpty())
        return;
    status->clearMessage();
    status->showMessage(stringPath);
    saveFile();

}

void MainWindow::exitFile()
{
    QMainWindow::close();
}

void MainWindow::cutFile()
{
    text->cut();
}

void MainWindow::copyFile()
{
    text->copy();
}

void MainWindow::pasteFile()
{
    text->paste();
}

void MainWindow::findFile()
{
    QDialog * findDialog = new QDialog(this);
    findDialog->resize(300, 100);
    QLabel *searchLabel1 = new QLabel("Search for : ", findDialog);
    searchString1 = new QLineEdit(findDialog);
    QPushButton *find = new QPushButton("Find All", findDialog);
    QPushButton *close = new QPushButton("Close", findDialog);

    connect(close, SIGNAL(clicked()),findDialog, SLOT(close()));
    connect(find, SIGNAL(clicked()),this, SLOT(searchFunction()));

    QVBoxLayout *vlay = new QVBoxLayout(findDialog);
    QHBoxLayout *h_string = new QHBoxLayout;
    QHBoxLayout *h_button = new QHBoxLayout;

    vlay->addLayout(h_string);
    vlay->addLayout(h_button);

    h_string->addWidget(searchLabel1);
    h_string->addWidget(searchString1);

    QSpacerItem * item = new QSpacerItem(10, 20,QSizePolicy::Expanding, QSizePolicy::Minimum);
    h_button->addSpacerItem(item);
    h_button->addWidget(find);
    h_button->addWidget(close);

    findDialog->exec();

}

void MainWindow::findReplaceFile()
{
    QDialog * findReplaceDialog = new QDialog(this);
    findReplaceDialog->resize(300, 100);
    QLabel *searchLabel2 = new QLabel("Search for : ",findReplaceDialog);
    searchString2 = new QLineEdit(findReplaceDialog);
    QLabel *replaceWith = new QLabel("Replace With : ", findReplaceDialog);
    replaceString = new QLineEdit(findReplaceDialog);
    QPushButton *replace = new QPushButton("Replace All", findReplaceDialog);
    QPushButton *close1 = new QPushButton("Close", findReplaceDialog);
    connect(close1,SIGNAL(clicked()),findReplaceDialog, SLOT(close()));
    connect(replace,SIGNAL(clicked()),this,SLOT(findReplaceFunction()));

    QVBoxLayout *vlay1 = new QVBoxLayout(findReplaceDialog);
    QHBoxLayout *h1_string = new QHBoxLayout;
    QHBoxLayout *h2_string = new QHBoxLayout;
    QHBoxLayout *h1_button = new QHBoxLayout;

    vlay1->addLayout(h1_string);
    vlay1->addLayout(h2_string);
    vlay1->addLayout(h1_button);

    h1_string->addWidget(searchLabel2);
    h1_string->addWidget(searchString2);
    h2_string->addWidget(replaceWith);
    h2_string->addWidget(replaceString);

    QSpacerItem * item1 = new QSpacerItem(10, 20,QSizePolicy::Expanding, QSizePolicy::Minimum);
    h1_button->addSpacerItem(item1);
    h1_button->addWidget(replace);
    h1_button->addWidget(close1);

    findReplaceDialog->exec();
}

void MainWindow::searchFunction()
{
    QString data = searchString1->text();
    QList<QTextEdit::ExtraSelection> extraSelections;

    if(!text->isReadOnly())
    {
        text->moveCursor(QTextCursor::Start);
        QColor color = QColor(Qt::gray).lighter(130);

        while(text->find(data))
        {
            QTextEdit::ExtraSelection extra;
            extra.format.setBackground(color);

            extra.cursor = text->textCursor();
            extraSelections.append(extra);
        }
    }

    text->setExtraSelections(extraSelections);

}

void MainWindow::findReplaceFunction()
{
    QString findString = searchString2->text();
    QString replaceWith = replaceString->text();
    QList<QTextEdit::ExtraSelection> extraSelections1;

    if(!text->isReadOnly())
    {
        text->moveCursor(QTextCursor::Start);
        QColor color1 = QColor(Qt::gray).lighter(130);
        while(text->find(findString))
        {
            QTextEdit::ExtraSelection extra1;
            extra1.format.setBackground(color1);
            extra1.cursor = text->textCursor();
            extra1.cursor.removeSelectedText();
            extra1.cursor.insertText(replaceWith);
            extraSelections1.append(extra1);
        }
    }
    text->setExtraSelections(extraSelections1);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Menu"),
                       tr("A simple Notepad Qt Application"));
}
