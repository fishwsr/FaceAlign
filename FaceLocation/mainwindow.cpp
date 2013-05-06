#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "faceAlign.h"
#include "FaceSketch.h"
#include "VideoRenderer.h"
#include <Phonon/MediaSource>
#include <Phonon/MediaObject>

using namespace Gdiplus; 

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
    this->image = new QImage();
	this->leftGraphicsScene = new QGraphicsScene(parent);
	this->rightGraphicsScene = new QGraphicsScene(parent);
	ui->statusBar->showMessage("Welcome");
	ui->leftGraphicsView->viewport()->installEventFilter(this);
	ui->rightGraphicsView->viewport()->installEventFilter(this);
	QLabel *bgThresholdLabel = new QLabel("Curve");
	QLabel *fcThresholdLabel = new QLabel("Quantization");
	QSlider *bgThresholdSlider=new QSlider(Qt::Horizontal);
	QSlider *qtzThresholdSlider=new QSlider(Qt::Horizontal);
	QSpinBox *bgThresholdSpinbox=new QSpinBox;
	QSpinBox *qtzThresholdSpinbox=new QSpinBox;
	bgThresholdSlider->setRange(30,150);
	qtzThresholdSlider->setRange(1,50);
	bgThresholdSpinbox->setRange(30,150);
	qtzThresholdSpinbox->setRange(1,50);
	bgThresholdSlider->setValue(60);
	qtzThresholdSlider->setValue(15);
	bgThresholdSpinbox->setValue(60);
	qtzThresholdSpinbox->setValue(15);
	ui->rightToolBar->addSeparator();
	ui->rightToolBar->addWidget(qtzThresholdSpinbox);
	ui->rightToolBar->addWidget(qtzThresholdSlider);
	ui->rightToolBar->addWidget(fcThresholdLabel);
	ui->rightToolBar->addSeparator();
	ui->rightToolBar->addWidget(bgThresholdSpinbox);
	ui->rightToolBar->addWidget(bgThresholdSlider);
	ui->rightToolBar->addWidget(bgThresholdLabel);
	QObject::connect(bgThresholdSpinbox,SIGNAL(valueChanged(int)),bgThresholdSlider,SLOT(setValue(int)));
	QObject::connect(qtzThresholdSpinbox,SIGNAL(valueChanged(int)),qtzThresholdSlider,SLOT(setValue(int)));
	QObject::connect(bgThresholdSlider,SIGNAL(valueChanged(int)),bgThresholdSpinbox,SLOT(setValue(int)));
	QObject::connect(qtzThresholdSlider,SIGNAL(valueChanged(int)),qtzThresholdSpinbox,SLOT(setValue(int)));
	QObject::connect(bgThresholdSlider,SIGNAL(valueChanged(int)),this,SLOT(on_bgThresholdSlider_valueChanged(int)));
	QObject::connect(qtzThresholdSlider,SIGNAL(valueChanged(int)),this,SLOT(on_qtzThresholdSlider_valueChanged(int)));
    bgThresholdValue = 60;
	fcThresholdValue = 80;
	qtzThresholdValue = 15;
	ui->rightToolBar->setDisabled(true);
	
	setCurrentFile("");
	isAligned = false;
	imgItem = NULL;
	videoRenderer = NULL;
	faceSketch = NULL;

	//debug purpose
	//openImage("./penny.jpg");

	initList(ui->browListWidget, QString("data/sketchMode/leftEyeBrow/leftEyebrow"));
	initList(ui->eyeListWidget, QString("data/colorMode/leftEye/leftEye"));
	initList(ui->noseListWidget, QString("data/sketchMode/nose/nose"));
	initList(ui->mouthListWidget, QString("data/sketchMode/mouth/mouth"));
	initList(ui->faceContourListWidget, QString("data/sketchMode/faceContour/faceContour"));

}

MainWindow::~MainWindow()
{
	delete image;
	if(imgItem != NULL)
	{
		delete imgItem;
	}
	delete leftGraphicsScene;
	delete ui; 
	if(isAligned)
	{
		delete facemodel;
	}
	freeOldResource();
}

void MainWindow::on_openAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image file",
                ".",
                "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(!fileName.isEmpty())
    {
		freeOldResource();
		faceSketch = new CFaceSketch();
		openImage(fileName);
		setCurrentFile(fileName);
		ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_alignAction_triggered()
{
	CFaceAlign face;
	int pointnum = face.PointNum();
	float *ptsPos;
	ptsPos = face.procPic((const char *)curImg.toLocal8Bit());
	//ptsPos = face.procPic("temp/resizedPic.jpg");
	facemodel = new QFaceModel(ptsPos,pointnum,imgItem);
	ui->leftGraphicsView->show();
	isAligned = true;
	ui->sketchAction->setEnabled(isAligned);
	setWindowModified(true);
	ui->alignAction->setDisabled(true);
	delete[] ptsPos;
	ui->templateAreaWidget->setEnabled(true);
	ui->browListWidget->setCurrentRow(faceSketch->getBrowIndex()-1);
	ui->eyeListWidget->setCurrentRow(faceSketch->getEyeIndex()-1);
	ui->noseListWidget->setCurrentRow(faceSketch->getNoseIndex()-1);
	ui->mouthListWidget->setCurrentRow(faceSketch->getMouthIndex()-1);
	ui->faceContourListWidget->setCurrentRow(faceSketch->getFaceIndex()-1);
}

void MainWindow::on_sketchAction_triggered()
{
	faceSketch->setBrowIndex(ui->browListWidget->currentRow() + 1);
	faceSketch->setEyeIndex(ui->eyeListWidget->currentRow()+1);
	faceSketch->setNoseIndex(ui->noseListWidget->currentRow()+1);
	faceSketch->setMouthIndex(ui->mouthListWidget->currentRow()+1);
	faceSketch->setFaceIndex(ui->faceContourListWidget->currentRow()+1);

	Mat srcImg = imread((const char *)curImg.toLocal8Bit());
	if(srcImg.empty()) {
		qDebug("image file not Found");
		return;
	}
	Mat sketchedImg = faceSketch->sketchFace(facemodel,srcImg);
	imwrite("temp/wholeSketch.jpg", sketchedImg);

	QImage* rightImage = new QImage();
	if(rightImage->load("temp/wholeSketch.jpg"))
	{
		this->rightGraphicsScene->clear();
		
		QGraphicsPixmapItem* rightImgItem = new QGraphicsPixmapItem();
		int width = ui->rightGraphicsView->width();
		int hight = ui->rightGraphicsView->height();
		/**image = image->scaled(width, hight, Qt::KeepAspectRatio);
		image->save("temp/resizedPic.jpg");*/

		rightImgItem->setPixmap(QPixmap::fromImage(*rightImage));
		rightGraphicsScene->addItem(rightImgItem);
		ui->rightGraphicsView->setEnabled(true);
		ui->rightGraphicsView->setScene(rightGraphicsScene);
		ui->rightGraphicsView->show();
		ui->rightGraphicsView->setMouseTracking(true);
		ui->rightToolBar->setEnabled(true);
	}

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
	QImage image(image->size(), image->format());
	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing);
	leftGraphicsScene->render(&painter);
	image.save("temp/aligned.jpg");
}

void MainWindow::on_closeAction_triggered()
{
	if (okToContinue()) 
	{
		delete imgItem;
		imgItem = NULL;
		setCurrentFile("");
		this->leftGraphicsScene->clear();
		this->rightGraphicsScene->clear();
		ui->leftGraphicsView->setDisabled(true);
		ui->leftGraphicsView->setMouseTracking(false);
		ui->alignAction->setDisabled(true);
		ui->sketchAction->setDisabled(true);
		ui->renderAction->setDisabled(true);
		ui->rightToolBar->setDisabled(true);
		freeOldResource();
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
void MainWindow::setCurrentFile(const QString &fileName, const QString &imgName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
    }
    setWindowTitle(tr("%1 - %2[*]").arg(tr("FaceLocation")).arg(shownName));
	if (imgName == NULL)
	{
		curImg = curFile;
	}
	else
	{
		curImg = imgName;
	}
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
		if(image->load(fileName))
		{
			this->leftGraphicsScene->clear();	
			imgItem = new QGraphicsPixmapItem();
			/*int width = ui->leftGraphicsView->width();
			int hight = ui->leftGraphicsView->height();
			*image = image->scaled(width, hight, Qt::KeepAspectRatio);
			image->save("temp/resizedPic.jpg");*/

			imgItem->setPixmap(QPixmap::fromImage(*image));
			leftGraphicsScene->addItem(imgItem);
			ui->leftGraphicsView->setEnabled(true);
			ui->leftGraphicsView->setScene(leftGraphicsScene);
			//ui->graphicsView->adjustSize();
			ui->leftGraphicsView->show();
			ui->leftGraphicsView->setMouseTracking(true);
			ui->alignAction->setEnabled(true);
			this->rightGraphicsScene->clear();
		}		
}

void MainWindow::on_renderAction_triggered()
{
	ui->templateAreaWidget->setDisabled(true);
	QString renderedVideoPath = "temp/"+ QFileInfo(curFile).baseName() + ".avi";
	videoRenderer->render(renderedVideoPath.toStdString(), bgThresholdValue, qtzThresholdValue);
	QMessageBox::information(this,"Face Location", "Video rendering completed");
	ui->stackedWidget->setCurrentIndex(1);
	ui->sketchAction->setDisabled(true);
	ui->templateAreaWidget->setDisabled(true);
	ui->rightVideoPlayer->load(Phonon::MediaSource(renderedVideoPath));
}

void MainWindow::initList( QListWidget* widgetList, QString filePath )
{
	
		widgetList->setIconSize(QSize(100, 100));
		widgetList->setSpacing(15);
		QImage templateImg;
		for(int i = 1; i <= 100; i++) {
			QString prefix(filePath);
			prefix.append(QString::number(i)).append(".JPG");
			if(templateImg.load(prefix)){
				QIcon icon(QPixmap::fromImage(templateImg));
				//icon.actualSize(QSize(100, 100));
				QListWidgetItem* widgetItem = new QListWidgetItem();
				widgetItem->setIcon(icon);
				widgetList->addItem(widgetItem);
			}else {
				qDebug(prefix.append(" NOT found").toLocal8Bit());
				break;
			}
		}


}

void MainWindow::on_openVideoAction_triggered()
{
	QString videoFileName = QFileDialog::getOpenFileName(
		this, "open video file",
		".",
		"Video files (*.mp4 *.avi);;All files (*.*)");
	if(!videoFileName.isEmpty())
	{
		freeOldResource();
		setCurrentFile(videoFileName, "temp/firstFrame.jpg");
		faceSketch = new CFaceSketch();
		videoRenderer = new CVideoRenderer((const char *)videoFileName.toLocal8Bit(), faceSketch);
		Mat firstFrame = videoRenderer->getFirstFrame();
		imwrite(curImg.toStdString(), firstFrame);
		openImage(curImg);
		mediaSource = Phonon::MediaSource(videoFileName);
		ui->leftVideoPlayer->load(mediaSource);
		ui->seekSlider->setMediaObject(ui->leftVideoPlayer->mediaObject());
		ui->volumeSlider->setAudioOutput(ui->leftVideoPlayer->audioOutput());
		ui->renderAction->setEnabled(true);
	}

}

void MainWindow::freeOldResource()
{
	if(videoRenderer != NULL) {
		delete videoRenderer;
		videoRenderer = NULL;
	}
	if(faceSketch != NULL) {
		delete faceSketch;
		faceSketch = NULL;
	}
}

void MainWindow::on_playButton_clicked()
{
	ui->leftVideoPlayer->play();
	ui->rightVideoPlayer->play();
	ui->pauseButton->setEnabled(true);
	ui->stopButton->setEnabled(true);
	ui->playButton->setEnabled(false);
}

void MainWindow::on_pauseButton_clicked()
{
	ui->leftVideoPlayer->pause();
	ui->rightVideoPlayer->pause();
	ui->pauseButton->setEnabled(false);
	ui->stopButton->setEnabled(true);
	ui->playButton->setEnabled(true);
}

void MainWindow::on_stopButton_clicked()
{
	ui->leftVideoPlayer->stop();
	ui->rightVideoPlayer->stop();
	ui->pauseButton->setEnabled(false);
	ui->stopButton->setEnabled(false);
	ui->playButton->setEnabled(true);
}

void MainWindow::on_bgThresholdSlider_valueChanged( int value )
{
	bgThresholdValue = value;
	updateSketch();
}

void MainWindow::on_qtzThresholdSlider_valueChanged( int value )
{
	qtzThresholdValue = value;
	updateSketch();
}

void MainWindow::updateSketch()
{
	if(faceSketch == NULL) {
		qDebug("########################################Sketch Must be Done First!!!");
		exit(-1);
	}
	Mat srcImg = imread((const char *)curImg.toLocal8Bit());
	if(srcImg.empty()) {
		qDebug("image file not Found");
		return;
	}
	faceSketch->updateBackground(srcImg, bgThresholdValue, qtzThresholdValue);

	QImage* rightImage = new QImage();
	if(rightImage->load("temp/wholeSketch.jpg"))
	{
		this->rightGraphicsScene->clear();

		QGraphicsPixmapItem* rightImgItem = new QGraphicsPixmapItem();
		int width = ui->rightGraphicsView->width();
		int hight = ui->rightGraphicsView->height();
		rightImgItem->setPixmap(QPixmap::fromImage(*rightImage));
		rightGraphicsScene->addItem(rightImgItem);
		ui->rightGraphicsView->setEnabled(true);
		ui->rightGraphicsView->setScene(rightGraphicsScene);
		ui->rightGraphicsView->show();
		ui->rightGraphicsView->setMouseTracking(true);		
	}
}

