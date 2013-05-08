#pragma once
#include <QDialog>
#include "ui_modalProgressDialog.h"

class ModalProgressDialog :	public QDialog
{
	Q_OBJECT
public:
	ModalProgressDialog(QWidget *parent = 0);
	~ModalProgressDialog(void);
private:
	Ui::ModalProgressDialog ui;
};

