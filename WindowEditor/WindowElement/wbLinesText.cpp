#include "wbLinesText.h"
#include<qdom.h>
#include"../ElementWidget/LinesTextEditWidget/LinesTextEditWidget.h"
#include"../../Manager/WinFontCalculate.h"
wbLinesText::wbLinesText():
	m_text("text")
{
	m_rect.setSize(WinFontCalculate::instance()->orginalSize());
	m_strlist = m_text.split('\n');
}

void wbLinesText::drawElement(QPainter& p)
{
	int leading = WinFontCalculate::instance()->leading();
	int ascent = WinFontCalculate::instance()->ascent();
	int descent = WinFontCalculate::instance()->descent();
	int height = ascent + descent;
	int x = m_rect.x();
	int y = m_rect.y();
	p.drawText(m_rect.x(), m_rect.y() + ascent + leading, m_strlist[0]);
	for (int i = 1; i < m_strlist.size(); ++i)
	{
		p.drawText(x, y + (i + 1) * height - descent, m_strlist[i]);
	}
}

void wbLinesText::adjust_rect()
{
	m_rect.setSize(WinFontCalculate::instance()->calculateTextRectSize(m_strlist));
}

ElementWidgetBase* wbLinesText::createEditor()
{
	LinesTextEditMainWidget* ret = new LinesTextEditMainWidget;
	ret->setValue(this);
	return ret;
}

QString wbLinesText::JsInstructionForWinBase()
{
	return QString("drawTextEx(\'%1\',%2,%3);\n").arg(m_strlist.join("\\n"))
		.arg(this->rect().x()-ConfigureInfoContainer::instance()->window_configure()._textPadding)
		.arg(this->rect().y() - ConfigureInfoContainer::instance()->window_configure()._standardPadding);
}

void wbLinesText::Xml_SaveElement(QDomDocument& doc, QDomElement& parent)
{
	QDomElement this_node = doc.createElement("wbLinesText");
	//Rect
	QDomElement rect_node = doc.createElement("rect");
	rect_node.setAttribute("x", this->rect().x());
	rect_node.setAttribute("y", this->rect().y());
	rect_node.setAttribute("width", this->rect().width());
	rect_node.setAttribute("height", this->rect().height());
	this_node.appendChild(rect_node);
	//--------
	//TestList
	QDomElement textlist_node = doc.createElement("textlist");
	QDomElement text;
	for (int i = 0; i < this->m_strlist.size(); ++i)
	{
		text = doc.createElement(QString("text%1").arg(i));
		text.appendChild(doc.createTextNode(m_strlist[i]));
		textlist_node.appendChild(text);
	}
	this_node.appendChild(textlist_node);
	//---------
	parent.appendChild(this_node);
}

void wbLinesText::Xml_LoadElement(QDomElement& element)
{
	//Rect
	QDomElement rect_node = element.firstChildElement();
	m_rect.setRect(rect_node.attribute("x").toInt(), rect_node.attribute("y").toInt(), rect_node.attribute("width").toInt(), rect_node.attribute("height").toInt());
	//TextList
	QDomElement textlist_node = rect_node.nextSiblingElement();
	QDomElement text = textlist_node.firstChildElement();
	m_strlist.clear();
	while (!text.isNull())
	{
		m_strlist.push_back(text.firstChild().nodeValue());
		text = text.nextSiblingElement();
	}
	m_text = m_strlist.join('\n');
}

void wbLinesText::setText(QString& text)
{
	m_text = text;
	m_strlist = m_text.split('\n');
}
