#pragma once
#include"../item/ObjectEditor.h"

#ifdef DLL_WINDOWBASE_EXPORT
#define WINDOWBASE_EXPORT Q_DECL_EXPORT
#else
#define WINDOWBASE_EXPORT Q_DECL_IMPORT
#endif // RPGOBJECT_EXPORT
namespace hjdd
{
	namespace widget
	{
		class ElementDisplayWidget;
		class WINDOWBASE_EXPORT WindowBaseEditor:public ObjectEditor
		{
			Q_OBJECT
		public:
			WindowBaseEditor(QWidget*parent=nullptr);
			virtual~WindowBaseEditor();
		public slots:
			void slot_updateItem();
		protected:
			virtual void initLayOut(QVBoxLayout* vlayout);
			virtual void setProperties(item::GraphicsItem* item);
			virtual void Connection();
			virtual void DisConnection();

			virtual void closeEvent(QCloseEvent* e);
		private:
			ElementDisplayWidget* m_tinyWorld;
		};
	}
}