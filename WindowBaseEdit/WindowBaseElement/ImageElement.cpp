#include"ImageElement.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
namespace hjdd
{
	namespace elements
	{
		ImageElementClipWidget::ImageElementClipWidget(QWidget* parent):
			QWidget(parent),
			m_img_element(0),
			m_active(false)
		{
		}
		ImageElementClipWidget::~ImageElementClipWidget()
		{
		}
		void ImageElementClipWidget::setItem(ImageElement* item)
		{
			m_img_element = item;
		}

		QRect ImageElementClipWidget::ClipRectControlRect() const
		{
			if (!m_img_element)return QRect();
			QRect& cliprect = m_img_element->clipRect();

			return QRect(
				cliprect.x() - m_img_element->pixmap()->_pixmap.width() / 2,
				cliprect.y() - m_img_element->pixmap()->_pixmap.height() / 2,
				cliprect.width(),
				cliprect.height()
			);
		}

		void ImageElementClipWidget::paintEvent(QPaintEvent* e)
		{
			if (m_img_element && m_img_element->pixmap())
			{
				int sprite_width = m_img_element->pixmap()->_pixmap.width();
				int sprite_height = m_img_element->pixmap()->_pixmap.height();
				int des_x = this->width() / 2 - sprite_width / 2;
				int des_y = this->height() / 2 - sprite_height / 2;
				const QRect clipRect = m_img_element->clipRect();
				QPainter p(this);
				p.drawPixmap(des_x, des_y, m_img_element->pixmap()->_pixmap);
				p.fillRect(des_x, des_y, sprite_width, sprite_height, QColor(102, 139, 139, 127));
				p.drawPixmap(clipRect.x() + des_x, clipRect.y() + des_y, clipRect.width(), clipRect.height(),
					m_img_element->pixmap()->_pixmap,
					clipRect.x(), clipRect.y(), clipRect.width(), clipRect.height());
				//锚点
				this->drawAnchor(&p, QRect(clipRect.x() + des_x, clipRect.y() + des_y, clipRect.width(), clipRect.height()));
				p.end();
			}
		}

		void ImageElementClipWidget::mousePressEvent(QMouseEvent* event)
		{
			if (event->button() == Qt::LeftButton)
			{
				m_active = false;
				int ms_pos_x = event->x() - this->width() / 2;
				int ms_pos_y = event->y() - this->height() / 2;
				const QRect rect = ClipRectControlRect();
				if (rect.contains(ms_pos_x, ms_pos_y))
				{
					m_active = true;
					m_clickedStatue = CLICKED_CONTENT;
					if (ms_pos_x < rect.left() + SQUARE_SIZE)
					{
						m_clickedStatue |= CLICKED_LEFT;
					}
					else if (ms_pos_x > rect.right() - SQUARE_SIZE)
					{
						m_clickedStatue |= CLICKED_RIGHT;
					}

					if (ms_pos_y < rect.top() + SQUARE_SIZE)
					{
						m_clickedStatue |= CLICKED_TOP;
					}
					else if (ms_pos_y > rect.bottom() - SQUARE_SIZE)
					{
						m_clickedStatue |= CLICKED_BOTTOM;
					}
					m_diff = QPoint(rect.x() - ms_pos_x, rect.y() - ms_pos_y);
				}
			}
		}

		void ImageElementClipWidget::mouseMoveEvent(QMouseEvent* event)
		{
			if (event->buttons() & Qt::LeftButton && m_active)
			{
				int ms_pos_x = event->x() - this->width() / 2;
				int ms_pos_y = event->y() - this->height() / 2;
				QRect& cliprect = m_img_element->clipRect();

				int sprite_halfwidth = m_img_element->pixmap()->_pixmap.width() / 2;
				int sprite_halfheight = m_img_element->pixmap()->_pixmap.height() / 2;
				if (m_clickedStatue == CLICKED_CONTENT)
				{
					int des_x = ms_pos_x + m_diff.x();
					int des_y = ms_pos_y + m_diff.y();
					if (des_x < -sprite_halfwidth)
					{
						des_x = -sprite_halfwidth;
					}
					else if (des_x + cliprect.width() > sprite_halfwidth)
					{
						des_x = sprite_halfwidth - cliprect.width();
					}
					if (des_y < -sprite_halfheight)
					{
						des_y = -sprite_halfheight;
					}
					else if (des_y + cliprect.height() > sprite_halfheight)
					{
						des_y = sprite_halfheight - cliprect.height();
					}
					cliprect.moveTo(des_x + sprite_halfwidth, des_y + sprite_halfheight);
				}
				else
				{
					if (m_clickedStatue & CLICKED_LEFT)
					{
						if (ms_pos_x < -sprite_halfwidth)
						{
							ms_pos_x = -sprite_halfwidth;
						}
						if (ms_pos_x + sprite_halfwidth <= cliprect.right() - SQUARE_SIZE * 7)
							cliprect.setLeft(ms_pos_x + sprite_halfwidth);
					}
					else if (m_clickedStatue & CLICKED_RIGHT)
					{
						if (ms_pos_x > sprite_halfwidth)
						{
							ms_pos_x = sprite_halfwidth;
						}
						if (ms_pos_x + sprite_halfwidth >= cliprect.left() + SQUARE_SIZE * 7)
							cliprect.setRight(ms_pos_x + sprite_halfwidth);
					}

					if (m_clickedStatue & CLICKED_TOP)
					{
						if (ms_pos_y < -sprite_halfheight)
						{
							ms_pos_y = -sprite_halfheight;
						}
						if (ms_pos_y + sprite_halfheight <= cliprect.bottom() - SQUARE_SIZE * 7)
							cliprect.setTop(ms_pos_y + sprite_halfheight);
					}
					else if (m_clickedStatue & CLICKED_BOTTOM)
					{
						if (ms_pos_y > sprite_halfheight)
						{
							ms_pos_y = sprite_halfheight;
						}
						if (ms_pos_y + sprite_halfwidth >= cliprect.top() + SQUARE_SIZE * 7)
							cliprect.setBottom(ms_pos_y + sprite_halfheight);
					}
				}
				this->update();
				emit this->clipChanged();
			}
		}

		void ImageElementClipWidget::drawAnchor(QPainter* painter, const QRect rect)
		{
			painter->setBrush(Qt::red);
			//第一行三个
			painter->drawRect(rect.x(),rect.y(),SQUARE_SIZE,SQUARE_SIZE);//左上
			painter->drawRect(rect.x() + rect.width() / 2 - SQUARE_HALF_SIZE,rect.y(),SQUARE_SIZE,SQUARE_SIZE);//上
			painter->drawRect(rect.x() + rect.width() - SQUARE_SIZE,rect.y(),SQUARE_SIZE,SQUARE_SIZE);//右上

			//中间两个
			painter->drawRect(rect.x(),rect.y() + rect.height() / 2 - SQUARE_HALF_SIZE,SQUARE_SIZE,SQUARE_SIZE);//左
			painter->drawRect(rect.x() + rect.width() - SQUARE_SIZE,rect.y() + rect.height() / 2 - SQUARE_HALF_SIZE,SQUARE_SIZE,SQUARE_SIZE);//右

			//下端三个
			painter->drawRect(rect.x(),rect.y() + rect.height() - SQUARE_SIZE,SQUARE_SIZE,SQUARE_SIZE);//左下
			painter->drawRect(rect.x() + rect.width() / 2 - SQUARE_HALF_SIZE,rect.y() + rect.height() - SQUARE_SIZE,SQUARE_SIZE,SQUARE_SIZE);//下
			painter->drawRect(rect.x() + rect.width() - SQUARE_SIZE,rect.y() + rect.height() - SQUARE_SIZE,SQUARE_SIZE,SQUARE_SIZE);//右下
		}

		ImageElementEdit::ImageElementEdit(QWidget* parent):
			ElementEdit(parent)
		{
			QVBoxLayout* layout = new QVBoxLayout(this);
			this->setLayout(layout);

			QHBoxLayout* pixmap_layer = new QHBoxLayout;
			layout->addLayout(pixmap_layer);
			QLabel* pixmap_label = new QLabel("图片", this);
			m_pixmap_edit = new QLineEdit(this);
			m_pixmap_edit->setEnabled(false);
			QPushButton* selected = new QPushButton("...", this);
			pixmap_layer->addWidget(pixmap_label);
			pixmap_layer->addWidget(m_pixmap_edit);
			pixmap_layer->addWidget(selected);

			m_clipWidget = new ImageElementClipWidget(this);
			layout->addWidget(m_clipWidget);
			connect(selected, &QPushButton::clicked, this, &ImageElementEdit::SelectPixmap);
			connect(m_clipWidget, &ImageElementClipWidget::clipChanged, [this]() {emit this->propertiesChanged(); });
		}
		ImageElementEdit::~ImageElementEdit()
		{
		}

		void ImageElementEdit::SelectPixmap()
		{
			QString dir = manager::ConfigInfo::instance()->imgPath();
			QString filename = QFileDialog::getOpenFileName(nullptr, "选择图片", dir + "pictures/", "*.png");
			if (!filename.isEmpty())
			{
				m_pixmap_edit->setText(filename.right(filename.length() - dir.length()));
				((ImageElement*)this->element())->setImage(m_pixmap_edit->text());
				emit this->propertiesChanged();
			}
		}

		void ImageElementEdit::setProperties(Element*element)
		{
			ImageElement* ele = (ImageElement*)element;
			if (ele->pixmap())
			{
				m_pixmap_edit->setText(ele->pixmap()->_filename);
				m_clipWidget->setMinimumSize(ele->pixmap()->_pixmap.width(), ele->pixmap()->_pixmap.height());
			}
			else m_pixmap_edit->clear();
			m_clipWidget->setItem(ele);
		}

		void ImageElementEdit::OthersConnect(Element*){}
		void ImageElementEdit::OthersDisConnect(Element*){}

		ImageElement::ImageElement():
			Element(128,128),
			m_sprite_src(0)
		{
		}
		ImageElement::~ImageElement()
		{
			if (m_sprite_src)
			{
				RELEASE_IMAGE(m_sprite_src);
				m_sprite_src = 0;
			}
		}
		QDomElement ImageElement::xmlSave(QDomDocument& dom)
		{
			QDomElement Image = dom.createElement("Image");
			//Size
			QDomElement Size = dom.createElement("Size");
			QDomElement dw = dom.createElement("width");
			dw.appendChild(dom.createTextNode(QString::number(m_width)));
			QDomElement dh = dom.createElement("height");
			dh.appendChild(dom.createTextNode(QString::number(m_height)));
			Size.appendChild(dw);
			Size.appendChild(dh);
			Image.appendChild(Size);


			if (m_sprite_src)
			{
				//ClipRect
				QDomElement ClipRect = dom.createElement("ClipRect");
				QDomElement sx = dom.createElement("x");
				sx.appendChild(dom.createTextNode(QString::number(m_clipRect.x())));
				QDomElement sy = dom.createElement("y");
				sy.appendChild(dom.createTextNode(QString::number(m_clipRect.y())));
				QDomElement sw = dom.createElement("width");
				sw.appendChild(dom.createTextNode(QString::number(m_clipRect.width())));
				QDomElement sh = dom.createElement("height");
				sh.appendChild(dom.createTextNode(QString::number(m_clipRect.height())));
				ClipRect.appendChild(sx);
				ClipRect.appendChild(sy);
				ClipRect.appendChild(sw);
				ClipRect.appendChild(sh);
				Image.appendChild(ClipRect);
				//Path
				QDomElement Path = dom.createElement("Path");
				Path.appendChild(dom.createTextNode(m_sprite_src->_filename));
				Image.appendChild(Path);
			}
			return Image;
		}
		void ImageElement::xmlLoad(QDomElement& selfPrototype)
		{
			QDomElement next = selfPrototype.firstChildElement();
			while (!next.isNull())
			{
				if (next.tagName() == "Size")
				{
					this->m_width = next.firstChildElement("width").firstChild().nodeValue().toInt();
					this->m_height = next.firstChildElement("height").firstChild().nodeValue().toInt();
				}
				else if (next.tagName() == "ClipRect")
				{
					QDomElement inNext = next.firstChildElement();
					while (!inNext.isNull())
					{
						if (inNext.tagName() == "x") { m_clipRect.setX(inNext.firstChild().nodeValue().toInt()); }
						else if(inNext.tagName() == "y"){ m_clipRect.setY(inNext.firstChild().nodeValue().toInt()); }
						else if(inNext.tagName() == "width"){ m_clipRect.setWidth(inNext.firstChild().nodeValue().toInt()); }
						else if(inNext.tagName() == "height"){ m_clipRect.setHeight(inNext.firstChild().nodeValue().toInt()); }
						inNext = inNext.nextSiblingElement();
					}
				}
				else if (next.tagName() == "Path")
				{
					m_sprite_src = LOAD_IMAGE(next.firstChild().nodeValue());
				}
				next = next.nextSiblingElement();
			}
		}
		void ImageElement::setImage(const QString& filename)
		{
			if (m_sprite_src && filename == m_sprite_src->_filename)return;
			if (m_sprite_src)
			{
				RELEASE_IMAGE(m_sprite_src);
			}
			m_sprite_src = LOAD_IMAGE(filename);
			m_clipRect.setRect(0, 0, m_sprite_src->_pixmap.width(), m_sprite_src->_pixmap.height());
			m_width = m_sprite_src->_pixmap.width();
			m_height = m_sprite_src->_pixmap.height();
		}
		void ImageElement::drawBackground(QPainter* painter)
		{
			if (m_sprite_src)
			{
				painter->drawPixmap(QRect(0,0,m_width,m_height), m_sprite_src->_pixmap, m_clipRect);
			}
			else
			{
				painter->fillRect(QRect(0, 0, m_width, m_height),Qt::gray);
			}
		}

		Element* create()
		{
			return new ImageElement;
		}
		int element_type()
		{
			return IMAGE_ELEMENT_TYPE;
		}
		QString Js_RestParams(QDomElement& properties)
		{
			QStringList jsParams;
			QDomElement next = properties.firstChildElement();
			while (!next.isNull())
			{
				if (next.tagName() == "Size" || next.tagName() == "ClipRect")
				{
					QDomElement inNext = next.firstChildElement();
					while (!inNext.isNull())
					{
						jsParams.push_back(inNext.firstChild().nodeValue());
						inNext = inNext.nextSiblingElement();
					}
				}
				else if (next.tagName() == "Path")
				{
					QStringList path_section = next.firstChild().nodeValue().split('/');
					QString filename = path_section[1].left(path_section[1].length()-4);
					jsParams.push_back(QString("\'img/%1/\',\'%2\'").arg(path_section[0]).arg(filename));
				}
				next = next.nextSiblingElement();
			}
			return jsParams.join(',');
		}
		
}
}