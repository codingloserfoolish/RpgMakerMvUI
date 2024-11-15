#include"GraphicsScene.h"
#include"../manager/EditorLaunch.h"
#include"../manager/ObjectsFactory.h"

namespace hjdd
{
	namespace scene
	{
		bool GraphicsScene::needToSave = false;
		GraphicsScene::GraphicsScene(QObject* parent):
			QGraphicsScene(parent)
		{
			m_ObjCtxMenu = new widget::ObjectPopCtxMenu;
			connect(m_ObjCtxMenu, &widget::ObjectPopCtxMenu::triggerId, this, &GraphicsScene::objectPopContextMenuEvent);
			/*item::GraphicsItem* item1 = new item::Text;
			((item::Text*)item1)->setText("1235");
			this->addItem(item1);*/
		}
		GraphicsScene::~GraphicsScene()
		{
			delete m_ObjCtxMenu;
		}
		bool GraphicsScene::existActiveObject()
		{
			return item::GraphicsItem::_selected_item && item::GraphicsItem::_selected_item->isActive();
		}
		void GraphicsScene::deactiveObj()
		{
			item::GraphicsItem* pre = item::GraphicsItem::_selected_item;
			item::GraphicsItem::_selected_item = 0;
			if (pre)pre->update();
		}

		item::GraphicsItem* GraphicsScene::appendItem(int type)
		{
			item::GraphicsItem* item = manager::ObjectsFactory::instance()->create(type);
			if (this->existActiveObject())
			{
				item::GraphicsItem::_selected_item->addChild(item);
				emit createObject(item, item::GraphicsItem::_selected_item->standardItem);
			}
			else
			{
				this->addItem(item);
				m_directItems.push_back(item);
				emit createObject(item, 0);
			}
			return item;
		}

		void GraphicsScene::resetScene()
		{
			for (QGraphicsItem* item : m_directItems)
			{
				this->removeItem(item);
				delete item;
			}
			m_directItems.clear();
			item::GraphicsItem::_selected_item = 0;
			GraphicsScene::needToSave = false;
		}

		void GraphicsScene::sendChangedMessage()
		{
			if (!GraphicsScene::needToSave)
			{
				GraphicsScene::needToSave = true;
				emit this->hadChanged();
			}
		}

		void GraphicsScene::objectPopContextMenuEvent(int id, int param)
		{
			if (id == OBJECT_MENU_EDIT)
			{
				if (this->existActiveObject())
				{
					manager::EditorLaunch::instance()->showEditor(item::GraphicsItem::_selected_item);

					//保存设置
					this->sendChangedMessage();
				}
			}
			else if (id == OBJECT_MENU_CREATE)
			{
				appendItem(param);

				//保存设置
				this->sendChangedMessage();
			}
			else if(id==OBJECT_MENU_DELETE)
			{
				if (this->existActiveObject())
				{
					//通知TreeObjects删除相关节点
					emit deleteObject(item::GraphicsItem::_selected_item);
					//如果删除物体，必须断开它与其他控件的连接
					manager::EditorLaunch::instance()->closeEditor(item::GraphicsItem::_selected_item);
					manager::EditorLaunch::instance()->releaseConnection();
					
					//从直接子对象中移除
					if (!item::GraphicsItem::_selected_item->parentItem())
					{
						m_directItems.removeOne(item::GraphicsItem::_selected_item);
					}
					this->removeItem(item::GraphicsItem::_selected_item);
					delete item::GraphicsItem::_selected_item;
					item::GraphicsItem::_selected_item = 0;
					
					
					//保存设置
					this->sendChangedMessage();
				}
			}
		}

		void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
		{
			//先让物体失活
			if (item::GraphicsItem::_selected_item)item::GraphicsItem::_selected_item->deactive();
			QGraphicsScene::mousePressEvent(mouseEvent);
			if (!this->existActiveObject())
			{
				this->deactiveObj();
			}
			else
			{
				emit selectObject(item::GraphicsItem::_selected_item->standardItem);
				if (manager::EditorLaunch::instance()->isOpen())
				{
					manager::EditorLaunch::instance()->showEditor(item::GraphicsItem::_selected_item);
				}

				//保存设置
				this->sendChangedMessage();
			}
		}
		void GraphicsScene::keyReleaseEvent(QKeyEvent* keyEvent)
		{
			if (keyEvent->key() == Qt::Key_Escape)
			{
				this->deactiveObj();
			}
		}
		void GraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* contextMenuEvent)
		{
			m_ObjCtxMenu->exec(QCursor::pos());
		}
	}
}