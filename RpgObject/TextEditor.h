#pragma once
#include"../item/ObjectEditor.h"
#include<QPlainTextEdit>

#ifdef DLL_TEXT_EXPORT
#define TEXT_EXPORT Q_DECL_EXPORT
#else
#define TEXT_EXPORT Q_DECL_IMPORT
#endif // DLL_TEXT_EXPORT

namespace hjdd
{
	namespace widget
	{
		class TEXT_EXPORT TextEditWidget :public QPlainTextEdit
		{
			Q_OBJECT
		public:
			TextEditWidget(QWidget*parent=nullptr);
			virtual~TextEditWidget();
		};

		class TEXT_EXPORT TextEditor:public ObjectEditor
		{
			Q_OBJECT
		public:
			TextEditor(QWidget* parent = nullptr);
			virtual~TextEditor();
		protected:
			virtual void initLayOut(QVBoxLayout* vlayout);
			virtual void setProperties(item::GraphicsItem* item);
			virtual void Connection();
			virtual void DisConnection();
		private:
			TextEditWidget* m_textEdit;
		};
	}
}