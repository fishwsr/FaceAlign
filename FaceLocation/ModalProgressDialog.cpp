#include "ModalProgressDialog.h"


ModalProgressDialog::ModalProgressDialog(QWidget *parent):QDialog(parent)
{
	ui.setupUi(this);
}


ModalProgressDialog::~ModalProgressDialog(void)
{
}

void ModalProgressDialog::setProgress(float percent){
	ui.progressBar->setValue(100*percent);
}
