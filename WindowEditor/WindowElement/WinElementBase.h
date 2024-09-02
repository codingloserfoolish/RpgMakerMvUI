#ifndef WINELEMENTBASE_H_
#define WINELEMENTBASE_H_
#include<qrect.h>
#include<qpainter.h>
#include"../../Manager/ConfigureInfoContainer.h"
class QDomDocument;
class QDomElement;
class ElementWidgetBase;
enum WinElementType
{
	C_UNKNOW,
	C_LINESTEXT,
	C_BITMAP
};
class WinElementBase
{
public:
	virtual void drawElement(QPainter&p) = 0;
	virtual ElementWidgetBase* createEditor() = 0;
	virtual QString JsInstructionForWinBase() = 0;
	virtual void Xml_SaveElement(QDomDocument&doc,QDomElement& parent)=0;
	virtual void Xml_LoadElement(QDomElement&element)=0;
	virtual void adjust_rect() {}
	virtual int we_type()const { return C_UNKNOW; }
	bool contain(const QPoint& point) { return m_rect.contains(point); }
	QPoint position() { return m_rect.topLeft(); }
	void move(const QPoint& pos) { m_rect.moveTo(pos); }
	QRect& rect() { return m_rect; }
protected:
	QRect m_rect;
};
#endif // !WINELEMENTBASE_H_
