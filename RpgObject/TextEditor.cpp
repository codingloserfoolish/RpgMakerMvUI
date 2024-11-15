#include"TextEditor.h"
#include"Text.h"
namespace hjdd
{
	namespace widget
	{
		TextEditWidget::TextEditWidget(QWidget* parent):
			QPlainTextEdit(parent)
		{
		}
		TextEditWidget::~TextEditWidget()
		{
		}

		TextEditor::TextEditor(QWidget* parent) :
			ObjectEditor(parent)
		{
		}
		TextEditor::~TextEditor()
		{
		}
		void TextEditor::initLayOut(QVBoxLayout* vlayout)
		{
			m_textEdit = new TextEditWidget(this);
			vlayout->addWidget(m_textEdit);
		}
		void TextEditor::setProperties(item::GraphicsItem* item)
		{
			item::Text* textItem = (item::Text*)item;
			m_textEdit->setPlainText(textItem->text());
		}
		void TextEditor::Connection()
		{
			connect(m_textEdit, &TextEditWidget::textChanged, [this]() {
				((item::Text*)this->item())->setText(m_textEdit->toPlainText());
				});
		}
		void TextEditor::DisConnection()
		{
			disconnect(m_textEdit, 0, this, 0);
		}
	}
}