#pragma once
#include"SingleModel.hpp"
#include<QString>
#include<QSize>
#include"config.h"
namespace hjdd
{
	namespace manager
	{
		class MANAGER_EXPORT ConfigInfo:public SingleModel<ConfigInfo>
		{
		public:
			bool loadWorkSpace(const QString&workspace);
			void setSceneName(const QString& SceneName) { m_filename = SceneName; };
			QString imgPath() { return m_workspace + "img/"; }
			QString pluginPath() { return m_workspace + "js/plugins/"; }
			QString savePath() { return m_workspace + "UIData/"; }
			QString fontFamily() { return m_gameFont; }
			QString& SceneName() { return m_filename; }

			//static QSize graphicsSize;
		private:
			QString m_gameFont;
			QString m_workspace;
			QString m_filename;//保存文件的文件名
		};
	}
}