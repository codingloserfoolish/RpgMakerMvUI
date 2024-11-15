#include"GraphicsItem.h"
namespace hjdd
{
	namespace item
	{
		ITEMEXPORT QPointF GraphicsItem::_local_mouse_diff(0, 0);
		ITEMEXPORT GraphicsItem* GraphicsItem::_selected_item=0;
		GraphicsItem::GraphicsItem(QGraphicsItem* parent):
			QObject(nullptr),
			QGraphicsItem(parent),
			m_isActive(false),
			m_moveCtrlStatue(MoveCtrStatue_CONTENT),
			m_width(100),
			m_height(100),
			standardItem(0)
		{
			setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		}
		GraphicsItem::GraphicsItem(int width, int height, QGraphicsItem* parent):
			QObject(nullptr),
			QGraphicsItem(parent),
			m_moveCtrlStatue(MoveCtrStatue_CONTENT),
			m_width(width),
			m_height(height),
			standardItem(0)
		{
			setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
		}
		GraphicsItem::~GraphicsItem()
		{
		}
		bool GraphicsItem::findChild(GraphicsItem* item)
		{
			for (QGraphicsItem* child : this->childItems())
			{
				if (((GraphicsItem*)child)->findChild(item))
					return true;
			}
			return item == this;
		}
		QRectF GraphicsItem::ContentSelectedRect()const
		{
			return QRectF(
				-m_width/2+ GraphicsAnchor_HalfSquareSize,
				-m_height/2+ GraphicsAnchor_HalfSquareSize,
				m_width-GraphicsAnchor_SquareSize, 
				m_height- GraphicsAnchor_SquareSize);
		}
		QRectF GraphicsItem::ContentPaintRect() const
		{
			return QRectF(
				-m_width / 2,
				-m_height / 2,
				m_width,
				m_height
			);
		}
		QRectF GraphicsItem::ContentAnchorRect() const
		{
			return QRectF(
				-m_width / 2 - GraphicsAnchor_HalfSquareSize,
				-m_height / 2 - GraphicsAnchor_HalfSquareSize,
				m_width + GraphicsAnchor_SquareSize,
				m_height + GraphicsAnchor_SquareSize);
		}
		QRectF GraphicsItem::boundingRect() const
		{
			return QRectF(
				-m_width / 2 - GraphicsAnchor_HalfSquareSize-1,
				-m_height / 2 - GraphicsAnchor_HalfSquareSize-1,
				m_width + GraphicsAnchor_SquareSize+2,
				m_height + GraphicsAnchor_SquareSize+2);
		}
		void GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
		{
			drawBackground(painter, option);
			drawForeground(painter, option);
		}

		void GraphicsItem::addChild(GraphicsItem* item)
		{
			if (item)
			{
				item->setParentItem(this);
			}
		}
		
		void GraphicsItem::drawForeground(QPainter* painter, const QStyleOptionGraphicsItem* option)
		{
			if (GraphicsItem::_selected_item==this)
			{
				this->drawAnchor(painter, option);
				this->drawAngle(painter, option);
			}
		}

		void GraphicsItem::drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option)
		{
			painter->setBrush(Qt::gray);
			painter->drawRect(this->ContentPaintRect());
		}

		void GraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
		{
			if (event->buttons() & Qt::LeftButton)
			{
				if (m_moveCtrlStatue == MoveCtrStatue_CONTENT)
				{
					this->setPos(mapToParent(event->pos()) - GraphicsItem::_local_mouse_diff);
					emit positionChanged(this->pos());
				}
				else
				{
					QPointF ms_pos =event->pos();
					qreal x_asix=0,y_asix=0;
					if (m_moveCtrlStatue == MoveCtrStatue_ANGLE_BAR)
					{
						QPointF parent_ms = mapToParent(ms_pos)-this->pos();
						if (parent_ms.x() > ANGLE_RANGE)
						{
							this->setRotation(fast_atan(parent_ms.y() / parent_ms.x()) * 180 / pi);
						}
						else if(abs(parent_ms.x())<= ANGLE_RANGE)
						{
							if (parent_ms.y() < 0)this->setRotation(-90);
							else this->setRotation(90);
						}
						else
						{
							if (parent_ms.y() > 0) this->setRotation(fast_atan(parent_ms.y() / parent_ms.x())*180/pi+180);
							else this->setRotation(fast_atan(parent_ms.y() / parent_ms.x())*180/pi - 180);
						}
						emit angleChanged(this->rotation());
						//this->update();
					}
					else
					{
						if (m_moveCtrlStatue & MoveCtrStatue_LEFT)
						{
							x_asix = (ms_pos.x() + m_width / 2);
							this->m_width -= x_asix;
							x_asix /= 2;
						}
						else if (m_moveCtrlStatue & MoveCtrStatue_RIGHT)
						{
							x_asix = (ms_pos.x() - m_width / 2);
							this->m_width += x_asix;
							x_asix /= 2;
						}
						if (m_moveCtrlStatue & MoveCtrStatue_TOP)
						{
							y_asix = (ms_pos.y() + m_height / 2);
							this->m_height -= y_asix;
							y_asix /= 2;
						}
						else if (m_moveCtrlStatue & MoveCtrStatue_BOTTOM)
						{
							y_asix = (ms_pos.y() - m_height / 2);
							this->m_height += y_asix;
							y_asix /= 2;
						}
						/*
						* 防止图形大小过小
						*/
						if (this->m_width < GraphicsAnchor_SquareSize * 3)
						{
							this->m_width += abs(x_asix * 2);
							x_asix = 0;
						}
						if (this->m_height < GraphicsAnchor_SquareSize * 3)
						{
							this->m_height += abs(y_asix * 2);
							y_asix = 0;
						}

						if (x_asix != 0 || y_asix != 0)
						{
							this->setPos(mapToParent(x_asix, y_asix));
							emit positionChanged(this->pos());
						}
						emit sizeChanged(this->ContentPaintRect());
					}
					
				}
			}
		}
		void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
		{
			this->m_isActive = true;
			if (event->button() == Qt::LeftButton)
			{
				m_moveCtrlStatue = 0;
				if (GraphicsItem::_selected_item != this)
				{
					if (this->boundingRect().contains(event->pos()))
					{
						m_moveCtrlStatue = MoveCtrStatue_CONTENT;
						GraphicsItem* pre = GraphicsItem::_selected_item;
						GraphicsItem::_selected_item = this;
						if (pre)
						{
							pre->update();
						}
						this->update();
					}
				}
				else
				{
					const QPointF ms_p = event->pos();
					const QRectF foreRect = ContentAnchorRect();
					qreal ms_angle = abs(fast_atan(ms_p.y() / ms_p.x()));
					if (this->ContentSelectedRect().contains(ms_p))//
					{
						m_moveCtrlStatue = MoveCtrStatue_CONTENT;
					}
					else if (foreRect.contains(ms_p))//锚点区域
					{
						if (ms_p.x() < foreRect.x() + GraphicsAnchor_SquareSize)
						{
							m_moveCtrlStatue |= MoveCtrStatue_LEFT;
						}
						else if (ms_p.x() > foreRect.x() + foreRect.width() - GraphicsAnchor_SquareSize)
						{
							m_moveCtrlStatue |= MoveCtrStatue_RIGHT;
						}
						if (ms_p.y() < foreRect.y() + GraphicsAnchor_SquareSize)
						{
							m_moveCtrlStatue |= MoveCtrStatue_TOP;
						}
						else if (ms_p.y() > foreRect.y() + foreRect.height() - GraphicsAnchor_SquareSize)
						{
							m_moveCtrlStatue |= MoveCtrStatue_BOTTOM;
						}
					}

					if (ms_p.x() > 0 && ms_angle <= ANGLE_RANGE * 2)
					{
						m_moveCtrlStatue = MoveCtrStatue_ANGLE_BAR;
					}

				}
				GraphicsItem::_local_mouse_diff = mapToParent(event->pos()) - this->pos();
			}
		}


		void GraphicsItem::drawAnchor(QPainter* painter, const QStyleOptionGraphicsItem* option)
		{
			painter->setBrush(Qt::red);
			const QRectF foreRect = ContentAnchorRect();
			//第一行三个
			painter->drawRect(
				foreRect.x(), 
				foreRect.y(), 
				GraphicsAnchor_SquareSize, 
				GraphicsAnchor_SquareSize);//左上
			painter->drawRect(
				foreRect.x()+foreRect.width()/2- GraphicsAnchor_HalfSquareSize, 
				foreRect.y(), 
				GraphicsAnchor_SquareSize, 
				GraphicsAnchor_SquareSize);//上
			painter->drawRect(
				foreRect.x()+foreRect.width()- GraphicsAnchor_SquareSize, 
				foreRect.y(), 
				GraphicsAnchor_SquareSize, 
				GraphicsAnchor_SquareSize);//右上

			//中间两个
			painter->drawRect(
				foreRect.x(), 
				foreRect.y()+foreRect.height()/2- GraphicsAnchor_HalfSquareSize, 
				GraphicsAnchor_SquareSize, 
				GraphicsAnchor_SquareSize);//左
			painter->drawRect(
				foreRect.x()+foreRect.width()- GraphicsAnchor_SquareSize, 
				foreRect.y()+foreRect.height()/2- GraphicsAnchor_HalfSquareSize, 
				GraphicsAnchor_SquareSize, 
				GraphicsAnchor_SquareSize);//右
			
			//下端三个
			painter->drawRect(
				foreRect.x(), 
				foreRect.y()+foreRect.height()- GraphicsAnchor_SquareSize, 
				GraphicsAnchor_SquareSize, 
				GraphicsAnchor_SquareSize);//左下
			painter->drawRect(
				foreRect.x() + foreRect.width()/2- GraphicsAnchor_HalfSquareSize, 
				foreRect.y()+foreRect.height()- GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize, 
				GraphicsAnchor_SquareSize);//下
			painter->drawRect(
				foreRect.x() + foreRect.width()- GraphicsAnchor_SquareSize, 
				foreRect.y()+foreRect.height()- GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//右下
		}
		void GraphicsItem::drawAngle(QPainter* painter, const QStyleOptionGraphicsItem* option)
		{
			int radius = this->radius();
			painter->setPen(Qt::green);
			//基准轴
			painter->drawLine(0, 0, radius * fast_cos(this->rotation()*pi/180), -radius * fast_sin(this->rotation()* pi / 180));
			painter->setPen(QPen(QColor(2, 250, 216), 4));
			//转动杆
			painter->drawLine(0, 0, radius, 0);
			//角度
			painter->drawArc(
				-radius/2+GraphicsAnchor_HalfSquareSize,
				-radius /2+ GraphicsAnchor_HalfSquareSize, 
				radius-GraphicsAnchor_SquareSize, 
				radius-GraphicsAnchor_SquareSize,
				0,
				this->rotation()*16);
			painter->setPen(QColor(165,42,42));
			painter->drawText(radius/2-18,-4, QString::number(this->rotation(),'f',1));
		}
	}
}