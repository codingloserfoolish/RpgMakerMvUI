#ifndef RPGSCENE_H_
#define RPGSCENE_H_
#include"GameObject.h"
#include"../Manager/PixmapNode.h"
#include"../Manager/PixmapManager.h"
#include"../Manager/JsImageReserveContainer.h"
class RpgScene :public GameObject
{
	Q_OBJECT
	Q_PROPERTY(PixmapNode background READ R_BACKGROUND WRITE S_BACKGROUND)
public:
	Q_INVOKABLE PixmapNode R_BACKGROUND() { if (m_background) { m_send._name = m_background->_name; return m_send; }return PixmapNode(); }
	Q_INVOKABLE void S_BACKGROUND(PixmapNode& name) { m_background = PixmapManager::instance()->loadPixmap(name._name); }

	RpgScene(int width, int height, QObject* parent = nullptr);
	void CreateObject(GameObject* parent, int x, int y, int type);

	virtual void Js_Context(QTextStream&)override;

	virtual RpgObjectEditorBase* createEditor()override { return 0; }
protected:
	virtual void draw_self(QPainter& p)override;
	virtual QString Js_NewObject()override;
	virtual QString Js_AttributeSet()override;

	virtual QDomElement Xml_SaveData(QDomDocument& doc, QDomElement& parent_node)override;
	virtual QDomNode Xml_LoadData(QDomNode&)override;

	PixmapNode m_send;
	Pixmap* m_background;
};
#endif // !RPGSCENE_H_
