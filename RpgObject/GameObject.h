#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include<qpainter.h>
#include<qtextstream.h>
#include<vector>
#include<qxml.h>
#include<qdom.h>
#include<qstandarditemmodel.h>

#include"../MyTransform/Transform2D.h"

class GameObject;
class RpgObjectEditorBase;
struct GameObject_Data
{
	GameObject* _gameObject;
	GameObject_Data():_gameObject(0) {};
	GameObject_Data(GameObject* self) { _gameObject = self; }
	GameObject* gameObject()const { return _gameObject; }
};
Q_DECLARE_METATYPE(GameObject_Data)
#define GAMEOBJECT_ROLE Qt::UserRole+1


#define GAMEOBJECT_DEACTIVE 0
#define GAMEOBJECT_ACTIVE 1
#define GAMEOBJECT_MOVE	2
#define GAMEOBJECT_CHANGE 4
#define GAMEOBJECT_NAME_CHANGE 8
#define GAMEOBJECT_PROPERTY_UPDATE 16

#define CORRECT_JS_COOR_X(x) x+this->m_parent->canvas_width()/2
#define CORRECT_JS_COOR_Y(y) y+this->m_parent->canvas_height()/2

class GameObject:public Transform2D
{
	Q_OBJECT
	Q_PROPERTY(QSize size READ R_CANVASE_SIZE WRITE S_CANVASE_SIZE)
public:
	Q_INVOKABLE QSize R_CANVASE_SIZE() { return QSize(m_canvas_width, m_canvas_height); }
	Q_INVOKABLE void S_CANVASE_SIZE(QSize size) { m_canvas_width = size.width(); m_canvas_height = size.height(); }

	explicit GameObject(int canvas_w, int canvas_h, QObject* parent = nullptr);
	virtual ~GameObject();

	void clear();

	void draw(QPainter&p);
	
	/*
	* 拾取物体,递归实现
	*/
	GameObject* pickup(float x, float y,QPointF*diff=0);
	Matrix3 global_getTransMatrixIn();//外部点转换为本物体的坐标
	Matrix3 global_getTransMatrixOut();//本物体坐标转换为最外的窗口坐标
	float global_angle();
	/*
	* 添加移除
	*/
	void addChild(GameObject* object);
	void removeChild(GameObject* object);


	/*
	* 转换和检查鼠标点是否选中
	*/
	virtual bool contain(QPointF pos);


	/*
	* 名称检查   原始object0
	*/
	//保证，向上
	QString guarantee_name_super(QString&name);
	//保证，向下
	QString guarantee_name_low(QString& name);
	//产生基本名字
	QString create_base_name();

	virtual void Js_Context(QTextStream&);
	void Xml_Save_Content(QDomDocument&doc,QDomElement& parent_node);
	void Xml_Load_Content(QDomNode&self);

	QString gameobject_name() { return !m_parent ? "this" : "this."+this->objectName(); };
	bool isActive()const { return m_active; }
	void setActive(bool active) { m_active = active; }
	int canvas_width()const { return m_canvas_width; }
	int canvas_height()const { return m_canvas_height; }
	void set_canvas_width(int value) { m_canvas_width = value; }
	void set_canvas_height(int value) { m_canvas_height = value; }

	QStandardItem* standard_item()const { return m_bind_standardItem; }
	size_t childrenCount()const { return m_children.size();}
	GameObject* at(int index) { return m_children.at(index); }
	GameObject* gm_parent()const { return m_parent; }

	virtual RpgObjectEditorBase* createEditor() = 0;
	virtual int gm_type()const { return -1; }//-1 scene,0 window,1 sprite,2 text,3 commandwindow
protected:
	virtual void on_destroy() {};

	virtual void draw_self(QPainter& p)=0;
	virtual QString Js_NewObject()=0;
	virtual QString Js_AttributeSet() = 0;
	virtual void Js_ExtraData(QTextStream&stream) {}

	virtual QDomElement Xml_SaveData(QDomDocument& doc,QDomElement& parent_node)=0;
	virtual QDomNode Xml_LoadData(QDomNode& self)=0;
	bool m_active;
	int m_canvas_width;
	int m_canvas_height;


	QStandardItem* m_bind_standardItem;
	GameObject* m_parent;
	std::vector<GameObject*> m_children;

	static int _object_name_index;
};
#endif // !GAMEOBJECT_H_
