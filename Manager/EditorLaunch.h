#pragma once
#include"SingleModel.hpp"
#include<QMap>
#include"../item/GraphicsItem.h"
#include"../item/ObjectEditor.h"
namespace hjdd
{
	namespace manager
	{
		class EditorLaunch :public SingleModel<EditorLaunch>
		{
		public:
			virtual~EditorLaunch();
			void showEditor(item::GraphicsItem*item);
			void releaseConnection();
			void closeEditor(item::GraphicsItem* item);
			bool isOpen() { return !(m_current_editor == m_editors.end() || !(*m_current_editor)->isVisible()); }
			void closeAll();
			void destroyAll();
			void moveEdit(const QPoint& pos);
		private:
			EditorLaunch()
			{
				m_current_editor = m_editors.end();
			}
			QMap<int, widget::ObjectEditor*>::iterator m_current_editor;
			QMap<int, widget::ObjectEditor*> m_editors;
			QPoint m_displayPos;
			friend class SingleModel<EditorLaunch>;
		};
	}
}