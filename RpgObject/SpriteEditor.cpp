#include"SpriteEditor.h"
#include"Sprite.h"
#include<QMouseEvent>
#include<QDebug>
namespace hjdd
{
	namespace widget
	{
		SpriteClipWidget::SpriteClipWidget(QWidget* parent):
			QWidget(parent),
			m_sprite(0),
			m_active(false)
		{
			
		}

		SpriteClipWidget::~SpriteClipWidget()
		{
		}

		void SpriteClipWidget::setItem(item::GraphicsItem* item)
		{
			m_sprite = item;
		}

		QRect SpriteClipWidget::ClipRectControlRect() const
		{
			if (!m_sprite)return QRect();
			item::Sprite* sprite = (item::Sprite*)m_sprite;
			QRect& cliprect = sprite->clipRect();

			return QRect(
				cliprect.x() - sprite->pixmap()->_pixmap.width()/2,
				cliprect.y() - sprite->pixmap()->_pixmap.height()/2,
				cliprect.width(),
				cliprect.height()
			);
		}

		void SpriteClipWidget::paintEvent(QPaintEvent* e)
		{
			if (m_sprite&&((item::Sprite*)m_sprite)->pixmap())
			{
				item::Sprite* sprite = (item::Sprite*)m_sprite;
				int sprite_width = sprite->pixmap()->_pixmap.width();
				int sprite_height = sprite->pixmap()->_pixmap.height();
				int des_x = this->width() / 2 - sprite_width / 2;
				int des_y = this->height() / 2 - sprite_height / 2;
				const QRect clipRect = sprite->clipRect();
				QPainter p(this);
				p.drawPixmap(des_x,des_y,sprite->pixmap()->_pixmap);
				p.fillRect(des_x, des_y, sprite_width, sprite_height, QColor(102, 139, 139, 127));
				p.drawPixmap(clipRect.x()+des_x,clipRect.y()+des_y, clipRect.width(),clipRect.height(),
					sprite->pixmap()->_pixmap, 
					clipRect.x(),clipRect.y(),clipRect.width(),clipRect.height());
				//锚点
				this->drawAnchor(&p, QRect(clipRect.x() + des_x, clipRect.y() + des_y, clipRect.width(), clipRect.height()));
				p.end();
			}
		}

		void SpriteClipWidget::mousePressEvent(QMouseEvent* event)
		{
			if (event->button() == Qt::LeftButton)
			{
				m_active = false;
				int ms_pos_x = event->x() - this->width() / 2;
				int ms_pos_y = event->y() - this->height() / 2;
				const QRect rect = ClipRectControlRect();
				if (rect.contains(ms_pos_x,ms_pos_y))
				{
					m_active = true;
					m_moveCtrlStatue = MoveCtrStatue_CONTENT;
					if (ms_pos_x < rect.left() + GraphicsAnchor_SquareSize)
					{
						m_moveCtrlStatue |= MoveCtrStatue_LEFT;
					}
					else if (ms_pos_x > rect.right() - GraphicsAnchor_SquareSize)
					{
						m_moveCtrlStatue |= MoveCtrStatue_RIGHT;
					}
					
					if (ms_pos_y < rect.top() + GraphicsAnchor_SquareSize)
					{
						m_moveCtrlStatue |= MoveCtrStatue_TOP;
					}
					else if (ms_pos_y > rect.bottom() - GraphicsAnchor_SquareSize)
					{
						m_moveCtrlStatue |= MoveCtrStatue_BOTTOM;
					}
					m_diff =QPoint(rect.x()-ms_pos_x,rect.y()-ms_pos_y);
				}
			}
		}

		void SpriteClipWidget::mouseMoveEvent(QMouseEvent* event)
		{
			if (event->buttons() & Qt::LeftButton&&m_active)
			{
				int ms_pos_x = event->x() - this->width() / 2;
				int ms_pos_y = event->y() - this->height() / 2;
				QRect& cliprect = ((item::Sprite*)m_sprite)->clipRect();
				
				int sprite_halfwidth = ((item::Sprite*)m_sprite)->pixmap()->_pixmap.width() / 2;
				int sprite_halfheight = ((item::Sprite*)m_sprite)->pixmap()->_pixmap.height() / 2;
				if (m_moveCtrlStatue == MoveCtrStatue_CONTENT)
				{
					int des_x = ms_pos_x + m_diff.x();
					int des_y = ms_pos_y + m_diff.y();
					if (des_x<-sprite_halfwidth)
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
					cliprect.moveTo(des_x+ sprite_halfwidth, des_y+ sprite_halfheight);
				}
				else
				{
					if (m_moveCtrlStatue & MoveCtrStatue_LEFT)
					{
						if (ms_pos_x < -sprite_halfwidth)
						{
							ms_pos_x = -sprite_halfwidth;
						}
						if(ms_pos_x+ sprite_halfwidth <=cliprect.right()-GraphicsAnchor_SquareSize*7)
							cliprect.setLeft(ms_pos_x+ sprite_halfwidth);
					}
					else if (m_moveCtrlStatue & MoveCtrStatue_RIGHT)
					{
						if (ms_pos_x > sprite_halfwidth)
						{
							ms_pos_x = sprite_halfwidth;
						}
						if (ms_pos_x + sprite_halfwidth >= cliprect.left() + GraphicsAnchor_SquareSize * 7)
							cliprect.setRight(ms_pos_x+ sprite_halfwidth);
					}
					
					if (m_moveCtrlStatue & MoveCtrStatue_TOP)
					{
						if (ms_pos_y < -sprite_halfheight)
						{
							ms_pos_y = -sprite_halfheight;
						}
						if (ms_pos_y + sprite_halfheight <= cliprect.bottom() - GraphicsAnchor_SquareSize * 7)
							cliprect.setTop(ms_pos_y+ sprite_halfheight);
					}
					else if (m_moveCtrlStatue & MoveCtrStatue_BOTTOM)
					{
						if (ms_pos_y > sprite_halfheight)
						{
							ms_pos_y = sprite_halfheight;
						}
						if (ms_pos_y + sprite_halfwidth >= cliprect.top() + GraphicsAnchor_SquareSize * 7)
							cliprect.setBottom(ms_pos_y+ sprite_halfheight);
					}
				}
				this->update();
				m_sprite->update();
			}
		}

		void SpriteClipWidget::drawAnchor(QPainter* painter, const QRect rect)
		{
			painter->setBrush(Qt::red);
			//第一行三个
			painter->drawRect(
				rect.x(),
				rect.y(),
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//左上
			painter->drawRect(
				rect.x() + rect.width() / 2 - GraphicsAnchor_HalfSquareSize,
				rect.y(),
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//上
			painter->drawRect(
				rect.x() + rect.width() - GraphicsAnchor_SquareSize,
				rect.y(),
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//右上

			//中间两个
			painter->drawRect(
				rect.x(),
				rect.y() + rect.height() / 2 - GraphicsAnchor_HalfSquareSize,
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//左
			painter->drawRect(
				rect.x() + rect.width() - GraphicsAnchor_SquareSize,
				rect.y() + rect.height() / 2 - GraphicsAnchor_HalfSquareSize,
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//右

			//下端三个
			painter->drawRect(
				rect.x(),
				rect.y() + rect.height() - GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//左下
			painter->drawRect(
				rect.x() + rect.width() / 2 - GraphicsAnchor_HalfSquareSize,
				rect.y() + rect.height() - GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//下
			painter->drawRect(
				rect.x() + rect.width() - GraphicsAnchor_SquareSize,
				rect.y() + rect.height() - GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize,
				GraphicsAnchor_SquareSize);//右下
		}

		SpriteEditor::SpriteEditor(QWidget* parent):
			ObjectEditor(parent),
			m_pixmap_edit(0)
		{
			this->resize(this->width(), this->height() + 540);
		}
		SpriteEditor::~SpriteEditor()
		{
		}
		void SpriteEditor::SelectPixmap()
		{
			QString dir = manager::ConfigInfo::instance()->imgPath();
			QString filename = QFileDialog::getOpenFileName(nullptr, "选择图片", dir+"pictures/", "*.png");
			if (!filename.isEmpty())
			{
				m_pixmap_edit->setText(filename.right(filename.length() - dir.length()));
			}
		}
		void SpriteEditor::initLayOut(QVBoxLayout* vlayout)
		{
			QHBoxLayout* pixmap_layer=new QHBoxLayout;
			vlayout->addLayout(pixmap_layer);
			QLabel* pixmap_label = new QLabel("图片", this);
			m_pixmap_edit = new QLineEdit(this);
			m_pixmap_edit->setEnabled(false);
			QPushButton* selected = new QPushButton("...", this);
			pixmap_layer->addWidget(pixmap_label);
			pixmap_layer->addWidget(m_pixmap_edit);
			pixmap_layer->addWidget(selected);

			m_clipWidget = new SpriteClipWidget(this);
			vlayout->addWidget(m_clipWidget);
			connect(selected, &QPushButton::clicked, this, &SpriteEditor::SelectPixmap);
		}
		void SpriteEditor::setProperties(item::GraphicsItem* item)
		{
			m_pixmap_edit->setText(((item::Sprite*)item)->filename());
			m_clipWidget->setItem(item);
		}
		void SpriteEditor::Connection()
		{
			connect(m_pixmap_edit,&QLineEdit::textChanged,(item::Sprite*)this->item(),&item::Sprite::setImage);
		}
		void SpriteEditor::DisConnection()
		{
			disconnect(m_pixmap_edit, 0, (item::Sprite*)this->item(), 0);
		}
	}
}