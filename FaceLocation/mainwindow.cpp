#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Align.h"

using namespace Gdiplus; 

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    this->image = new QImage();

//    createActions();
//    createMenus();
 //   createContextMenu();

//    setCurrentFile("");
}

MainWindow::~MainWindow()
{
	delete image;  
	delete ui; 
}

void MainWindow::on_openAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(image->load(fileName))
        {
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(*image));
            ui->graphicsView->setScene(scene);
            ui->graphicsView->resize(image->width() + 10, image->height() + 10);
            ui->graphicsView->show();
        }
    }
}

void MainWindow::on_alignAction_triggered()
{
	procPic(strFilePath);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        event->accept();
    } else {
        event->ignore();
    }
}


//void MainWindow::open()
//{
//    if (okToContinue()) {
//        QString fileName = QFileDialog::getOpenFileName(this,
//                                   tr("Open Spreadsheet"), ".",
//                                   tr("Spreadsheet files (*.sp)"));
//        if (!fileName.isEmpty())
//            loadFile(fileName);
//    }
//}

//void MainWindow::createActions()
//{
//    //openAction = new QAction(tr("&Open..."), this);
//    //openAction->setIcon(QIcon(":/images/open.png"));
//    //openAction->setShortcut(QKeySequence::Open);
//    //openAction->setStatusTip(tr("Open an existing spreadsheet file"));
//    //connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
//
//    saveAction = new QAction(tr("&Save"), this);
//    saveAction->setIcon(QIcon(":/images/save.png"));
//    saveAction->setShortcut(QKeySequence::Save);
//    saveAction->setStatusTip(tr("Save the spreadsheet to disk"));
//    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
//
//    //exitAction = new QAction(tr("E&xit"), this);
//    //exitAction->setShortcut(tr("Ctrl+Q"));
//    //exitAction->setStatusTip(tr("Exit the application"));
//    //connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
//}

//void MainWindow::createMenus()
//{
//    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(openAction);
//    fileMenu->addAction(saveAction);
//    separatorAction = fileMenu->addSeparator();
//    fileMenu->addAction(exitAction);
//
//}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Spreadsheet"),
                        tr("The document has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
//            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

//bool MainWindow::loadFile(const QString &fileName)
//{
//    if (!spreadsheet->readFile(fileName)) {
//        statusBar()->showMessage(tr("Loading canceled"), 2000);
//        return false;
//    }
//
//    setCurrentFile(fileName);
//    statusBar()->showMessage(tr("File loaded"), 2000);
//    return true;
//}
//
//bool MainWindow::saveFile(const QString &fileName)
//{
//    if (!spreadsheet->writeFile(fileName)) {
//        statusBar()->showMessage(tr("Saving canceled"), 2000);
//        return false;
//    }
//
//    setCurrentFile(fileName);
//    statusBar()->showMessage(tr("File saved"), 2000);
//    return true;
//}
//
//void MainWindow::setCurrentFile(const QString &fileName)
//{
//    curFile = fileName;
//    setWindowModified(false);
//
//    QString shownName = tr("Untitled");
//    if (!curFile.isEmpty()) {
//        shownName = strippedName(curFile);
//        recentFiles.removeAll(curFile);
//        recentFiles.prepend(curFile);
//        updateRecentFileActions();
//    }
//
//    setWindowTitle(tr("%1[*] - %2").arg(shownName)
//                                   .arg(tr("Spreadsheet")));
//}