#include"RpgWindowBase.h"

RpgWindow::RpgWindow(float width, float height, QObject* parent):GameObject(width,height,parent)
{
	m_system_window=PixmapManager::instance()->loadPixmap("system/Window");
}

void RpgWindow::draw_self(QPainter& p)
{
	int pos_x = -m_canvas_width / 2;
	int pos_y = -m_canvas_height / 2;
	p.drawPixmap(pos_x, pos_y, 24, 24, m_system_window->_pixmap, 96, 0, 24, 24);
	p.drawPixmap(24+pos_x, pos_y, canvas_width() - 48, 24, m_system_window->_pixmap, 120, 0, 48, 24);
	p.drawPixmap(canvas_width()/2 - 24, pos_y, 24, 24, m_system_window->_pixmap, 168, 0, 24, 24);

	p.drawPixmap(canvas_width()/2 - 24, 24+pos_y, 24, canvas_height() - 48, m_system_window->_pixmap, 168, 24, 24, 48);
	p.drawPixmap(canvas_width()/2 - 24, canvas_height()/2 - 24, 24, 24, m_system_window->_pixmap, 168, 72, 24, 24);

	p.drawPixmap(24+pos_x, canvas_height()/2 - 24, canvas_width() - 48, 24, m_system_window->_pixmap, 120, 72, 48, 24);
	p.drawPixmap(pos_x, canvas_height()/2 - 24, 24, 24, m_system_window->_pixmap, 96, 72, 24, 24);
	p.drawPixmap(pos_x, 24+pos_y, 24, canvas_height() - 48, m_system_window->_pixmap, 96, 24, 24, 48);
}

QString RpgWindow::Js_NewObject()
{
	return QString("this.%1=new hjdd.Window_BaseEx();\n").arg(this->objectName());
}

QString RpgWindow::Js_AttributeSet()
{
	QString var = QString("this.%1.").arg(this->objectName());
	QString geometry = var+QString("setGeometry(%1,%2,%3,%4);\n").arg(CORRECT_JS_COOR_X(m_x)).arg(CORRECT_JS_COOR_Y(m_y)).arg(m_canvas_width).arg(m_canvas_height);
	QString rotation = var+QString("rotate(%1);\n").arg(m_angle*PI/180.0f);
	QString s_parent = var + QString("setParent(%1);\n").arg(m_parent->gameobject_name());
	QString check_message;
	if (m_if_check_message)
	{
		check_message =var + QString("if_check_message=true;\n");
	}
	return  geometry + rotation+s_parent+check_message;
}

QDomElement RpgWindow::Xml_SaveData(QDomDocument& doc, QDomElement& parent_node)
{
	QDomElement Window_Node = doc.createElement("RpgWindow");
	//name
	Window_Node.setAttribute("name", this->objectName());
	Window_Node.setAttribute("checkMessage", this->m_if_check_message);
	//Transform
	QDomElement c_transform= doc.createElement("Transform");
	c_transform.setAttribute("x", this->m_x);
	c_transform.setAttribute("y", this->m_y);
	c_transform.setAttribute("angle", this->m_angle);
	Window_Node.appendChild(c_transform);

	//Size
	QDomElement c_size = doc.createElement("Size");
	c_size.setAttribute("width",this->m_canvas_width);
	c_size.setAttribute("height",this->m_canvas_height);
	Window_Node.appendChild(c_size);

	parent_node.appendChild(Window_Node);
	return Window_Node;
}

QDomNode RpgWindow::Xml_LoadData(QDomNode& self)
{
	QDomElement Window_Node = self.toElement();
	//name
	this->setObjectName(Window_Node.attribute("name"));
	m_bind_standardItem->setText(this->objectName());
	this->m_if_check_message= Window_Node.attribute("checkMessage").toInt();
	//Transform
	QDomElement c_transform = Window_Node.firstChildElement();
	this->m_x = c_transform.attribute("x").toFloat();
	this->m_y = c_transform.attribute("y").toFloat();
	this->m_angle = c_transform.attribute("angle").toFloat();

	//Size
	QDomElement c_size = c_transform.nextSiblingElement();
	this->set_canvas_width(c_size.attribute("width").toInt());
	this->set_canvas_height(c_size.attribute("height").toInt());

	return c_size.nextSibling();
}
