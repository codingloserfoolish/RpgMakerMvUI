#include"ElementWidgetBase.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<qlabel.h>
#include"../SpinBox/SpinBox.h"
#include"../../WindowElement/WinElementBase.h"
ElementWidgetBase::ElementWidgetBase(QWidget* parent):QWidget(parent),m_item(0)
{
	QVBoxLayout* main_layer = new QVBoxLayout(this);
	QLabel* x_lab = new QLabel("x", this);
	m_disp_x = new SpinBox(this);
	QLabel* y_lab = new QLabel("y", this);
	m_disp_y = new SpinBox(this);
	QHBoxLayout* disp_layer = new QHBoxLayout;
	main_layer->addLayout(disp_layer);
	disp_layer->addWidget(x_lab);
	disp_layer->addWidget(m_disp_x);
	disp_layer->addWidget(y_lab);
	disp_layer->addWidget(m_disp_y);
	connect(m_disp_x, &QSpinBox::textChanged, this, &ElementWidgetBase::slot_sendDispChanged);
	connect(m_disp_y, &QSpinBox::textChanged, this, &ElementWidgetBase::slot_sendDispChanged);
}

void ElementWidgetBase::setDispValue(int x, int y)
{
	m_disp_x->blockSignals(true);
	m_disp_y->blockSignals(true);
	m_disp_x->setValue(x);
	m_disp_y->setValue(y);
	m_disp_x->blockSignals(false);
	m_disp_y->blockSignals(false);
}

void ElementWidgetBase::updateDispValue()
{
	this->setDispValue(m_item->rect().x(),m_item->rect().y());
}

void ElementWidgetBase::setValue(WinElementBase* element)
{
	m_item = element;
	this->setDispValue(element->rect().x(), element->rect().y());
	this->setOtherProperty(element);
}

void ElementWidgetBase::slot_sendDispChanged()
{
	m_item->rect().moveTo(m_disp_x->value(), m_disp_y->value());
	emit itemChanged();
}