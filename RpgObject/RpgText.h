#ifndef RPGTEXT_H_
#define RPGTEXT_H_
#include"GameObject.h"
#include"../Widget/FontLabel.h"
#include<qdebug.h>
#define RGB24(color) color.red()<<16|color.green()<<8|color.blue()

class RpgText :public GameObject
{
	Q_OBJECT
	Q_PROPERTY(QString text READ R_TEXT WRITE S_TEXT)
	Q_PROPERTY(QFont fontStyle READ R_FONT_STYLE WRITE S_FONT_STYLE)
	Q_PROPERTY(QColor OutLineColor READ R_OUTLINECOLOR WRITE S_OUTLINECOLOR)
	Q_PROPERTY(QColor ContentColor READ R_CONTENTCOLOR WRITE S_CONTENTCOLOR)
	Q_PROPERTY(int OutLineWidth READ R_OUTLINEWIDTH WRITE S_OUTLINEWIDTH)
	
public:
	Q_INVOKABLE QString& R_TEXT() { return m_text; }
	Q_INVOKABLE void S_TEXT(QString& text) { m_text = text; m_label.setContentTextWithAdjustSize(text); resize_canvas(m_label.size()); }

	Q_INVOKABLE QFont& R_FONT_STYLE() { return m_label.font(); }
	Q_INVOKABLE void S_FONT_STYLE(QFont& font) { m_label.setFontWithAdjustSize(font); resize_canvas(m_label.size());}

	Q_INVOKABLE QColor& R_OUTLINECOLOR() { return m_label.outline().color(); }
	Q_INVOKABLE void S_OUTLINECOLOR(QColor&color) { m_label.setOutLineColor(color); }

	Q_INVOKABLE QColor& R_CONTENTCOLOR() { return m_label.contentColor(); }
	Q_INVOKABLE void S_CONTENTCOLOR(QColor& color) {m_label.setContentColor(color); }

	Q_INVOKABLE int R_OUTLINEWIDTH() { return m_label.outline().width(); }
	Q_INVOKABLE void S_OUTLINEWIDTH(int width) { m_label.setOutLineWidth(width); }


	RpgText(QObject* parent = nullptr);

	void resize_canvas(QSize& size) { m_canvas_width = size.width(); m_canvas_height = size.height(); }
protected:
	virtual void draw_self(QPainter& p)override;
	virtual QString Js_NewObject()override;
	virtual QString Js_AttributeSet()override;

	virtual QDomElement Xml_SaveData(QDomDocument& doc, QDomElement& parent_node)override;
	virtual QDomNode Xml_LoadData(QDomNode& self)override;

	FontLabel m_label;
	QString m_text;
};

#endif // !RPGTEXT_H_
