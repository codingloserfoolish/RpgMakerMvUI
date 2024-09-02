#include "WinFontCalculate.h"
#include<qfontmetrics.h>
WinFontCalculate* WinFontCalculate::_instance=0;
void WinFontCalculate::init(QFont& font)
{
	m_font = font;
	QFontMetrics metrics(font);
	m_ascent = metrics.ascent();
	m_descent = metrics.descent();
	m_leading = metrics.leading();
	m_orignal_textRect = QSize(metrics.width("text"), m_leading + m_ascent + m_descent);
}
void WinFontCalculate::setFont(QFont& font)
{
	m_font = font;
	QFontMetrics metrics(font);
	m_ascent = metrics.ascent();
	m_descent = metrics.descent();
	m_leading = metrics.leading();
}

QSize WinFontCalculate::calculateTextRectSize(QStringList&list)
{
	int maxLen = 0;
	int size_width = 0;
	QFontMetrics metrics(m_font);
	for (QString& str : list)
	{
		if (maxLen < str.length())
		{
			maxLen = str.length();
			size_width = metrics.width(str);
		}
	}
	return QSize(size_width, list.size()*(m_ascent+m_descent+m_leading));
}
