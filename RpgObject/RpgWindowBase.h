#ifndef RPGWINDOW_H_
#define RPGWINDOW_H_
#include"GameObject.h"
#include"../Manager/PixmapManager.h"

class RpgWindow:public GameObject
{
	Q_OBJECT
	Q_PROPERTY(bool CheckMessage READ R_IF_CHECK_MESSAGE WRITE S_IF_CHECK_MESSAGE)
public:
	Q_INVOKABLE bool R_IF_CHECK_MESSAGE() { return m_if_check_message; }
	Q_INVOKABLE void S_IF_CHECK_MESSAGE(bool value) { m_if_check_message = value; }

	explicit RpgWindow(float width,float height,QObject*parent=nullptr);
protected:

	virtual void draw_self(QPainter& p)override;
	virtual QString Js_NewObject()override;
	virtual QString Js_AttributeSet()override;
	virtual QDomElement Xml_SaveData(QDomDocument& doc,QDomElement& parent_node)override;
	virtual QDomNode Xml_LoadData(QDomNode& self)override;

	bool m_if_check_message;
	Pixmap* m_system_window;
};
#endif // !RPGWINDOW_H_
