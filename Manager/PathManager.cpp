#include"PathManager.h"
PathManager* PathManager::m_instance = 0;

void PathManager::open(QString& current)
{
	m_projectPath=current.left(current.lastIndexOf('/')+1);
}
