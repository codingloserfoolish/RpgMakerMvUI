#include"LinesTextEditWidget.h"
#include<QContextMenuEvent>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<qpushbutton.h>

#include<qdebug.h>
#include"../../../Dialog/IconSelectedDialog.h"
#include"../../../Dialog/VariableSelectedDialog.h"
#include"../../WindowElement/wbLinesText.h"
LinesTextEditWidget::LinesTextEditWidget(QWidget* parent):
	QTextEdit(parent)
{
	
}

void LinesTextEditWidget::insertText_Extra(QString& text)
{
	this->textCursor().insertText(text);
}

void LinesTextEditWidget::slot_selectIcon()
{
	int id = IconSelectedDialog::GetIconId();
	this->insertText_Extra(QString("\\\\I[%1]").arg(id));
}

void LinesTextEditWidget::slot_selectVariable()
{
	this->insertText_Extra(QString("\\\\V[%1]").arg(VariableSelectedDialog::GetVariables()));
}

void LinesTextEditWidget::contextMenuEvent(QContextMenuEvent* event)
{
	qDebug()<<this->textCursor().position();
}

LinesTextEditMainWidget::LinesTextEditMainWidget(QWidget* parent):ElementWidgetBase(parent)
{
	QVBoxLayout* main_layer = (QVBoxLayout*)this->layout();
	QHBoxLayout* btn_layer = new QHBoxLayout;
	QPushButton* icon_btn = new QPushButton("insert icon", this);
	QPushButton* variables_btn = new QPushButton("insert variables", this);
	btn_layer->addWidget(icon_btn);
	btn_layer->addWidget(variables_btn);
	main_layer->addLayout(btn_layer);

	m_text_edit = new LinesTextEditWidget(this);
	main_layer->addWidget(m_text_edit);

	connect(icon_btn, &QPushButton::clicked, m_text_edit, &LinesTextEditWidget::slot_selectIcon);
	connect(variables_btn, &QPushButton::clicked, m_text_edit, &LinesTextEditWidget::slot_selectVariable);
	connect(m_text_edit, &QTextEdit::textChanged, this, &LinesTextEditMainWidget::slot_TextChanged);

}

void LinesTextEditMainWidget::setOtherProperty(WinElementBase*item)
{
	m_text_edit->blockSignals(true);
	m_text_edit->setText(((wbLinesText*)item)->text());
	m_text_edit->blockSignals(false);
}

void LinesTextEditMainWidget::slot_TextChanged()
{
	wbLinesText* item = (wbLinesText*)(this->item());
	item->setText(m_text_edit->toPlainText());
	emit itemChanged();
}