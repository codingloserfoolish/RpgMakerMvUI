#include"RpgSprite.h"

RpgSprite::RpgSprite(QObject* parent):
	GameObject(128,128,parent),
	m_bitmap(0),
	m_if_check_message(false),
	m_frame_rect(0,0,128,128)
{

}

void RpgSprite::setBitmap(QString& name)
{
	m_bitmap=PixmapManager::instance()->loadPixmap(name);
	if (m_bitmap)
	{
		this->m_canvas_width = m_bitmap->width();
		this->m_canvas_height = m_bitmap->height();
		m_frame_rect.setSize(QSize(this->m_canvas_width, this->m_canvas_height));
	}
}

RpgObjectEditorBase* RpgSprite::createEditor()
{
	return 0;
}

int RpgSprite::clamp(int value, int min, int max)
{
	if (min > value)return min;
	else if (max < value)return max;
	else return value;
}



void RpgSprite::draw_self(QPainter& p)
{
	if (m_bitmap)
	{
		int s_x = clamp(m_frame_rect.x(), 0, this->m_bitmap->width());
		int s_y = clamp(m_frame_rect.y(), 0, this->m_bitmap->height());
		int s_width = clamp(m_frame_rect.width() + m_frame_rect.x(), 0, this->m_bitmap->width()) - s_x;
		int s_height = clamp(m_frame_rect.height() + m_frame_rect.y(), 0, this->m_bitmap->height()) - s_y;
		p.drawPixmap(-m_canvas_width/2,-m_canvas_height/2,m_canvas_width, m_canvas_height,
			m_bitmap->_pixmap,
			s_x,s_y,s_width,s_height);
	}
	else
	{
		p.setBrush(QColor(Qt::black));
		p.drawRect(-this->m_canvas_width / 2, -this->m_canvas_height / 2, this->m_canvas_width, this->m_canvas_height);
	}
}

QString RpgSprite::Js_NewObject()
{
	int index = this->m_bitmap->_name.lastIndexOf('/');
	QString floder = "img/" + this->m_bitmap->_name.left(index+1);
	QString name = this->m_bitmap->_name.right(this->m_bitmap->_name.length()-index-1);
	return QString("this.%1=new hjdd.SpriteEx(ImageManager.loadBitmap('%2','%3',0,true));\n").arg(this->objectName()).arg(floder).arg(name);
}

QString RpgSprite::Js_AttributeSet()
{
	QString var = QString("this.%1.").arg(this->objectName());
	QString geometry = var + QString("setGeometry(%1,%2,%3,%4);\n").arg(CORRECT_JS_COOR_X(m_x)).arg(CORRECT_JS_COOR_Y(m_y)).arg(m_canvas_width).arg(m_canvas_height);
	QString frameRect = var + QString("setFrame(%1,%2,%3,%4);\n").arg(this->m_frame_rect.x()).arg(this->m_frame_rect.y()).arg(this->m_frame_rect.width()).arg(this->m_frame_rect.height());
	QString rotation = var + QString("rotate(%1);\n").arg(m_angle * PI / 180.0f);
	QString s_parent = var + QString("setParent(%1);\n").arg(m_parent->gameobject_name());
	QString check_message;
	if (m_if_check_message)
	{
		check_message = var + QString("if_check_message=true;\n");
	}
	return  geometry + frameRect + rotation + s_parent + check_message;
}

QDomElement RpgSprite::Xml_SaveData(QDomDocument& doc, QDomElement& parent_node)
{
	QDomElement Sprite_Node = doc.createElement("RpgSprite");
	//name
	Sprite_Node.setAttribute("name", this->objectName());
	Sprite_Node.setAttribute("checkMessage", this->m_if_check_message);
	//Transform
	QDomElement c_transform = doc.createElement("Transform");
	c_transform.setAttribute("x", this->m_x);
	c_transform.setAttribute("y", this->m_y);
	c_transform.setAttribute("angle", this->m_angle);
	Sprite_Node.appendChild(c_transform);

	//Size
	QDomElement c_size = doc.createElement("Size");
	c_size.setAttribute("width", this->m_canvas_width);
	c_size.setAttribute("height", this->m_canvas_height);
	Sprite_Node.appendChild(c_size);

	//Bitmap
	QDomElement c_bitmap = doc.createElement("Bitmap");
	c_bitmap.setAttribute("name", this->m_bitmap->_name);
	//FrameRect
	QDomElement c_frame = doc.createElement("FrameRect");
	c_frame.setAttribute("x", this->m_frame_rect.x());
	c_frame.setAttribute("y", this->m_frame_rect.y());
	c_frame.setAttribute("width", this->m_frame_rect.width());
	c_frame.setAttribute("height", this->m_frame_rect.height());
	c_bitmap.appendChild(c_frame);
	Sprite_Node.appendChild(c_bitmap);

	parent_node.appendChild(Sprite_Node);
	return Sprite_Node;
}

QDomNode RpgSprite::Xml_LoadData(QDomNode& self)
{
	QDomElement Sprite_Node = self.toElement();
	//name
	this->setObjectName(Sprite_Node.attribute("name"));
	m_bind_standardItem->setText(this->objectName());
	this->m_if_check_message = Sprite_Node.attribute("checkMessage").toInt();
	//Transform
	QDomElement c_transform = Sprite_Node.firstChildElement();
	this->m_x = c_transform.attribute("x").toFloat();
	this->m_y = c_transform.attribute("y").toFloat();
	this->m_angle = c_transform.attribute("angle").toFloat();

	//Size
	QDomElement c_size = c_transform.nextSiblingElement();
	this->set_canvas_width(c_size.attribute("width").toInt());
	this->set_canvas_height(c_size.attribute("height").toInt());

	//Bitmap
	QDomElement c_bitmap = c_size.nextSiblingElement();
	this->m_bitmap = PixmapManager::instance()->loadPixmap(c_bitmap.attribute("name"));
	//FrameRect
	QDomElement c_frame = c_bitmap.firstChildElement();
	this->m_frame_rect.setX(c_frame.attribute("x").toInt());
	this->m_frame_rect.setY(c_frame.attribute("y").toInt());
	this->m_frame_rect.setWidth(c_frame.attribute("width").toInt());
	this->m_frame_rect.setHeight(c_frame.attribute("height").toInt());

	return c_bitmap.nextSibling();
}
