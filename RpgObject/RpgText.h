#ifndef RPGTEXT_H_
#define RPGTEXT_H_
#include"GameObject.h"
#include"../Manager/PixmapManager.h"
#define COLOR_BITMAP PixmapManager::instance()->colorBitmap()
#define IVPIXMAP PixmapManager::instance()->IVPixmap()

class RpgText :public GameObject
{
	Q_OBJECT
	Q_PROPERTY(QColor outlineColor READ R_OUTLINECOLOR WRITE W_OUTLINECOLOR)
	Q_PROPERTY(int outlineWidth READ R_OUTLINEWIDTH WRITE W_OUTLINEWIDTH)
	Q_PROPERTY(bool italics READ R_ITALICS WRITE W_ITALICS)
public:
	Q_INVOKABLE QColor R_OUTLINECOLOR() { return m_outlinePen.color(); }
	Q_INVOKABLE void W_OUTLINECOLOR(QColor color) { m_outlinePen.setColor(color); }

	Q_INVOKABLE int R_OUTLINEWIDTH() { return m_outlinePen.width(); }
	Q_INVOKABLE void W_OUTLINEWIDTH(int width) { m_outlinePen.setWidth(width); }

	Q_INVOKABLE bool R_ITALICS() { return m_isItalics; }
	Q_INVOKABLE void W_ITALICS(bool value) { m_isItalics = value; }

	RpgText(QObject*parent=nullptr);
	void setText(QString&text);
	QString& text() { return m_text; }
	virtual int gm_type()const override { return 2; }
	virtual RpgObjectEditorBase* createEditor()override;
protected:
	virtual void draw_self(QPainter& p);
	virtual QString Js_NewObject();
	virtual QString Js_AttributeSet();

	virtual QDomElement Xml_SaveData(QDomDocument& doc, QDomElement& parent_node);
	virtual QDomNode Xml_LoadData(QDomNode& self);
private:
	bool isIcon(int* index,int*whichIcon);
	bool isVariable(int* index);
	bool isColor(int* index, int* whichColor);
	bool isFontSizeScaled(int *index,int*isNegative);
	void drawIcon(QPainter&p,int which,int x,int y);
	int drawSpecialText(QPainter&p,QString& text, int x, int y);
	int textWidth(QPainter&p,QString& text);
	int textMaxHeight(QPainter& p);
	void initPainter(QPainter& p);
	QString m_text;

	QPen m_outlinePen;
	bool m_isItalics;
};

#endif // !RPGTEXT_H_
