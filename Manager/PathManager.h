#ifndef PATHMANAGER_H_
#define PATHMANAGER_H_
#include<qstring.h>
class PathManager
{
public:
	PathManager(const PathManager&) = delete;
	PathManager& operator=(const PathManager&) = delete;
	static PathManager* instance() { if (!m_instance) { m_instance = new PathManager; } return m_instance; }
	static void destroy() { if (m_instance)delete m_instance; }
	void open(QString& current);
	void setSceneName(QString& name) { m_sceneName = name; }
	QString& sceneName() { return m_sceneName; }
	QString image_path()const { return m_projectPath + "img/"; }
	QString plugin_path()const { return m_projectPath + "js/plugins/"; }
	QString save_path()const { return m_projectPath + "save/"; }
private:
	PathManager() {};
	static PathManager* m_instance;
	QString m_projectPath;
	QString m_sceneName;
};

#endif // !PATHMANAGER_H_

