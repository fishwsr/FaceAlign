#include "ModalProgressDialog.h"


ModalProgressDialog::ModalProgressDialog(QWidget *parent):QDialog(parent)
{
	ui.setupUi(this);
	ui.progressBar->setValue(0);
}


ModalProgressDialog::~ModalProgressDialog(void)
{
}

void ModalProgressDialog::setProgress(float percent){
	ui.progressBar->setValue(100*percent);
}
