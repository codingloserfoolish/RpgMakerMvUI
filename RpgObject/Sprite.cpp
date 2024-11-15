#include"Sprite.h"
namespace hjdd
{
	namespace item
	{
		Sprite::Sprite(QGraphicsItem* parent):
			GraphicsItem(parent),
			m_sprite_src(0)
		{
		}
		Sprite::~Sprite()
		{
			if (m_sprite_src)
			{
				RELEASE_IMAGE(m_sprite_src);
				m_sprite_src = 0;
			}
		}
		QDomElement Sprite::OthersProperties(QDomDocument& dom)
		{
			if (!m_sprite_src)return QDomElement();
			QDomElement imageNode = dom.createElement("Image");
			//²Ã¼ôÇø
			QDomElement clipRect = dom.createElement("ClipRect");
			QDomElement left = dom.createElement("Left");
			left.appendChild(dom.createTextNode(QString::number(m_clipRect.left())));
			QDomElement top = dom.createElement("Top");
			top.appendChild(dom.createTextNode(QString::number(m_clipRect.top())));
			QDomElement right = dom.createElement("Right");
			right.appendChild(dom.createTextNode(QString::number(m_clipRect.right())));
			QDomElement bottom = dom.createElement("Bottom");
			bottom.appendChild(dom.createTextNode(QString::number(m_clipRect.bottom())));

			clipRect.appendChild(left);
			clipRect.appendChild(top);
			clipRect.appendChild(right);
			clipRect.appendChild(bottom);

			//²Ã¼ôÇøÊÕÎ²
			imageNode.appendChild(clipRect);
			//Í¼Æ¬Â·¾¶
			QDomElement imgPath = dom.createElement("Path");
			imgPath.appendChild(dom.createTextNode(m_sprite_src->_filename));
			//ÊÕÎ²
			imageNode.appendChild(imgPath);
			return imageNode;
		}
		void Sprite::OthersPropertiesLoad(QDomElement& node)
		{
			if (!node.isNull())
			{
				QDomElement clipRect = node.firstChildElement();
				QDomElement imgPath = clipRect.nextSiblingElement();
				//left
				QDomElement clipChild=clipRect.firstChildElement();
				m_clipRect.setLeft(clipChild.firstChild().nodeValue().toInt());
				//top
				clipChild = clipChild.nextSiblingElement();
				m_clipRect.setTop(clipChild.firstChild().nodeValue().toInt());
				//right
				clipChild = clipChild.nextSiblingElement();
				m_clipRect.setRight(clipChild.firstChild().nodeValue().toInt());
				//bottom
				clipChild = clipChild.nextSiblingElement();
				m_clipRect.setBottom(clipChild.firstChild().nodeValue().toInt());

				//Í¼Æ¬
				m_sprite_src = LOAD_IMAGE(imgPath.firstChild().nodeValue());
			}
		}

		void Sprite::setImage(const QString& filename)
		{
			if (m_sprite_src&&filename == m_sprite_src->_filename)return;
			if (m_sprite_src)
			{
				RELEASE_IMAGE(m_sprite_src);
			}
			m_sprite_src=LOAD_IMAGE(filename);
			m_clipRect.setRect(0, 0, m_sprite_src->_pixmap.width(), m_sprite_src->_pixmap.height());
			this->setWidth(m_sprite_src->_pixmap.width());
			this->setHeight(m_sprite_src->_pixmap.height());
			this->update();
		}
		void Sprite::drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option)
		{
			if (m_sprite_src)
			{
				painter->drawPixmap(-this->width()/2, -this->height() / 2,this->width(),this->height(), 
					m_sprite_src->_pixmap,
					m_clipRect.x(),m_clipRect.y(),m_clipRect.width(),m_clipRect.height());
			}
			else
			{
				GraphicsItem::drawBackground(painter, option);
			}
		}

		GraphicsItem* create()
		{
			return new Sprite();
		}
		int rpgobject_type()
		{
			return RPGOBJECT_SPRITE;
		}
		QString Js_initProperties(QDomElement& properties, QString objectName)
		{
			QDomElement clipRect = properties.firstChildElement();
			QStringList imgPath = clipRect.nextSiblingElement().firstChild().nodeValue().split('/');
			QString name = imgPath.at(1);

			QStringList rect;

			QDomElement next=clipRect.firstChildElement();
			while (!next.isNull())
			{
				rect.push_back(next.firstChild().nodeValue());
				next = next.nextSiblingElement();
			}

			QString js_insert = QString("%1.bitmap=ImageManager.loadBitmap('img/%2/','%3',null,false);\n%1.setFrame(%4);\n")
				.arg(objectName)
				.arg(imgPath.at(0))
				.arg(name.left(name.length()-4))
				.arg(rect.join(','));
			
			return js_insert;
		}
	}
}