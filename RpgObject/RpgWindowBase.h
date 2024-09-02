#ifndef RPGWINDOW_H_
#define RPGWINDOW_H_
#include"GameObject.h"
#include"../Manager/PixmapManager.h"
class WinElementBase;
class RpgWindow:public GameObject
{
	Q_OBJECT
	Q_PROPERTY(bool CheckMessage READ R_IF_CHECK_MESSAGE WRITE S_IF_CHECK_MESSAGE)
public:
	Q_INVOKABLE bool R_IF_CHECK_MESSAGE() { return m_if_check_message; }
	Q_INVOKABLE void S_IF_CHECK_MESSAGE(bool value) { m_if_check_message = value; }

	explicit RpgWindow(float width,float height,QObject*parent=nullptr);
	std::vector<WinElementBase*>& elements() { return m_window_elements; }
	virtual int gm_type()const override { return 0; }

	virtual RpgObjectEditorBase* createEditor()override;
protected:
	virtual void on_destroy()override;
	virtual void draw_self(QPainter& p)override;
	virtual QString Js_NewObject()override;
	virtual QString Js_AttributeSet()override;
	virtual void Js_ExtraData(QTextStream& stream)override;
	virtual QDomElement Xml_SaveData(QDomDocument& doc,QDomElement& parent_node)override;
	virtual QDomNode Xml_LoadData(QDomNode& self)override;

	void LoadElements(QDomElement&);
	bool m_if_check_message;
	Pixmap* m_system_window;
	std::vector<WinElementBase*>m_window_elements;
};
#endif // !RPGWINDOW_H_
