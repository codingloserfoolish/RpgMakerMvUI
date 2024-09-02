#include"ConfigureInfoContainer.h"
#include"WinFontCalculate.h"
ConfigureInfoContainer* ConfigureInfoContainer::m_instance = 0;

void ConfigureInfoContainer::setGameFont(const QString& path)
{
	int fontId=QFontDatabase::addApplicationFont(path);
	QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
	m_gameFont = QFont(fontName);
	m_gameFont.setPixelSize(m_winConfigure._fontSize);
	WinFontCalculate::instance()->init(this->m_gameFont);
}

void ConfigureInfoContainer::destroy()
{
	if (m_instance)delete m_instance;
	WinFontCalculate::destroy();
}