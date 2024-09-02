#include"RpgObjectEditorManager.h"
#include"../RpgObject/GameObject.h"
#include"../RpgObjectEditor/RpgObjectEditorBase.h"
RpgObjectEditorManager* RpgObjectEditorManager::m_instance = 0;
RpgObjectEditorManager::~RpgObjectEditorManager()
{
	for (QMap<int, RpgObjectEditorBase*>::iterator it = m_rpgobject_editors.begin(); it != m_rpgobject_editors.end(); ++it)
	{
		delete it.value();
	}
}
RpgObjectEditorBase* RpgObjectEditorManager::editor(GameObject* item)
{
	if (item == 0)return 0;
	RpgObjectEditorBase* result = m_rpgobject_editors.value(item->gm_type(), 0);
	if (result == 0)
	{
		result = item->createEditor();
		if (result == 0)return 0;
		m_rpgobject_editors.insert(item->gm_type(), result);
	}
	result->setObject(item);
	return result;
}
RpgObjectEditorManager::RpgObjectEditorManager(){}