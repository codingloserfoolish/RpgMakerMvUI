#include"PreStartWidget.h"
#include<QVBoxLayout>
#include<qpushbutton.h>
PreStartWidget::PreStartWidget(QWidget* parent):QDialog(parent)
{
	this->setFixedWidth(500);
	QVBoxLayout* main_layer = new QVBoxLayout(this);
	QPushButton* create_btn = new QPushButton("create",this);
	QPushButton* load_btn = new QPushButton("load", this);
	QPushButton* exit_btn = new QPushButton("exit", this);
	main_layer->addWidget(create_btn);
	main_layer->addWidget(load_btn);
	main_layer->addWidget(exit_btn);

	connect(create_btn, &QPushButton::clicked, this, &PreStartWidget::slot_create_btn);
	connect(load_btn, &QPushButton::clicked, this, &PreStartWidget::slot_load_btn);
	connect(exit_btn, &QPushButton::clicked, this, &PreStartWidget::slot_exit_btn);

}

void PreStartWidget::slot_create_btn()
{
	this->done(1);
}
void PreStartWidget::slot_load_btn()
{
	this->done(2);
}
void PreStartWidget::slot_exit_btn()
{
	this->reject();
}