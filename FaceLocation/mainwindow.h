#include "faceAlign.h"
#include "QFaceModel.h"
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsPixmapItem>

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
	void on_alignAction_triggered();
	void on_saveAction_triggered();
	void on_closeAcion_triggered();

private:
	bool okToContinue();
    void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	bool isAligned;
	QImage *image;
	QGraphicsPixmapItem *imgItem;
    QString curFile;
	QGraphicsScene *scene;
	QFaceModel *facemodel;

    QMenu *fileMenu;

    QAction *openAction;
	QAction *saveAction;
	QAction *exitAction;
	QAction *alignAction;
	QAction *separatorAction;

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H