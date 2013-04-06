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
	ui->stackedWidget->setCurrentIndex(0);
    this->image = new QImage();
	this->scene = new QGraphicsScene(parent);
	ui->statusBar->showMessage("Welcome");
	ui->leftGraphicsView->viewport()->installEventFilter(this);
    setCurrentFile("");
	isAligned = false;
	imgItem = NULL;

	//debug purpose
	openImage("./penny.jpg");
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
		openImage(fileName);
    }
}

void MainWindow::on_alignAction_triggered()
{
	CFaceAlign face;
	int pointnum = face.PointNum();
	float *ptsPos;
	//ptsPos = face.procPic((const char *)curFile.toLocal8Bit());
	ptsPos = face.procPic("temp/resizedPic.jpg");
	facemodel = new QFaceModel(ptsPos,pointnum,imgItem);
	ui->leftGraphicsView->show();
	isAligned = true;
	ui->sketchAction->setEnabled(isAligned);
	setWindowModified(true);
	delete[] ptsPos;
}

void MainWindow::on_sketchAction_triggered()
{
	CFaceSketch faceSketch;
	faceSketch.sketchFace(facemodel, image->width(), image->height());
	//std::vector<QGraphicsTextItem *> cmpnControlPts;
	//int cmpnPtsNum;
	//std::ifstream fin;
	//fin.open("colorMode\\mouth\\mouth1.pts");
	//if(!fin)
	//{
	//	return;
	//}
	//fin >> cmpnPtsNum;
	//cmpnControlPts.resize(cmpnPtsNum);
	//for (int i = 0; i < cmpnPtsNum; i++)
	//{
	//	double ptsX, ptsY;
	//	fin >> ptsX >>ptsY;
	//	cmpnControlPts[i] = new QGraphicsTextItem(QString::number(i), imgItem);
	//	cmpnControlPts[i]->setPos(ptsX,ptsY);
	//}
	//fin.close();
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
		ui->leftGraphicsView->setDisabled(true);
		ui->leftGraphicsView->setMouseTracking(false);
		ui->alignAction->setDisabled(true);
		ui->sketchAction->setDisabled(true);
	}
}

void MainWindow::on_showNodeIDAction_triggered()
{
	bool isChecked = ui->showNodeIDAction->isChecked();
	facemodel->setNodesIdVisible(isChecked);
};

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

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent* mouse = static_cast<QMouseEvent*>(event);
		QPointF pos = ui->leftGraphicsView->mapToScene(mouse->pos());
		ui->statusBar->showMessage(QString("x:%1,y:%2").arg(pos.x()).arg(pos.y()));
	}
	return false;
}

void MainWindow::contextMenuEvent( QContextMenuEvent *event )
{
	if (isAligned == true)
	{
		QMenu menu(this);
		menu.addAction(ui->showNodeIDAction);
		menu.exec(event->globalPos());
	}
}

void MainWindow::openImage( QString fileName )
{
		setCurrentFile(fileName);
		if(image->load(fileName))
		{
			on_closeAction_triggered();
			imgItem = new QGraphicsPixmapItem();
			int width = ui->leftGraphicsView->width();
			int hight = ui->leftGraphicsView->height();
			*image = image->scaled(width, hight, Qt::KeepAspectRatio);
			image->save("temp/resizedPic.jpg");

			imgItem->setPixmap(QPixmap::fromImage(*image));
			scene->addItem(imgItem);
			ui->leftGraphicsView->setEnabled(true);
			ui->leftGraphicsView->setScene(scene);
			//ui->graphicsView->adjustSize();
			ui->leftGraphicsView->show();
			ui->leftGraphicsView->setMouseTracking(true);
			ui->alignAction->setEnabled(true);
		}
}
