#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "faceAlign.h"

using namespace Gdiplus; 

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    this->image = new QImage();
	this->scene = new QGraphicsScene(parent);
	ui->graphicsView->setScene(scene);
	//ui->graphicsView->resize(image->width(), image->height());
	ui->graphicsView->show();
    setCurrentFile("");
}

MainWindow::~MainWindow()
{
	delete image;
	delete imgItem;
	delete scene;
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
		setCurrentFile(fileName);
        if(image->load(fileName))
        {
			imgItem = new QGraphicsPixmapItem();
			imgItem->setPixmap(QPixmap::fromImage(*image));
            scene->addItem(imgItem);
        }
    }
}

void MainWindow::on_alignAction_triggered()
{
	CFaceAlign face;
	int pointnum = face.PointNum();
	float *ptsPos= new float[pointnum];
	QGraphicsEllipseItem **dots = new QGraphicsEllipseItem*[pointnum];
	ptsPos = face.procPic((const char *)curFile.toLocal8Bit());

	for(int i=0;i<pointnum;i=i++)
	{
		dots[i] = new QGraphicsEllipseItem(QRect(ptsPos[2*i], ptsPos[2*i+1], 2.5, 2.5),imgItem);
		//if(i<172)
		//{
		//	dots[i] = new QGraphicsEllipseItem(QRect(ptsPos[i], ptsPos[i+1], 2, 2),imgItem);
		//	//switch(i)
		//	//{
		//	//case 14:
		//	//case 30:
		//	//	point2.x = ptsPos[i-14];
		//	//	point2.y = ptsPos[i-13];
		//	//	break;
		//	//case 50:
		//	//case 70:
		//	//	point2.x = ptsPos[i-18];
		//	//	point2.y = ptsPos[i-17];
		//	//	break;
		//	//case 94:
		//	//	point2.x = ptsPos[i-22];
		//	//	point2.y = ptsPos[i-21];
		//	//	break;
		//	//case 134:
		//	//	point2.x = ptsPos[i-38];
		//	//	point2.y = ptsPos[i-37];
		//	//}
		//	//line(img, point1, point2, CV_RGB(255,0,0));
		//}
		//else if(i == 172)
		//{
		//	dots[i] = new QGraphicsEllipseItem(QRect(ptsPos[i], ptsPos[i+1], 2, 2),imgItem);
		//}
		//else
		//{
		//	dots[i] = new QGraphicsEllipseItem(QRect(ptsPos[i], ptsPos[i+1], 2, 2),imgItem);
		//}
		dots[i]->setPen(QPen(Qt::red));
		dots[i]->setBrush(Qt::red);
		dots[i]->setFlag(QGraphicsItem::ItemIsMovable,true);
	} 
	ui->graphicsView->show();
	delete ptsPos;
	delete *dots;
}

void MainWindow::on_saveAction_triggered(){
	std::cout << "Hello";
}

void MainWindow::on_closeAction_triggered(){
	delete this->imgItem;
	this->scene->clear();
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
        int r = QMessageBox::warning(this, tr("FaceAlign"),
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
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
    }
    setWindowTitle(tr("%1 - %2[*]").arg(tr("FaceAlign")).arg(shownName));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}