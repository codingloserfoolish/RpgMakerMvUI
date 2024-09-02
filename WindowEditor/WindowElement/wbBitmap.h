#ifndef BITMAP_H_
#define BITMAP_H_
#include"WinElementBase.h"
#include"../../Manager/PixmapManager.h"
class wbBitmap :public WinElementBase
{
public:
	wbBitmap();
	virtual void drawElement(QPainter& p)override;
	virtual int we_type()const override { return C_BITMAP; }
	virtual ElementWidgetBase* createEditor()override;
	virtual QString JsInstructionForWinBase()override;
	virtual void Xml_SaveElement(QDomDocument& doc, QDomElement& parent)override;
	virtual void Xml_LoadElement(QDomElement& element)override;
	void setPixmap(QString&name);
	Pixmap* bitmap()const { return m_pixmap; }
	QRect& frame() { return m_frame; }
	QRect real_frame();
	int clamp(int min, int max,int value);
private:
	QString rectToString(QRect& rect);
	QString desRectToString(QRect& rect);
protected:
	Pixmap* m_pixmap;
	QRect m_frame;
	QRect m_real_frame;
};
#endif // !BITMAP_H_
