#include "faceAlign.h"
#include "QFaceModel.h"
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QListWidget>
#include "FaceSketch.h"
#include "VideoRenderer.h"
#include <Phonon/MediaSource>
#include <QCheckBox>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui/QMainWindow>

namespace Ui {
    class MainWindow;
}

class CFaceAlign;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();
	protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_openAction_triggered();
	void openImage( QString fileName );
	void on_bgThresholdSlider_valueChanged(int value);
	void on_qtzThresholdSlider_valueChanged(int value);
	void on_alignAction_triggered();
	void on_saveAction_triggered();
	void on_closeAction_triggered();
	void on_sketchAction_triggered();
	void on_showNodeIDAction_triggered();
	void on_renderAction_triggered();
	void on_openVideoAction_triggered();
	void on_playButton_clicked();
	void on_pauseButton_clicked();
	void on_stopButton_clicked();
	void on_chooseTemplateAction_triggered();
	void on_colorCheckBox_stateChanged(int value);

protected:
	bool eventFilter(QObject* object, QEvent* event);
	void contextMenuEvent(QContextMenuEvent *event);
	void freeOldResource();

private:
	int bgThresholdValue;
	int fcThresholdValue;
	int qtzThresholdValue;
	bool canTriggerSketch;
	bool okToContinue();
    void setCurrentFile(const QString &fileName, const QString &imgName = NULL);
	QString strippedName(const QString &fullFileName);
	void initList(QListWidget* widgetList, QString filePath);
	bool isAligned;
	QImage *image;
	QGraphicsPixmapItem *imgItem;
    QString curFile;
	QString curImg;
	QGraphicsScene *leftGraphicsScene;
	QGraphicsScene *rightGraphicsScene;
	QFaceModel *facemodel;
	Ui::MainWindow *ui;
	CFaceSketch* faceSketch;
	CVideoRenderer* videoRenderer;
	Phonon::MediaSource mediaSource;
	QCheckBox* colorCheckBox;
	void updateSketch();
};
#endif // MAINWINDOW_H