#include"WindowBase.h"
#include"../WindowBaseEdit/ElementEditManager/ElementLoader.h"
#include"../WindowBaseEdit/ElementEditManager/ElementsFactory.h"
#include<QDebug>
namespace hjdd
{
	namespace item
	{
		manager::Pixmap* WindowBase::_winskin = 0;
		WindowBase::WindowBase(QGraphicsItem* parent)
			:GraphicsItem(parent),
			m_elements_canvas(0)
		{
			if (!_winskin)
			{
				_winskin=manager::ImageManager::instance()->loadImage("system/Window.png");
			}
		}
		WindowBase::WindowBase(int width, int height, QGraphicsItem* parent)
			:GraphicsItem(width,height,parent),
			m_elements_canvas(0)
		{
			if (!_winskin)
			{
				_winskin=manager::ImageManager::instance()->loadImage("system/Window.png");
			}
		}
		WindowBase::~WindowBase()
		{
			if (_winskin->_count)
			{		
				manager::ImageManager::instance()->releseImage(WindowBase::_winskin);
			}
			if (m_elements_canvas)delete m_elements_canvas;
			for (elements::Element* ele : m_elements)
			{
				delete ele;
			}
		}
		QDomElement WindowBase::OthersProperties(QDomDocument& dom)
		{
			if (m_elements.empty())return QDomElement();
			QDomElement elementsNode = dom.createElement("Elements");
			for (elements::Element* element : m_elements)
			{
				QDomElement elementNode = dom.createElement("Element");
				elementNode.setAttribute("dllname", manager::ElementLibraryLoader::instance()->typeToName(element->element_type()));
				//x,y属性
				QDomElement xNode = dom.createElement("x");
				QDomElement yNode = dom.createElement("y");
				xNode.appendChild(dom.createTextNode(QString::number(element->pos().x())));
				yNode.appendChild(dom.createTextNode(QString::number(element->pos().y())));
				//自定义属性
				QDomElement others = element->xmlSave(dom);

				//添加
				elementNode.appendChild(xNode);
				elementNode.appendChild(yNode);
				if (!others.isNull())
				{
					elementNode.appendChild(others);
				}
				//加入
				elementsNode.appendChild(elementNode);
			}
			return elementsNode;
		}
		void WindowBase::OthersPropertiesLoad(QDomElement& node)
		{
			if (node.isNull())return;
			QDomElement element = node.firstChildElement();
			int x = 0, y = 0;
			while (!element.isNull())
			{
				int type = manager::ElementLibraryLoader::instance()->loadElementLib(element.attribute("dllname"));
				elements::Element* ele = manager::ElementsFactory::instance()->create(type);
				QDomElement next = element.firstChildElement();
				while (!next.isNull())
				{
					if (next.tagName() == "x")
					{
						x = next.firstChild().nodeValue().toInt();
					}
					else if (next.tagName() == "y")
					{
						y = next.firstChild().nodeValue().toInt();
					}
					else
					{
						ele->xmlLoad(next);
					}
					next = next.nextSiblingElement();
				}
				ele->setPos(x, y);
				m_elements.push_back(ele);
				element = element.nextSiblingElement();
			}
			this->refreshElementsCanvas();
		}
		void WindowBase::refreshElementsCanvas()
		{
			if (!m_elements_canvas)m_elements_canvas = new QPixmap(this->width()-36, this->height()-36);
			else if (m_elements_canvas->width()+36 < this->width() || m_elements_canvas->height()+36 < this->height())
			{
				delete m_elements_canvas;
				m_elements_canvas = new QPixmap(this->width()-36, this->height()-36);
			}

			m_elements_canvas->fill(Qt::transparent);
			QPainter painter(m_elements_canvas);
			for (elements::Element* element : m_elements)
			{
				painter.save();
				painter.translate(element->pos());
				element->paint(&painter);
				painter.restore();
			}
			painter.end();

		}
		void WindowBase::drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option)
		{
			const QRectF paint_region = this->ContentPaintRect();
			int pos_x = paint_region.x();
			int pos_y = paint_region.y();
			int width = paint_region.width();
			int height= paint_region.height();

			painter->drawPixmap(pos_x, pos_y, 24, 24, _winskin->_pixmap, 96, 0, 24, 24);
			painter->drawPixmap(24 + pos_x, pos_y, width - 48, 24, _winskin->_pixmap, 120, 0, 48, 24);
			painter->drawPixmap(width / 2 - 24, pos_y, 24, 24, _winskin->_pixmap, 168, 0, 24, 24);

			painter->drawPixmap(width / 2 - 24, 24 + pos_y, 24, height - 48, _winskin->_pixmap, 168, 24, 24, 48);
			painter->drawPixmap(width / 2 - 24, height / 2 - 24, 24, 24, _winskin->_pixmap, 168, 72, 24, 24);

			painter->drawPixmap(24 + pos_x, height / 2 - 24, width - 48, 24, _winskin->_pixmap, 120, 72, 48, 24);
			painter->drawPixmap(pos_x, height / 2 - 24, 24, 24, _winskin->_pixmap, 96, 72, 24, 24);
			painter->drawPixmap(pos_x, 24 + pos_y, 24, height - 48, _winskin->_pixmap, 96, 24, 24, 48);

			if (m_elements_canvas)
			{
				painter->drawPixmap(pos_x + 18, pos_y + 18, (*m_elements_canvas));
			}
		}

		GraphicsItem* create()
		{
			return new WindowBase();
		}
		int rpgobject_type()
		{
			return RPGOBJECT_WINDOWBASE;
		}
		QString Js_initProperties(QDomElement& properties, QString objectName)
		{
			if (properties.isNull())return "";
			//objectName.appendElements(
			QStringList js_elements;
			QStringList js_params;
			QDomElement element = properties.firstChildElement();
			while (!element.isNull())
			{
				QDomElement next = element.firstChildElement();
				while (!next.isNull())
				{
					if (next.tagName() == "x"|| next.tagName() == "y")
					{
						js_params.push_back(next.firstChild().nodeValue());
					}
					else
					{
						int type = manager::ElementLibraryLoader::instance()->loadElementLib(element.attribute("dllname"));
						js_params.push_back(manager::ElementsFactory::instance()->Js_RestParams(type,next));
					}
					next = next.nextSiblingElement();
				}
				js_elements.push_back(QString("new hjdd.%1(%2)").arg(element.attribute("dllname")).arg(js_params.join(',')));
				js_params.clear();
				element = element.nextSiblingElement();
			}
			return QString("%1.appendElements(%2);\n%1.refreshElement();\n").arg(objectName).arg(js_elements.join(",\n"));
		}
		
	}
}

