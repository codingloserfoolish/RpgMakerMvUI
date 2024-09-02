#ifndef RPGOBECTEDITOR_H_
#define RPGOBECTEDITOR_H_
#include<qwidget.h>
class GameObject;
class RpgObjectEditorBase:public QWidget
{
	Q_OBJECT
public:
	RpgObjectEditorBase(QWidget* parent = nullptr);
	void setObject(GameObject* obj) { m_item = obj; this->InitEditor(obj); }
	GameObject* item()const { return m_item; }
signals:
	void itemChanged();
protected:
	virtual void InitEditor(GameObject* item)=0;
private:
	GameObject* m_item;
};
#endif // !RPGOBECTEDITOR_H_
