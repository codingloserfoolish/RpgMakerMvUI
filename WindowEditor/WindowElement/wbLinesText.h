#ifndef LINESTEXT_H_
#define LINESTEXT_H_
#include"WinElementBase.h"
class wbLinesText :public WinElementBase
{
public:
	wbLinesText();
	virtual void drawElement(QPainter& p)override;
	virtual int we_type()const override { return C_LINESTEXT; }
	virtual void adjust_rect()override;
	virtual ElementWidgetBase* createEditor()override;
	virtual QString JsInstructionForWinBase()override;
	virtual void Xml_SaveElement(QDomDocument& doc, QDomElement& parent)override;
	virtual void Xml_LoadElement(QDomElement& element)override;
	QString& text() { return m_text; }
	void setText(QString&text);
protected:
	QString m_text;
	QStringList m_strlist;
};
#endif // !LINESTEXT_H_
