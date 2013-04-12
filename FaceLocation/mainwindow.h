#include "faceAlign.h"
#include "QFaceModel.h"
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QListWidget>
#include "FaceSketch.h"

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
	void on_thresholdSlider_valueChanged(int value);
	void on_alignAction_triggered();
	void on_saveAction_triggered();
	void on_closeAction_triggered();
	void on_sketchAction_triggered();
	void on_showNodeIDAction_triggered();
	void on_renderAction_triggered();

protected:
	bool eventFilter(QObject* object, QEvent* event);
	void contextMenuEvent(QContextMenuEvent *event);

private:
	bool okToContinue();
    void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	void initList(QListWidget* widgetList, QString filePath);
	bool isAligned;
	QImage *image;
	QGraphicsPixmapItem *imgItem;
    QString curFile;
	QGraphicsScene *leftGraphicsScene;
	QGraphicsScene *rightGraphicsScene;
	QFaceModel *facemodel;
	Ui::MainWindow *ui;
	CFaceSketch* faceSketch;
};
#endif // MAINWINDOW_H