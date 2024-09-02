#ifndef WINFONTCALCULATE_H_
#define WINFONTCALCULATE_H_
#include<qfont.h>
#include<qsize.h>
class WinFontCalculate
{
public:
	WinFontCalculate(const WinFontCalculate&)=delete;
	WinFontCalculate&operator=(const WinFontCalculate&) = delete;
	static WinFontCalculate* instance() { if (!_instance) { _instance = new WinFontCalculate; }return _instance; }
	static void destroy() { if (_instance)delete _instance; }
	void init(QFont& font);
	void setFont(QFont&font);
	QSize calculateTextRectSize(QStringList&list);
	QFont& winFont() { return m_font; }
	int ascent()const { return m_ascent; }
	int descent()const { return m_descent; }
	int leading()const { return m_leading; }
	QSize& orginalSize() { return m_orignal_textRect; }
private:
	WinFontCalculate() {};
	static WinFontCalculate* _instance;

	QSize m_orignal_textRect;
	QFont m_font;
	int m_ascent;
	int m_descent;
	int m_leading;
};
#endif // !WINFONTCALCULATE_H_
