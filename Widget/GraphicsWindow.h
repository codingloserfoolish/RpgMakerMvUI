#ifndef GRAPHICSWINDOW_H_
#define GRAPHICSWINDOW_H_
#include<qwidget.h>
#include"../QtPropertyBrowser/qtvariantproperty.h"
#include<qfile.h>
#include<qtextstream.h>
#include<qmetaobject.h>

#include"MenuExtend.h"

#include"../RpgObject/RpgScene.h"
class GraphicsWindow :public QWidget
{
	Q_OBJECT
public:
	GraphicsWindow(QWidget* parent = nullptr);

	void initContextPopMenu();

	GameObject* root() { return &m_scene; }

	QString Jsfile_name() { return "./result/"+m_scene.objectName() + ".txt"; }
	QString Xmlfile_name() { return "./result/" + m_scene.objectName() + ".xml"; }

	void generateJs();
	void saveXml();
signals:
	void gameObjectBehavior(int behivor,QObject*obj);
	
public slots:
	void slot_mousePopMenuEvent(const QPoint& pos);
	void slot_gameObjectChanged(GameObject*active);
	void slot_gameObjectPropertyChanged(QtProperty*,const QVariant&);
	void slot_generateObject(int type);
	void slot_responseTriggerPopMenuEvent(int behavior,int param);
	void slot_Save();
	void slot_Load();
protected:
	virtual void paintEvent(QPaintEvent* event)override;
	virtual void mousePressEvent(QMouseEvent* event)override;
	virtual void mouseMoveEvent(QMouseEvent* event)override;

	QPointF m_diff;
	GameObject* m_active_object;
	RpgScene m_scene;
private:
	ContextPopMenu* m_pCtMenu;
	QPoint m_menuPos;
};

#endif // !GRAPHICSWINDOW_H_
