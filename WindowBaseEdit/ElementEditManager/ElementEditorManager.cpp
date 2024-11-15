#include"ElementEditorManager.h"
namespace hjdd
{
	namespace manager
	{
		std::shared_ptr<ElementEditorManager> ElementEditorManager::_instance=nullptr;
		ElementEditorManager::~ElementEditorManager(){}
		void ElementEditorManager::show(elements::Element* ele)
		{
			QMap<int, elements::ElementEdit*>::iterator it = m_editors.find(ele->element_type());
			if (it == m_editors.end())
			{
				it=m_editors.insert(ele->element_type(),ele->editor());
			}
			if (m_current_editor != it)
			{
				if (m_current_editor != m_editors.end())
				{
					disconnect((*m_current_editor),0,this,0);
					(*m_current_editor)->DisConnection();
					if ((*m_current_editor)->isVisible())(*m_current_editor)->close();
				}
				m_current_editor = it;
				connect((*m_current_editor), &elements::ElementEdit::propertiesChanged,this,&ElementEditorManager::slot_sendpropertiesChanged);
			}
			(*m_current_editor)->setElement(ele);
			if(!(*m_current_editor)->isVisible())
				(*m_current_editor)->show();
		}
		void ElementEditorManager::close()
		{
			if (m_current_editor != m_editors.end())
			{
				disconnect((*m_current_editor), 0, this, 0);
				(*m_current_editor)->DisConnection();
				if ((*m_current_editor)->isVisible())
					(*m_current_editor)->close();
				m_current_editor == m_editors.end();
			}
		}
		void ElementEditorManager::clearDeletedElementConnect(elements::Element* ele)
		{
			if (!ele)return;
			QMap<int, elements::ElementEdit*>::iterator it = m_current_editor;
			if (it.key() != ele->element_type()){it = m_editors.find(ele->element_type());}

			if (it != m_editors.end()&&(*it)->element()==ele)
			{
				(*m_current_editor)->DisConnection();
				if ((*m_current_editor)->isVisible())(*m_current_editor)->close();
			}
		}
		void ElementEditorManager::destroyAll()
		{
			for (QMap<int, elements::ElementEdit*>::iterator it = m_editors.begin(); it != m_editors.end(); ++it)
			{
				delete (*it);
			}
		}
		ElementEditorManager::ElementEditorManager()
		{
			m_current_editor = m_editors.end();
		}
	}
}