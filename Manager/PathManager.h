#ifndef PATHMANAGER_H_
#define PATHMANAGER_H_
#include<qstring.h>
#include<qdatetime.h>
#include<qfile.h>
struct PathSaveNode
{
	QString _scene_name;
	QString _path;
	QString _time;
};

class HistoryList
{
public:
	HistoryList(const QString&file);
	~HistoryList();
	void push_back(PathSaveNode* node);
	PathSaveNode* check(PathSaveNode* node);
	bool exist(QString& sceneName, QString& path);
	void remove(QString& sceneName, QString& path);
protected:
	QFile m_file;
	std::vector<PathSaveNode*> m_list;
};

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
	QString data_path()const { return m_projectPath + "data/"; }
	QString plugin_path()const { return m_projectPath + "js/plugins/"; }
	QString save_path()const { return m_projectPath + "save/"; }
	QString font_path()const { return m_projectPath + "fonts/mplus-1m-regular.ttf"; }
	QString& project_path() { return m_projectPath; }
	QString& created_time() { return m_timeStr; }
private:
	PathManager() {};
	static PathManager* m_instance;
	QString m_projectPath;
	QString m_sceneName;
	QString m_timeStr;
};

#endif // !PATHMANAGER_H_

