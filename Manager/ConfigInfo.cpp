#include"ConfigInfo.h"
#include<QFontDatabase>
#include<QFile>

//QSize MANAGER_EXPORT hjdd::manager::ConfigInfo::graphicsSize;

bool hjdd::manager::ConfigInfo::loadWorkSpace(const QString& workspace)
{
	m_workspace = workspace;
	QString fontPath = workspace + "fonts/mplus-1m-regular.ttf";
	if (QFile::exists(fontPath))
	{
		m_gameFont = QFontDatabase::applicationFontFamilies(
			QFontDatabase::addApplicationFont(fontPath)
		).at(0);
		return true;
	}
	return false;
}
