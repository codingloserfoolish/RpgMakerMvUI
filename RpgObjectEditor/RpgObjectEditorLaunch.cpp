#include"RpgObjectEditorLaunch.h"
#include"../Manager/RpgObjectEditorManager.h"
#include"../RpgObjectEditor/RpgObjectEditorBase.h"
RpgObjectEditorLaunch::RpgObjectEditorLaunch(QObject* parent)
	:QObject(parent),
	m_current(0)
{
}

RpgObjectEditorLaunch::~RpgObjectEditorLaunch()
{
	RpgObjectEditorManager::destroy();
}

void RpgObjectEditorLaunch::showEditor(GameObject* item)
{
	RpgObjectEditorBase*next = RpgObjectEditorManager::instance()->editor(item);
	if (next == 0)return;
	if (next != m_current)
	{
		if (m_current != 0)
		{
			m_current->close();
			disconnect(m_current, 0, this, 0);
		}
		m_current=next;
		connect(m_current, &RpgObjectEditorBase::itemChanged, [this]() {emit itemChanged(); });
	}
	m_current->show();
}

void RpgObjectEditorLaunch::closeEditor()
{
	if (m_current)m_current->close();
}