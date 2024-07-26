#ifndef PRESTARTWIDGET_H_
#define PRESTARTWIDGET_H_
#include<qdialog.h>

class PreStartWidget :public QDialog
{
	Q_OBJECT
public:
	PreStartWidget(QWidget* parent = nullptr);
public slots:
	void slot_create_btn();
	void slot_load_btn();
	void slot_exit_btn();
};
#endif//!PRESTARTWIDGET_H_