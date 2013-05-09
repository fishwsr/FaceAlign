#pragma once
#include <QDialog>
#include "ui_modalProgressDialog.h"

class ModalProgressDialog :	public QDialog
{
	Q_OBJECT
public:
	ModalProgressDialog(QWidget *parent = 0);
	~ModalProgressDialog(void);
public slots:
	void setProgress(float percent);
private:
	Ui::ModalProgressDialog ui;
};

