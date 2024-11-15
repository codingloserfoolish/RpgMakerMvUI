#include"ElementDisplayWidget.h"
#include"../ElementEditManager/ElementsFactory.h"

namespace hjdd
{
	namespace widget
	{
		ElementDisplayWidget::ElementDisplayWidget(QWidget* parent):
			QWidget(parent),
			m_winBase(0)
		{
			m_ctxMenu = new ElementPopCtxMenu;
			connect(m_ctxMenu, &ElementPopCtxMenu::triggerId, this, &ElementDisplayWidget::slot_popContxtOperator);
			connect(manager::ElementEditorManager::instance().get(), &manager::ElementEditorManager::propertiesChanged, this, &ElementDisplayWidget::slot_updateEditDisplay);
		}
		ElementDisplayWidget::~ElementDisplayWidget()
		{
			if (m_ctxMenu)delete m_ctxMenu;
		}
		elements::Element* ElementDisplayWidget::pickup(const QPoint& pos)
		{
			for (elements::Element* item : m_winBase->elements())
			{
				if (item->Rect().contains(pos))return item;
			}
			return 0;
		}

		void ElementDisplayWidget::removeElement(elements::Element* elem)
		{
			std::vector<elements::Element*>& elements = m_winBase->elements();
			std::vector<elements::Element*>::iterator it= elements.end();
			for (it = elements.begin(); it != elements.end(); ++it)
			{
				if ((*it) == elem)break;
			}
			if (it != elements.end())
			{
				elements.erase(it);
				manager::ElementEditorManager::instance()->clearDeletedElementConnect(elem);
				delete elem;
			}
		}

		void ElementDisplayWidget::setObject(item::WindowBase* item)
		{
			if (item==m_winBase)return;
			//先断开之前物体的连接
			manager::ElementEditorManager::instance()->close();
			m_winBase = item;
			this->update();
		}

		void ElementDisplayWidget::clearElements()
		{
			for (std::vector<elements::Element*>::iterator it = m_winBase->elements().begin(); it != m_winBase->elements().end(); ++it)
			{
				delete (*it);
			}
		}

		void ElementDisplayWidget::slot_updateEditDisplay()
		{
			if (elements::Element::_selected_item && elements::Element::_selected_item->isActive())
			{
				this->update(elements::Element::_selected_item->CompleteRect());
				emit this->elementChanged();
			}
		}

		void ElementDisplayWidget::slot_popContxtOperator(int type, int param)
		{
			if (type == OBJECT_MENU_CREATE)
			{
				elements::Element* ele = manager::ElementsFactory::instance()->create(param);
				if (ele)
				{
					m_winBase->elements().push_back(ele);
					this->update(ele->CompleteRect());
					emit this->elementChanged();
				}
			}
			else if (type == OBJECT_MENU_DELETE)
			{
				if (elements::Element::_selected_item && elements::Element::_selected_item->isActive())
				{
					this->removeElement(elements::Element::_selected_item);
					elements::Element::_selected_item = 0;
					emit this->elementChanged();
				}
			}
			else if (type == OBJECT_MENU_EDIT)
			{
				if (elements::Element::_selected_item && elements::Element::_selected_item->isActive())
				{
					manager::ElementEditorManager::instance()->show(elements::Element::_selected_item);
				}
			}
		}

		void ElementDisplayWidget::contextMenuEvent(QContextMenuEvent* event)
		{
			if (m_ctxMenu)
			{
				m_ctxMenu->exec(event->globalPos());
			}
		}
		void ElementDisplayWidget::paintEvent(QPaintEvent* e)
		{
			if (!m_winBase)return;
			QPainter  p(this);
			p.setPen(Qt::red);
			p.drawRect(18,18,m_winBase->width()-36,m_winBase->height()-36);
			for (elements::Element* item : m_winBase->elements())
			{
				p.save();
				p.translate(item->pos());
				item->paint(&p);
				p.restore();
			}
			p.end();
		}
		void ElementDisplayWidget::mousePressEvent(QMouseEvent* e)
		{
			if (e->button() == Qt::LeftButton)
			{
				elements::Element* item = this->pickup(e->pos());
				if (item)
				{
					elements::Element* old = elements::Element::_selected_item;
					item->mouseEvent(e, MOUSE_PRESS);
					if (old != item)
					{
						this->update(item->CompleteRect());
					}
				}
				else
				{
					if (elements::Element::_selected_item&&elements::Element::_selected_item->isActive())
					{
						elements::Element::_selected_item->deactive();
						this->update(elements::Element::_selected_item->CompleteRect());
					}
				}
			}
		}
		void ElementDisplayWidget::mouseMoveEvent(QMouseEvent* e)
		{
			if ((e->buttons() & Qt::LeftButton)&&elements::Element::_selected_item&&elements::Element::_selected_item->isActive())
			{
				elements::Element::_selected_item->mouseEvent(e, MOUSE_MOVE);
				this->update();
				emit this->elementChanged();
			}
		}
		void ElementDisplayWidget::mouseReleaseEvent(QMouseEvent* e)
		{
			if (e->button() == Qt::LeftButton && elements::Element::_selected_item && elements::Element::_selected_item->isActive())
			{
				elements::Element::_selected_item->mouseEvent(e, MOUSE_RELEASE);
			}
		}
	}
}