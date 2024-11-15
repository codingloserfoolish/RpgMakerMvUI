#include"IWinBaseElement.h"

namespace hjdd
{
	namespace elements
	{
		Element* Element::_selected_item = 0;
		QPoint Element::_diff;
		unsigned char Element::_clicked_region = CONTENT_REGION;

		void IWinBaseElement::mouseEvent(QMouseEvent* e,int type)
		{
			const QPoint pos = e->pos() - this->pos();
			if (type == MOUSE_PRESS)this->mousePressEvent(pos);
			else if (type == MOUSE_RELEASE)this->mouseReleaseEvent(pos);
			else if (type == MOUSE_MOVE)this->mouseMoveEvent(pos);
		}
		void IWinBaseElement::mousePressEvent(const QPoint& pos)
		{
		}
		void IWinBaseElement::mouseMoveEvent(const QPoint& pos)
		{
		}
		void IWinBaseElement::mouseReleaseEvent(const QPoint& pos)
		{
		}

		Element::Element(int w, int h):
			m_width(w< MIN_RECT ? MIN_RECT:w),
			m_height(h < MIN_RECT ? MIN_RECT:h),
			m_active(false)
		{
		}

		void Element::paint(QPainter* painter)
		{
			this->drawBackground(painter);
			if(m_active)this->drawForeground(painter);
		}
		QSize Element::size() const
		{
			return QSize(m_width,m_height);
		}

		QRect Element::Rect() const
		{
			return m_active?this->CompleteRect() : this->ContentRect();
		}

		QRect Element::CompleteRect() const
		{
			return QRect(_x, _y, m_width, m_height);
		}

		QRect Element::ContentRect() const
		{
			return QRect(_x + SQUARE_SIZE, _y + SQUARE_SIZE, m_width - SQUARE_SIZE, m_height - SQUARE_SIZE);
		}



		void Element::drawForeground(QPainter* painter)
		{
			this->drawBackground(painter);
			this->drawAnchor(painter);
		}
		void Element::drawBackground(QPainter* painter)
		{
			painter->fillRect(0, 0, m_width, m_height, Qt::gray);
		}
		void Element::mousePressEvent(const QPoint& pos)
		{
			if (Element::_selected_item != this)
			{
				if (Element::_selected_item)
				{
					Element::_selected_item->deactive();
				}
				Element::_selected_item = this;
			}
			Element::_diff = pos;
			//判断选中了哪个区域
			Element::_clicked_region = CONTENT_REGION;
			if (!this->isContentRegion(pos))
			{
				if (pos.x() < SQUARE_SIZE)Element::_clicked_region |= ANCHOR_REGION_LEFT;
				else if (pos.x() > m_width - SQUARE_SIZE)Element::_clicked_region |= ANCHOR_REGION_RIGHT;
				if (pos.y() < SQUARE_SIZE)Element::_clicked_region |= ANCHOR_REGION_TOP;
				else if (pos.y() > m_height - SQUARE_SIZE)Element::_clicked_region |= ANCHOR_REGION_BOTTOM;
			}

			this->activate();
		}
		void Element::mouseMoveEvent(const QPoint& pos)
		{
			if (Element::_clicked_region == CONTENT_REGION)
			{
				const QPoint des = pos - Element::_diff;
				this->_x += des.x();
				this->_y += des.y();
			}
			else
			{
				if (Element::_clicked_region &ANCHOR_REGION_LEFT)
				{
					_x += pos.x();
					m_width -= pos.x();
				}
				else if (Element::_clicked_region & ANCHOR_REGION_RIGHT)
				{
					m_width = pos.x();
				}
				if (Element::_clicked_region & ANCHOR_REGION_TOP)
				{
					_y += pos.y();
					m_height -= pos.y();
				}
				else if (Element::_clicked_region & ANCHOR_REGION_BOTTOM)
				{
					m_height = pos.y();
				}

				//如果尺寸过小则展开
				if (m_width < SQUARE_SIZE * 4)m_width = SQUARE_SIZE * 4;
				if (m_height < SQUARE_SIZE * 4)m_height = SQUARE_SIZE * 4;
			}
		}
		void Element::mouseReleaseEvent(const QPoint& pos)
		{
		}
		void Element::drawAnchor(QPainter* painter)
		{
			painter->setBrush(Qt::red);
			//第一行三个
			painter->drawRect(0,0,SQUARE_SIZE,SQUARE_SIZE);//左上
			painter->drawRect(m_width/2 - SQUARE_HALF_SIZE,0,SQUARE_SIZE,SQUARE_SIZE);//上
			painter->drawRect(m_width - SQUARE_SIZE,0,SQUARE_SIZE,SQUARE_SIZE);//右上

			//中间两个
			painter->drawRect(0,m_height / 2- SQUARE_HALF_SIZE,SQUARE_SIZE,SQUARE_SIZE);//左
			painter->drawRect(m_width - SQUARE_SIZE,m_height / 2 - SQUARE_HALF_SIZE,SQUARE_SIZE,SQUARE_SIZE);//右

			//下端三个
			painter->drawRect(0,m_height - SQUARE_SIZE,SQUARE_SIZE,SQUARE_SIZE);//左下
			painter->drawRect(m_width / 2 - SQUARE_HALF_SIZE,m_height - SQUARE_SIZE,SQUARE_SIZE,SQUARE_SIZE);//下
			painter->drawRect(m_width - SQUARE_SIZE,m_height - SQUARE_SIZE,SQUARE_SIZE,SQUARE_SIZE);//右下
		}
		bool Element::isContentRegion(const QPoint& pos)
		{
			return pos.x() > SQUARE_SIZE && pos.y() > SQUARE_SIZE && (pos.x() < m_width - SQUARE_SIZE) && (pos.y() < m_height - SQUARE_SIZE);
		}

		ElementEdit::ElementEdit(QWidget* parent):
			QWidget(parent),
			m_element(0)
		{
		}
		void ElementEdit::setElement(Element* ele)
		{
			if (m_element != ele)
			{
				if (m_element)
				{
					this->DisConnection();
				}
				if (!ele)
				{
					m_element = 0;
					return;
				}
				m_element = ele;
				this->setProperties(m_element);
				this->Connection();
			}
		}
		void ElementEdit::Connection()
		{
			this->OthersConnect(m_element);
		}
		void ElementEdit::DisConnection()
		{
			if (m_element)
			{
				this->OthersDisConnect(m_element);
				m_element = 0;
			}
		}
		void ElementEdit::setProperties(Element*)
		{
		}
		void ElementEdit::OthersConnect(Element*)
		{
		}
		void ElementEdit::OthersDisConnect(Element*)
		{
		}
}
}

