#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "faceAlign.h"
#include "CThinPlateSpline.h"
#include <fstream>
#include <iostream>

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
	cv::Mat templateImg = cv::imread("colorMode\\nose\\nose1.jpg");
	cv::Mat bigTemplateImg(image->width(), image->height(), templateImg.type(),Scalar::all(0));
	Mat(templateImg, cvRect(0,0,templateImg.cols, templateImg.rows)).copyTo(Mat(bigTemplateImg, cvRect(0,0,templateImg.cols, templateImg.rows)));
	//bigTemplateImg.adjustROI(0, bigTemplateImg.rows, 0, bigTemplateImg.cols);
	bigTemplateImg(cvRect(0, 0, bigTemplateImg.cols, bigTemplateImg.rows));
	std::vector<cv::Point> noseControlPts;
	int nosePtsNum;
	std::ifstream fin;
	fin.open("colorMode\\nose\\nose1.pts");
	if(!fin)
	{
		return;
	}
	fin >> nosePtsNum;
	noseControlPts.resize(nosePtsNum);
	for (int i = 0; i < 12; i++)
	{
		double ptsX, ptsY;
		fin >> ptsX >>ptsY;
		noseControlPts[i].x = ptsX;
		noseControlPts[i].y = ptsY; 
	}
	fin.close();

	QVector<Node*> noseNodes;
	noseNodes = facemodel->getNoseNodes();
	std::vector<cv::Point> nosePts;
	nosePts.resize(nosePtsNum);
	for(int i = 0; i<12; i++)
	{
		nosePts[i].x = noseNodes[i]->sceneBoundingRect().center().x();
		nosePts[i].y = noseNodes[i]->sceneBoundingRect().center().y();
	}
	CThinPlateSpline tps(noseControlPts,nosePts);
	Mat warpedNose;
	tps.warpImage(bigTemplateImg, warpedNose,0.01,INTER_CUBIC,BACK_WARP);
	imwrite("temp\\mouth\\t1.jpg",bigTemplateImg);
	imwrite("temp\\mouth\\r1.jpg",warpedNose);
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
