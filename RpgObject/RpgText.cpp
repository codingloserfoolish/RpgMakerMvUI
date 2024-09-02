#include"RpgText.h"
#include<qpainterpath.h>
#include"../Manager/ConfigureInfoContainer.h"
#include"../Manager/PathManager.h"
#include"../RpgObjectEditor/RpgTextEditor.h"

RpgText::RpgText(QObject* parent)
	:GameObject(196,36,parent),
	m_text("rpgtext")
{
	m_isItalics = false;
	m_outlinePen.setWidth(4);
	m_outlinePen.setColor(QColor(0, 0, 0, 127));
}

void RpgText::setText(QString& text)
{
	m_text = text;
}

RpgObjectEditorBase* RpgText::createEditor()
{
	return new RpgTextEditor;
}

void RpgText::draw_self(QPainter& p)
{
	this->initPainter(p);
	QString textToDraw;
	int s_x = -m_canvas_width/2;
	int s_y = -m_canvas_height/2;
	int max_height= p.fontMetrics().height();
	int which_Icon = -1;
	int which_Color = -1;
	int end_index = 0;;
	int isNegative = -1;
	QChar symbol;
	for (int i = 0; i < m_text.size(); ++i)
	{
		symbol =m_text[i];
		if (symbol == '\\')
		{
			end_index = i;
			if (this->isIcon(&end_index,&which_Icon))
			{
				i = end_index;
				if (!textToDraw.isEmpty())
				{
					s_x += this->drawSpecialText(p, textToDraw, s_x, s_y);
					textToDraw.clear();
				}
				this->drawIcon(p,which_Icon, s_x, s_y);
				s_x += 32;
			}
			else if (this->isColor(&end_index, &which_Color))
			{
				i = end_index;
				if (!textToDraw.isEmpty())
				{
					s_x += this->drawSpecialText(p, textToDraw, s_x, s_y);
					textToDraw.clear();
				}
				if(which_Color!=-1)
				p.setPen(COLOR_BITMAP.pixelColor(96 + 12 * (which_Color % 8), 144 + 12 * (which_Color / 8)));
			}
			else if (this->isVariable(&end_index))
			{
				i = end_index;
				if (!textToDraw.isEmpty())
				{
					s_x += this->drawSpecialText(p, textToDraw, s_x, s_y);
					textToDraw.clear();
				}
				p.drawPixmap(s_x, s_y, IVPIXMAP, 0, 0, 32, 32);
				s_x += 32;
			}
			else if (this->isFontSizeScaled(&end_index,&isNegative))
			{
				i = end_index;
				if (!textToDraw.isEmpty())
				{
					s_x += this->drawSpecialText(p, textToDraw, s_x, s_y);
					textToDraw.clear();
				}
				QFont font = p.font();
				if (font.pixelSize() >= 24 && font.pixelSize() <= 96)
				{
					font.setPixelSize(font.pixelSize() + isNegative * 12);
					p.setFont(font);
					if (isNegative == 1 && p.fontMetrics().height() > max_height)
					{
						max_height = p.fontMetrics().height();
					}
				}
			}
			else
			{
				textToDraw += symbol;
			}

			if (i != end_index)
			{
				i = end_index;
				textToDraw += m_text.mid(i, end_index - i + 1);
			}
		}
		else if (symbol == '\n')
		{
			if (!textToDraw.isEmpty())
			{
				s_x += this->drawSpecialText(p, textToDraw, s_x, s_y);
				if (s_x > m_canvas_width)m_canvas_width = s_x;
				textToDraw.clear();
			}
			s_x = -m_canvas_width / 2;
			s_y += max_height;
			max_height = p.fontMetrics().height();
		}
		else
		{
			textToDraw += symbol;
		}
	}
	m_canvas_height = (s_y + max_height+m_canvas_height/2);
	if (!textToDraw.isEmpty())
	{
		s_x += this->drawSpecialText(p, textToDraw, s_x, s_y);
		if (s_x > m_canvas_width)m_canvas_width = s_x;
	}
}

QString RpgText::Js_NewObject()
{
	return QString("this.%1=new hjdd.Text(new Bitmap(%2,%3));\n").arg(this->objectName()).arg(m_canvas_width).arg(m_canvas_height);
}

QString RpgText::Js_AttributeSet()
{
	QString var = QString("this.%1.").arg(this->objectName());
	QString text = m_text;
	text.replace('\\', "\\\\");
	text.replace('\n', "\\n");
	QString draw = var+"setText(\'" + text + "\');\n";
	QString display = var+QString("displayLinesText(%1,%2);\n").arg(CORRECT_JS_COOR_X(m_x)).arg(CORRECT_JS_COOR_Y(m_y));
	QString setParent = var + "setParent(this);\n";

	return draw + display+setParent;
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
	QDomElement c_textlist = doc.createElement("TextList");
	QDomElement c_text;
	QStringList list = m_text.split('\n');
	for (int i=0;i<list.size();++i)
	{
		c_text = doc.createElement(QString("Text%1").arg(i));
		c_text.appendChild(doc.createTextNode(list[i]));
		c_textlist.appendChild(c_text);
	}
	Text_Node.appendChild(c_textlist);

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
	QStringList container;
	QDomElement c_textlist = c_size.nextSiblingElement();
	QDomElement c_text = c_textlist.firstChildElement();
	while (!c_text.isNull())
	{
		container.push_back(c_text.firstChild().nodeValue());
		c_text = c_text.nextSiblingElement();
	}
	m_text = container.join('\n');

	return c_textlist.nextSibling();
}

bool RpgText::isIcon(int* index, int* whichIcon)
{
	int this_index = 1+*index;
	if ("I["!=m_text.mid(this_index,2))return false;
	this_index += 2;
	QString icon_number;
	while (this_index < m_text.size())
	{
		if (m_text[this_index] >= '0' && m_text[this_index] <= '9') { icon_number += m_text[this_index]; ++this_index; }
		else if(icon_number.isEmpty()&& m_text[this_index]=='\\'&& this->isVariable(&this_index))++this_index;
		else if (m_text[this_index] == ']') { *whichIcon = icon_number.toInt(); *index=this_index; return true; }
		else { *index = this_index; return false; }
	}
	*index = this_index;
	return false;
}

bool RpgText::isVariable(int* index)
{
	int this_index = 1 + *index;
	if ("V[" != m_text.mid(this_index, 2))return false;
	this_index+=2;
	while (this_index < m_text.size())
	{
		if (m_text[this_index] >= '0' && m_text[this_index] <= '9')++this_index;
		else if (this_index == (3 + *index) && m_text[this_index] == '\\' && this->isVariable( &this_index))++this_index;
		else if (m_text[this_index] == ']') { *index = this_index; return true; }
		else { *index = this_index; return false; }
	}
	*index = this_index;
	return false;
}

bool RpgText::isColor(int* index, int* whichColor)
{
	int this_index = 1 + *index;
	if ("C[" != m_text.mid(this_index, 2))return false;
	this_index += 2;
	QString color_number;
	while (this_index < m_text.size())
	{
		if (m_text[this_index] >= '0' && m_text[this_index] <= '9') { color_number += m_text[this_index]; ++this_index; }
		else if (color_number.isEmpty() && m_text[this_index] == '\\'&& this->isVariable(&this_index))++this_index;
		else if (m_text[this_index] == ']') { *whichColor = color_number.toInt(); *index = this_index; return true; }
		else { *index = this_index; return false; }
	}
	*index = this_index;
	return false;
}

bool RpgText::isFontSizeScaled(int* index,int*isNegative)
{
	int this_index = 1 + *index;
	if (m_text[this_index] == '}')
	{
		*isNegative = -1;
	}
	else if (m_text[this_index] == '{')
	{
		*isNegative = 1;
	}
	else
	{
		return false;
	}
	*index = this_index;
	return true;
}

void RpgText::drawIcon(QPainter& p,int which, int x, int y)
{
	if (which == -1)
	{
		p.drawPixmap(x, y, IVPIXMAP, 32, 0, 32, 32);
	}
	else
	{
		int s_x = (which % 16)*32;
		int s_y = 32 * (which / 16);
		p.drawPixmap(x, y, PixmapManager::instance()->loadPixmap("system/IconSet")->_pixmap, s_x, s_y, 32, 32);
	}
}

int RpgText::drawSpecialText(QPainter& p, QString& text, int x, int y)
{
	int s_y = y + p.fontMetrics().height() - p.fontMetrics().descent();
	QPainterPath path;
	path.addText(x,s_y,p.font(),text);
	p.strokePath(path, m_outlinePen);
	p.drawPath(path);
	p.fillPath(path,p.pen().color());
	return p.fontMetrics().width(text);
}

int RpgText::textWidth(QPainter& p, QString& text)
{
	const QFontMetrics metrics= p.fontMetrics();
	return metrics.width(text);
}

int RpgText::textMaxHeight(QPainter& p)
{
	const QFontMetrics metrics = p.fontMetrics();
	return metrics.height()+metrics.leading();
}

void RpgText::initPainter(QPainter& p)
{
	QFont font = ConfigureInfoContainer::instance()->gameFont();
	font.setItalic(m_isItalics);
	p.setFont(font);
	p.setPen(QColor(255, 255, 255));
}
