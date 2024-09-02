#include"RpgTextEditor.h"
#include<qpushbutton.h>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include"../RpgObject/RpgText.h"
#include"../Dialog/IconSelectedDialog.h"
#include"../Dialog/VariableSelectedDialog.h"
#include"../Dialog/ColorSelectedDialog.h"

TextWidget::TextWidget(QWidget* parent)
	:QTextEdit(parent){}

void TextWidget::slot_insertIcon()
{
	int index = IconSelectedDialog::GetIconId();
	if(index!=0)this->textCursor().insertText(QString("\\I[%1]").arg(index));
}

void TextWidget::slot_insertVariable()
{
	QString data = VariableSelectedDialog::GetVariables();
	if(!data.isEmpty())this->textCursor().insertText(QString("\\V[%1]").arg(data));
}

void TextWidget::slot_insertColor()
{
	int index = ColorSelectedDialog::GetColorIndex();
	if(index!=-1)this->textCursor().insertText(QString("\\C[%1]").arg(index));
}

void TextWidget::contextMenuEvent(QContextMenuEvent* event)
{
}

RpgTextEditor::RpgTextEditor(QWidget* parent)
	:RpgObjectEditorBase(parent)
{
	QVBoxLayout* main_layer = new QVBoxLayout(this);
	QHBoxLayout* btn_layer = new QHBoxLayout;
	main_layer->addLayout(btn_layer);

	QPushButton* Icon_btn=new QPushButton("icon", this);
	QPushButton* Var_btn = new QPushButton("variable", this);
	QPushButton* Color_btn = new QPushButton("color", this);
	btn_layer->addWidget(Icon_btn);
	btn_layer->addWidget(Var_btn);
	btn_layer->addWidget(Color_btn);

	m_text_edit = new TextWidget(this);
	main_layer->addWidget(m_text_edit);
	connect(Icon_btn, &QPushButton::clicked, m_text_edit, &TextWidget::slot_insertIcon);
	connect(Var_btn, &QPushButton::clicked, m_text_edit, &TextWidget::slot_insertVariable);
	connect(Color_btn, &QPushButton::clicked, m_text_edit, &TextWidget::slot_insertColor);
	connect(m_text_edit, &QTextEdit::textChanged, this,&RpgTextEditor::slot_TextChanged);
}

void RpgTextEditor::slot_TextChanged()
{
	RpgText*item=(RpgText*)(this->item());
	item->setText(m_text_edit->toPlainText());
	emit itemChanged();
}

void RpgTextEditor::InitEditor(GameObject* item)
{
	RpgText* text_item = (RpgText*)item;
	m_text_edit->setText(text_item->text());
}


