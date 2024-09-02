#include"wbBitmap.h"
#include<qdom.h>
#include"../ElementWidget/BitmapEditWidget/BitmapEditWidget.h"
#include"../../Manager/JsImageReserveContainer.h"
wbBitmap::wbBitmap():m_pixmap(0)
{
	m_rect.setRect(0, 0, 100, 100);
}

void wbBitmap::drawElement(QPainter& p)
{
	if (!m_pixmap)
	{
		p.drawRect(m_rect);
		return;
	}
	p.drawPixmap(m_rect, m_pixmap->_pixmap, this->real_frame());
}

ElementWidgetBase* wbBitmap::createEditor()
{
	BitmapEditMainWidget* w = new BitmapEditMainWidget;
	w->setValue(this);
	return w;
}

QString wbBitmap::JsInstructionForWinBase()
{
	QString main_ins = "drawPixmap(";
	QString src = this->rectToString(this->real_frame());
	QString des = this->desRectToString(this->rect());
	SplitImgFloder*item = JsImageReserveContainer::instance()->addFloder(m_pixmap->_name);
	return main_ins+ des +','+ item->_floder+','+ item->_filename+',' + src + ");\n";
}

void wbBitmap::Xml_SaveElement(QDomDocument& doc, QDomElement& parent)
{
	if (!m_pixmap)return;
	QDomElement this_node = doc.createElement("wbBitmap");
	//Rect
	QDomElement rect_node = doc.createElement("rect");
	rect_node.setAttribute("x", this->rect().x());
	rect_node.setAttribute("y", this->rect().y());
	rect_node.setAttribute("width", this->rect().width());
	rect_node.setAttribute("height", this->rect().height());
	this_node.appendChild(rect_node);
	//------
	//Frame
	QDomElement frame_node = doc.createElement("frame");
	frame_node.setAttribute("x", this->frame().x());
	frame_node.setAttribute("y", this->frame().y());
	frame_node.setAttribute("width", this->frame().width());
	frame_node.setAttribute("height", this->frame().height());
	this_node.appendChild(frame_node);
	//------
	//Bitmap
	QDomElement bitmap_node = doc.createElement("bitmap");
	bitmap_node.setAttribute("name", this->m_pixmap->_name);
	this_node.appendChild(bitmap_node);
	//------

	parent.appendChild(this_node);
}

void wbBitmap::Xml_LoadElement(QDomElement& element)
{
	//Rect
	QDomElement rect_node = element.firstChildElement();
	m_rect.setRect(rect_node.attribute("x").toInt(), rect_node.attribute("y").toInt(), rect_node.attribute("width").toInt(), rect_node.attribute("height").toInt());
	//Frame
	QDomElement frame_node = rect_node.nextSiblingElement();
	m_frame.setRect(frame_node.attribute("x").toInt(), frame_node.attribute("y").toInt(), frame_node.attribute("width").toInt(), frame_node.attribute("height").toInt());
	//Bitmap
	QDomElement bitmap_node = frame_node.nextSiblingElement();
	m_pixmap = PixmapManager::instance()->loadPixmap(bitmap_node.attribute("name"));
}

void wbBitmap::setPixmap(QString& name)
{
	m_pixmap = PixmapManager::instance()->loadPixmap(name);
	if (m_pixmap)
	{
		m_rect.setSize(QSize(m_pixmap->width(),m_pixmap->height()));
		m_frame.setRect(0,0,m_rect.width(),m_rect.height());
	}
}

QRect wbBitmap::real_frame()
{
	int x = clamp(0, this->m_pixmap->width(), m_frame.x());
	int y = clamp(0, this->m_pixmap->height(), m_frame.y());
	int width = clamp(0, this->m_pixmap->width(), m_frame.width() + m_frame.x()) - x;
	int height = clamp(0, this->m_pixmap->height(), m_frame.height() + m_frame.y()) - y;
	return QRect(x, y, width, height);
}

int wbBitmap::clamp(int min, int max,int value)
{
	if (value < min)return min;
	else if (value > max)return max;
	else return value;
}

QString wbBitmap::rectToString(QRect& rect)
{
	return QString("%1,%2,").arg(rect.x()).arg(rect.y()) + QString("%1,%2").arg(rect.width()).arg(rect.height());
}

QString wbBitmap::desRectToString(QRect& rect)
{
	return QString("%1,%2,")
		.arg(rect.x() - ConfigureInfoContainer::instance()->window_configure()._textPadding)
		.arg(rect.y() - ConfigureInfoContainer::instance()->window_configure()._standardPadding)
		+ QString("%1,%2").arg(rect.width()).arg(rect.height());
}
