#include"LineEditorEnterWidget.h"
#include<QBoxLayout> 

LineEditorEnterWidget::LineEditorEnterWidget(QWidget* parent):QWidget(parent)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);
	m_textedit = new QLineEdit(this);
	m_enter_btn = new QPushButton("ok",this);
	m_enter_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
	m_enter_btn->setFixedWidth(30);
	layout->addWidget(m_textedit);
	layout->addWidget(m_enter_btn);
	connect(m_enter_btn, &QPushButton::clicked, this, &LineEditorEnterWidget::slot_sendString);
	this->setLayout(layout);
}

void LineEditorEnterWidget::slot_sendString()
{
	if(!m_textedit->text().isEmpty())
	emit valueChanged(m_textedit->text());
}

void LineEditorEnterWidget::setValue(const QString&text)
{
	m_textedit->setText(text);
}
