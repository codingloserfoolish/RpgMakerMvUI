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
	return QDomElement();
}

QDomNode RpgText::Xml_LoadData(QDomNode& self)
{
	return QDomNode();
}
