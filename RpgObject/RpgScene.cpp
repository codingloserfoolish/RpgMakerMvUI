#include"RpgScene.h"
#include"RpgWindowBase.h"
#include"RpgSprite.h"
#include"RpgText.h"
#include"RpgCommandWindow.h"
RpgScene::RpgScene(int width, int height, QObject* parent):
	GameObject(width,height,parent),
	m_background(0)
{
	this->translate(width / 2, height / 2);
}

void RpgScene::CreateObject(GameObject* parent, int x, int y, int type)
{
	GameObject* item = 0;
	//GameObject* owner = parent ? parent : this;
	if (type == 0) { item = new RpgWindow(100, 100); }
	else if (type == 1) { item = new RpgSprite; }
	else if (type == 2) { item = new RpgText; }
	else if (type == 3) {item = new RpgCommandWindow;}
	if (item)
	{
		item->translate(x, y);
		parent->addChild(item);
	}
}

void RpgScene::Js_Context(QTextStream&textStream)
{
	QString SetUi = QString("%1.prototype.setupUi=function(){\n").arg(this->objectName());
	textStream << Js_NewObject();
	textStream << SetUi;
	textStream << Js_AttributeSet();
	for (GameObject* child : m_children)
	{
		child->Js_Context(textStream);
	}
	textStream << "}\n";
	textStream << QString("%1.prototype.preReserveBitmap=function(){\n").arg(this->objectName());
	JsImageReserveContainer::instance()->JsPreReserveBitmapWriter(textStream);
	textStream << "}\n";
}



void RpgScene::draw_self(QPainter& p)
{
	if (m_background)
	{
		p.drawPixmap(-canvas_width() / 2, -canvas_height() / 2, canvas_width(), canvas_height(), 
			m_background->_pixmap,
			0,0,m_background->width(),m_background->height());
	}
	else
	{
		p.drawRect(-canvas_width() / 2 + 2, -canvas_height() / 2 + 2, canvas_width() - 4, canvas_height() - 4);
	}
}

QString RpgScene::Js_NewObject()
{
	QString ClassProto = QString("function %1(){this.initialize.apply(this,arguments);}\n").arg(this->objectName());
	QString Inherit = QString("%1.prototype=Object.create(hjdd.MainScene.prototype);\n"
		"%1.prototype.constructor=%1;\n").arg(this->objectName());
	QString Initialize = QString("%1.prototype.initialize=function(){hjdd.MainScene.prototype.initialize.call(this);}\n").arg(this->objectName());
	return ClassProto + Inherit + Initialize;
}

QString RpgScene::Js_AttributeSet()
{
	QString background = "";
	if (m_background)
	{
		int index = this->m_background->_name.lastIndexOf('/');
		QString floder = "img/" + this->m_background->_name.left(index + 1);
		QString name = this->m_background->_name.right(this->m_background->_name.length() - index - 1);
		background = QString("this.createBackground('%1','%2');\n").arg(floder).arg(name);
	}
	QString geometry = QString("this.setGeometry(%1,%2,%3,%4);\n").arg(m_x+8).arg(m_y+12).arg(m_canvas_width+16).arg(m_canvas_height+24);
	QString rotation = QString("this.rotate(%1);\n").arg(m_angle * PI / 180.0f);
	return  background+geometry + rotation;
}

QDomElement RpgScene::Xml_SaveData(QDomDocument& doc, QDomElement& parent_node)
{
	QDomElement Scene_Node = doc.createElement("RpgScene");
	//background
	if(m_background)
	Scene_Node.setAttribute("background", this->m_background->_name);
	//Transform
	QDomElement c_transform = doc.createElement("Transform");
	c_transform.setAttribute("x", this->m_x);
	c_transform.setAttribute("y", this->m_y);
	c_transform.setAttribute("angle", this->m_angle);
	Scene_Node.appendChild(c_transform);

	//Size
	QDomElement c_size = doc.createElement("Size");
	c_size.setAttribute("width", this->m_canvas_width);
	c_size.setAttribute("height", this->m_canvas_height);
	Scene_Node.appendChild(c_size);

	if (!parent_node.isNull())
		parent_node.appendChild(Scene_Node);
	else
		doc.appendChild(Scene_Node);
	return Scene_Node;
}

QDomNode RpgScene::Xml_LoadData(QDomNode&self)
{
	this->_object_name_index = 1;
	QDomElement Scene_Node = self.toElement();
	//background
	m_background = PixmapManager::instance()->loadPixmap(Scene_Node.attribute("background"));
	//Transform
	QDomElement c_transform = Scene_Node.firstChildElement();
	this->m_x = c_transform.attribute("x").toFloat();
	this->m_y= c_transform.attribute("y").toFloat();
	this->m_angle=c_transform.attribute("angle").toFloat();

	//Size
	QDomElement c_size = c_transform.nextSiblingElement();
	this->set_canvas_width(c_size.attribute("width").toInt());
	this->set_canvas_height(c_size.attribute("height").toInt());

	return c_size.nextSibling();
}
