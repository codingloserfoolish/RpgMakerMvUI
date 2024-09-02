#ifndef RPGCOMMANDWINDOW_H_
#define RPGCOMMANDWINDOW_H_
#include"GameObject.h"
#include"../Manager/PixmapManager.h"

class RpgCommandWindow :public GameObject
{
	Q_OBJECT
public:
	RpgCommandWindow(QObject*parent=nullptr);
	void setActiveIndex(int index) { m_active_index = index; }
	void setdisplayCount(int count);
	int activeIndex()const { return m_active_index; }
	int displayCout()const { return m_displayCount; }
	void appendCommand(const QString& command);
	void removeCommand(int index);
	void clearCommandList() { m_command_list.clear(); }
	virtual RpgObjectEditorBase* createEditor()override;
	QStringList& commandList(){ return m_command_list; }
	virtual int gm_type()const override { return 3; }
protected:
	virtual void draw_self(QPainter& p);
	virtual QString Js_NewObject();
	virtual QString Js_AttributeSet();

	virtual QDomElement Xml_SaveData(QDomDocument& doc, QDomElement& parent_node);
	virtual QDomNode Xml_LoadData(QDomNode& self);
private:
	void drawLineItem(QPainter&p,QString&text,int index);
	int m_displayCount;
	int m_item_height;
	int m_active_index;
	Pixmap* m_system_window;
	QStringList m_command_list;
};

#endif // !RPGCOMMANDWINDOW_H_
