#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "faceAlign.h"
#include "FaceSketch.h"

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
	isAligned = false;
	imgItem = NULL;
}

MainWindow::~MainWindow()
{
	delete image;
	if(imgItem != NULL)
	{
		delete imgItem;
	}
	delete scene;
	delete ui; 
	if(isAligned)
	{
		delete facemodel;
	}
}

void MainWindow::on_openAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(!fileName.isEmpty())
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
	float *ptsPos;
	ptsPos = face.procPic((const char *)curFile.toLocal8Bit());
	facemodel = new QFaceModel(ptsPos,pointnum,imgItem);
	ui->graphicsView->show();
	isAligned = true;
	setWindowModified(true);
	delete[] ptsPos;
}

void MainWindow::on_sketchAction_triggered()
{
	CFaceSketch faceSketch;
	faceSketch.componentSketch("nose", facemodel, image->width(), image->height());
}


void MainWindow::on_saveAction_triggered()
{
	
}

void MainWindow::on_closeAction_triggered()
{
	if (okToContinue()) 
	{
		delete imgItem;
		imgItem = NULL;
		setCurrentFile("");
		this->scene->clear();
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("FaceAlign"),
                        tr("The face has been aligned.\n"
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
