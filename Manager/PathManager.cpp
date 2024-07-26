#include"PathManager.h"
#include<qtextstream.h>
PathManager* PathManager::m_instance = 0;

HistoryList::HistoryList(const QString& file):m_file(file)
{
	m_file.open(QIODevice::ReadOnly);
	QTextStream stream(&m_file);
	PathSaveNode* node = 0;
	while (!stream.atEnd())
	{
		node = new PathSaveNode;
		node->_scene_name = stream.readLine();
		node->_path = stream.readLine();
		node->_time = stream.readLine();
		m_list.push_back(node);
	}
	m_file.close();
}

HistoryList::~HistoryList()
{
	m_file.open(QIODevice::WriteOnly);
	m_file.seek(0);
	QTextStream stream(&m_file);
	for (PathSaveNode* node : m_list)
	{
		stream << node->_scene_name << '\n';
		stream << node->_path << '\n';
		stream << node->_time << '\n';
		delete node; 
	}
	m_file.close();
}

void HistoryList::push_back(PathSaveNode* node)
{
	if (!check(node))
	{
		m_list.push_back(node);
	}
}

PathSaveNode* HistoryList::check(PathSaveNode* node)
{
	for (PathSaveNode* child : m_list)
	{
		if (child->_path == node->_path&&child->_scene_name==node->_scene_name)
		{
			child->_time = node->_time;
			delete node;
			return child;
		}
	}
	return 0;
}

bool HistoryList::exist(QString& sceneName, QString& path)
{
	for (PathSaveNode* child : m_list)
	{
		if (child->_path == path && child->_scene_name == sceneName)
		{
			return true;
		}
	}
	return false;
}

void HistoryList::remove(QString& sceneName, QString& path)
{
	for (std::vector<PathSaveNode*>::iterator it = m_list.begin(); it != m_list.end(); ++it)
	{
		if ((*it)->_scene_name == sceneName && (*it)->_path == path)
		{
			PathSaveNode* item = (*it);
			m_list.erase(it);
			delete item;
			return;
		}
	}
}

void PathManager::open(QString& current)
{
	//m_projectPath=current.left(current.lastIndexOf('/')+1);
	m_projectPath = current;
	m_timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}
