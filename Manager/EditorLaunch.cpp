#include"EditorLaunch.h"

namespace hjdd
{
	namespace manager
	{
		EditorLaunch::~EditorLaunch()
		{
		}
		void EditorLaunch::showEditor(item::GraphicsItem* item)
		{
			if (item == 0)return;
			auto it = m_editors.find(item->rpgobject_type());
			if (it == m_editors.end())
			{
				it = m_editors.insert(item->rpgobject_type(), item->editor());
				(*it)->initUI();
			}

			if (m_current_editor != it)
			{
				if (m_current_editor != m_editors.end())
				{
					(*m_current_editor)->close();
					(*m_current_editor)->releaseConnection();
					m_displayPos = (*m_current_editor)->pos();
				}
				m_current_editor = it;
			}

			(*it)->setObject(item);
			if (!(*it)->isVisible())
			{
				(*it)->move(m_displayPos);
				(*it)->show();
			}
		}
		void EditorLaunch::releaseConnection()
		{
			if (m_current_editor != m_editors.end())
			{
				(*m_current_editor)->releaseConnection();
			}
		}
		void EditorLaunch::closeEditor(item::GraphicsItem* item)
		{
			if (item == 0)return;
			if (m_current_editor != m_editors.end())
			{
				if ((*m_current_editor)->item() == item || item->findChild((*m_current_editor)->item()))
				{
					(*m_current_editor)->close();
				}
			}
		}
		void EditorLaunch::closeAll()
		{
			if (m_current_editor != m_editors.end())
			{
				(*m_current_editor)->close();
				//¶Ï¿ªÁ¬½Ó
				(*m_current_editor)->releaseConnection();
			}
		}
		void EditorLaunch::destroyAll()
		{
			m_current_editor = m_editors.end();
			for (QMap<int, widget::ObjectEditor*>::iterator it = m_editors.begin(); it != m_editors.end(); ++it)
			{
				delete (*it);
			}
		}
		void EditorLaunch::moveEdit(const QPoint& pos)
		{
			m_displayPos = pos;
			if (m_current_editor != m_editors.end())
			{
				(*m_current_editor)->move(pos);
			}
		}
	}
}