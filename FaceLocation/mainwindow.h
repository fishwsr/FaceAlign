#include "faceAlign.h"
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
	void on_closeAction_triggered();
//	bool save();

private:
//   void createActions();
//    void createMenus();
//    void createContextMenu();
//    bool loadFile(const QString &fileName);
	bool okToContinue();
 //   bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	QImage *image;
	QGraphicsPixmapItem *imgItem;
    QString curFile;
	QGraphicsScene *scene;

    QMenu *fileMenu;

    QAction *openAction;
	QAction *saveAction;
	QAction *exitAction;
	QAction *alignAction;
	QAction *separatorAction;

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H