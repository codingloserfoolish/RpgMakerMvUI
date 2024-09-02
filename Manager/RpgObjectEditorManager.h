#ifndef RPGOBJECTEDITORMANAGER_H_
#define RPGOBJECTEDITORMANAGER_H_
#include<qmap.h>
class GameObject;
class RpgObjectEditorBase;
class RpgObjectEditorManager
{
public:
	RpgObjectEditorManager(const RpgObjectEditorManager&)=delete;
	RpgObjectEditorManager& operator=(const RpgObjectEditorManager&) = delete;
	~RpgObjectEditorManager();
	static RpgObjectEditorManager* instance() { if (!m_instance)m_instance = new RpgObjectEditorManager; return m_instance; }
	static void destroy() { if (m_instance)delete m_instance; }
	/*
	* 获取、创建编辑窗口
	*/
	RpgObjectEditorBase* editor(GameObject*item);
private:
	RpgObjectEditorManager();

	QMap<int, RpgObjectEditorBase*> m_rpgobject_editors;

	static RpgObjectEditorManager* m_instance;
};
#endif // !RPGOBJECTEDITORMANAGER_H_
