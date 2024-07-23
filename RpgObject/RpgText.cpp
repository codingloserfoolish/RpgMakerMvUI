#include"RpgText.h"
#include<qdebug.h>
RpgText::RpgText(QObject* parent):
	GameObject(49,36,parent),
	m_text("text"),
	m_label("text")
{
}

void RpgText::draw_self(QPainter& p)
{
	if (!m_text.isEmpty())
	{
		p.drawPixmap(-m_canvas_width / 2, -m_canvas_height / 2,m_label.grab(m_label.rect()));
	}
	//p.drawRect(-m_canvas_width/ 2, -m_canvas_height / 2, m_canvas_width-2, m_canvas_height-2);
}

QString RpgText::Js_NewObject()
{
	QFont& _font = m_label.font();
	QString font = QString("fontFamily: '%1',fontSize: %2,fontStyle : '%3',fontWeight : %4,").arg(_font.family()).arg(_font.pointSize()).arg(_font.italic()?"italic":"normal").arg(_font.weight());
	QString fill = QString("fill:%1,").arg(RGB24(m_label.contentColor()));
	QString stroke = QString("stroke:{color:%1,width:%2,join:'bevel'}").arg(RGB24(m_label.outline().color())).arg(m_label.outline().width());
	QString style = "{" + font + fill + stroke + "}";
	return QString("this.%1=new PIXI.Text('%2',").arg(this->objectName()).arg(m_text)+style+");\n";
}

QString RpgText::Js_AttributeSet()
{
	QString var = QString("this.%1.").arg(this->objectName());
	QString move = var + QString("move(%1,%2);\n").arg(CORRECT_JS_COOR_X(m_x)).arg(CORRECT_JS_COOR_Y(m_y));
	QString rotation = var + QString("rotate(%1);\n").arg(m_angle * PI / 180.0f);
	QString s_parent = var + QString("setParent(%1);\n").arg(m_parent->gameobject_name());
	return move+rotation+s_parent;
}

QDomElement RpgText::Xml_SaveData(QDomDocument& doc, QDomElement& parent_node)
{
	QDomElement Text_Node = doc.createElement("RpgText");
	//name
	Text_Node.setAttribute("name", this->objectName());
	//Transform
	QDomElement c_transform = doc.createElement("Transform");
	c_transform.setAttribute("x", this->m_x);
	c_transform.setAttribute("y", this->m_y);
	c_transform.setAttribute("angle", this->m_angle);
	Text_Node.appendChild(c_transform);
	//Size
	QDomElement c_size = doc.createElement("Size");
	c_size.setAttribute("width", this->m_canvas_width);
	c_size.setAttribute("height", this->m_canvas_height);
	Text_Node.appendChild(c_size);
	//Text
	QDomElement c_text_list = doc.createElement("Text");
	c_text_list.setAttribute("text", m_text);
	Text_Node.appendChild(c_text_list);
	//FontStyle
	QDomElement c_font = doc.createElement("FontStyle");
	c_font.setAttribute("family", m_label.font().family());
	c_font.setAttribute("pointSize", m_label.font().pointSize());
	c_font.setAttribute("weight", m_label.font().weight());
	c_font.setAttribute("italic", m_label.font().italic());
	Text_Node.appendChild(c_font);
	//fillColor
	QDomElement c_fillColor = doc.createElement("FillColor");
	c_fillColor.setAttribute("r", m_label.contentColor().red());
	c_fillColor.setAttribute("g", m_label.contentColor().green());
	c_fillColor.setAttribute("b", m_label.contentColor().blue());
	Text_Node.appendChild(c_fillColor);
	//strok
	QDomElement c_stroke = doc.createElement("Stroke");
	c_stroke.setAttribute("width", m_label.outline().width());
	c_stroke.setAttribute("r", m_label.outline().color().red());
	c_stroke.setAttribute("g", m_label.outline().color().green());
	c_stroke.setAttribute("b", m_label.outline().color().blue());
	Text_Node.appendChild(c_stroke);

	parent_node.appendChild(Text_Node);
	return Text_Node;

}

QDomNode RpgText::Xml_LoadData(QDomNode& self)
{
	QDomElement Text_Node = self.toElement();
	//name
	this->setObjectName(Text_Node.attribute("name"));
	m_bind_standardItem->setText(this->objectName());
	//Transform
	QDomElement c_transform = Text_Node.firstChildElement();
	this->m_x = c_transform.attribute("x").toFloat();
	this->m_y = c_transform.attribute("y").toFloat();
	this->m_angle = c_transform.attribute("angle").toFloat();
	//Size
	QDomElement c_size = c_transform.nextSiblingElement();
	this->set_canvas_width(c_size.attribute("width").toInt());
	this->set_canvas_height(c_size.attribute("height").toInt());
	//Text
	QDomElement c_text_list = c_size.nextSiblingElement();
	m_text = c_text_list.attribute("text");
	m_label.setContentTextWithAdjustSize(m_text);
	//FontStyle
	QDomElement c_font = c_text_list.nextSiblingElement();
	m_label.setFontWithAdjustSize(QFont(c_font.attribute("family"), c_font.attribute("pointSize").toInt(), c_font.attribute("weight").toInt(), c_font.attribute("italic").toInt()));
	resize_canvas(m_label.size());
	//fillColor
	QDomElement c_fillColor= c_font.nextSiblingElement();
	m_label.setContentColor(QColor(c_fillColor.attribute("r").toInt(), c_fillColor.attribute("g").toInt(), c_fillColor.attribute("b").toInt()));
	//stroke
	QDomElement c_stroke = c_fillColor.nextSiblingElement();
	m_label.setOutLineColor(QColor(c_stroke.attribute("r").toInt(), c_stroke.attribute("g").toInt(),c_stroke.attribute("b").toInt()));
	m_label.setOutLineWidth(c_stroke.attribute("width").toInt());

	return c_stroke.nextSibling();
}
