#ifndef CONFIGUREINFOCONTAINER_H_
#define CONFIGUREINFOCONTAINER_H_
#include<qfontdatabase.h>
#include<qfontmetrics.h>
struct RpgWindowConfigureInfo
{
	int _fontSize;
	int _standardPadding;
	int _textPadding;
};

class ConfigureInfoContainer
{
public:
	ConfigureInfoContainer(const ConfigureInfoContainer&) = delete;
	ConfigureInfoContainer&operator=(const ConfigureInfoContainer&) = delete;
	static ConfigureInfoContainer* instance() { if (!m_instance)m_instance = new ConfigureInfoContainer; return m_instance; };
	static void destroy();
	RpgWindowConfigureInfo& window_configure(){ return m_winConfigure; }
	void setGameFont(const QString&path);
	QFont& gameFont() { return m_gameFont; }
private:
	ConfigureInfoContainer() { m_winConfigure = {28,18,6}; }
	static ConfigureInfoContainer* m_instance;
	QFont m_gameFont;
	RpgWindowConfigureInfo m_winConfigure;
};

#endif // !CONFIGUREINFOCONTAINER_H_
