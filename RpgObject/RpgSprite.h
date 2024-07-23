#ifndef RPGSPRITE_H_
#define RPGSPRITE_H_
#include"GameObject.h"
#include"../Manager/PixmapNode.h"
#include"../Manager/PixmapManager.h"
class RpgSprite:public GameObject
{
	Q_OBJECT
	Q_PROPERTY(bool CheckMessage READ R_IF_CHECK_MESSAGE WRITE S_IF_CHECK_MESSAGE)
	Q_PROPERTY(PixmapNode bitmap READ R_BITMAP WRITE S_BITMAP)
	Q_PROPERTY(QRect frameSize READ R_FRAME_RECT WRITE S_FRAME_RECT)
public:
	Q_INVOKABLE bool R_IF_CHECK_MESSAGE() { return m_if_check_message; }
	Q_INVOKABLE void S_IF_CHECK_MESSAGE(bool value) { m_if_check_message = value; }

	Q_INVOKABLE PixmapNode R_BITMAP() { if (m_bitmap) { m_send._name = m_bitmap->_name; return m_send; }return PixmapNode(); }
	Q_INVOKABLE void S_BITMAP(PixmapNode& name) { this->setBitmap(name._name); }

	Q_INVOKABLE QRect& R_FRAME_RECT() { return m_frame_rect; }
	Q_INVOKABLE void S_FRAME_RECT(QRect& rect) { m_frame_rect = rect;}

	RpgSprite(QObject* parent = nullptr);
	void setBitmap(QString&name);
protected:
	int clamp(int value,int min,int max);

	virtual void draw_self(QPainter& p)override;
	virtual QString Js_NewObject()override;
	virtual QString Js_AttributeSet()override;

	virtual QDomElement Xml_SaveData(QDomDocument& doc, QDomElement& parent_node)override;
	virtual QDomNode Xml_LoadData(QDomNode& self)override;


	bool m_if_check_message;
	QRect m_frame_rect;
	Pixmap* m_bitmap;
	PixmapNode m_send;
};
#endif // !RPGSPRITE_H_
