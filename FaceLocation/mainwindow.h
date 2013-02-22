#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
 
namespace Ui {
    class MainWindow;
}

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
//	bool save();

private:
//   void createActions();
//    void createMenus();
//    void createContextMenu();
//    bool loadFile(const QString &fileName);
	bool okToContinue();
 //   bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

	QImage *image;
    QString curFile;
	CFaceAlign face;
	QAction *separatorAction;

    QMenu *fileMenu;

    QAction *openAction;
	QAction *saveAction;
	QAction *exitAction;
	QAction *alignAction;

	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
