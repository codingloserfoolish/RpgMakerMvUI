#include"TreeObjects.h"
#include<QRegularExpression>
#include<QDebug>
namespace hjdd
{
	namespace widget
	{
		ObjectsItem::ObjectsItem(const QString& text, item::GraphicsItem*item):
			QStandardItem(text),
			m_object(item)
		{
		}

		TreeObjects::TreeObjects(QWidget* parent):
			QTreeView(parent),
			m_changeStatue(STATUE_NORMAL)
		{
			this->setWindowTitle("TreeView");
			m_model = new QStandardItemModel(this);
			m_model->setHorizontalHeaderLabels({"名字"});
			this->setModel(m_model);
			connect(m_model, &QStandardItemModel::itemChanged, this, &TreeObjects::slot_itemChanged);
		}
		TreeObjects::~TreeObjects()
		{
		}
		int TreeObjects::nameSameIndex(const QString& name)
		{
			QMap<QString, int>::iterator it = m_objectsName.find(name);
			if (it != m_objectsName.end())
			{
				return ++it.value();
			}
			m_objectsName.insert(name, 0);
			return -1;
		}

		QString TreeObjects::registerObjectName(item::GraphicsItem* item,int* ret)
		{
			QString name = item->objectName();
			if (name.isEmpty())
			{
				*ret = nameSameIndex("object");
				item->setObjectName(*ret ==-1?QString("object"):QString("object_%1").arg(*ret));
				return "object";
			}
			QRegExp rep("_(\\d+)$");
			int pos = rep.lastIndexIn(name);
			if (pos != -1)
			{
				name = name.left(pos);
			}
			*ret = rep.cap(1).toInt();
			QMap<QString, int>::iterator it = m_objectsName.find(name);
			if (it != m_objectsName.end())
			{
				if (it.value() < *ret)
				{
					it.value() = *ret;
				}
			}
			else
			{
				m_objectsName.insert(name, *ret);
			}
			return name;
		}

		void TreeObjects::removeSameName(const QString& name)
		{
			QRegExp rep("_(\\d+)$");
			int pos = rep.lastIndexIn(name);
			QMap<QString, int>::iterator it=m_objectsName.end();
			QString org_name = pos == -1 ? name : name.left(pos);
			it = m_objectsName.find(org_name);
			if (it != m_objectsName.end())
			{
				if (it.value() == 0)
				{
					m_objectsName.remove(org_name);
				}
				else if (it.value() <= rep.cap(1).toInt())
				{
					--it.value();
				}
			}
		}

		void TreeObjects::insertItem(item::GraphicsItem* item, QStandardItem* parent)
		{
			QStandardItem* standardItem = new ObjectsItem("object", item);
			item->setObjectName("object");
			item->standardItem = standardItem;
			if (parent)parent->insertRow(m_model->rowCount(parent->index()), standardItem);
			else m_model->insertRow(m_model->rowCount(), standardItem);
		}

		void TreeObjects::deleteItem(item::GraphicsItem* item)
		{
			if (item && item->standardItem)
			{
				removeSameName(item->objectName());
				const QModelIndex standardIndex = item->standardItem->index();
				m_changeStatue = STATUE_DELETE;
				m_model->removeRow(standardIndex.row(), standardIndex.parent());
				m_changeStatue = STATUE_NORMAL;
			}
		}

		void TreeObjects::loadFromScene(scene::GraphicsScene* scene)
		{
			m_objectsName.clear();
			m_model->removeRows(0, m_model->rowCount());
			QStandardItem* root = m_model->invisibleRootItem();
			m_model->blockSignals(true);
			for (QGraphicsItem* item : scene->childItems())
			{
				root->insertRow(root->rowCount(), insertItemForBlockSignals((item::GraphicsItem*)item));
			}
			m_model->blockSignals(false);
			this->reset();
		}
		
		void TreeObjects::slot_itemChanged(QStandardItem* item)
		{
			if (item == m_model->invisibleRootItem())return;
			QRegExp rep("\\(\\d+\\)");
			QString name = item->data(0).toString();
			int len = rep.lastIndexIn(name);
			if (len != -1)
			{
				name = name.left(len);
			}
			int index = nameSameIndex(name);
			if (-1!= index)
			{
				m_model->blockSignals(true);
				item->setText(name+QString("(%1)").arg(index));
				((ObjectsItem*)item)->m_object->setObjectName(name+QString("_%1").arg(index));
				m_model->blockSignals(false);
			}
			else
			{
				((ObjectsItem*)item)->m_object->setObjectName(name);
			}
		}


		void TreeObjects::slot_itemSelected(QStandardItem* item)
		{
			this->setCurrentIndex(item->index());
		}

		void TreeObjects::rowsInserted(const QModelIndex& parent, int start, int end)
		{
			emit m_model->itemChanged(m_model->itemFromIndex(m_model->index(start,0,parent)));
			QTreeView::rowsInserted(parent, start, end);
		}

		void TreeObjects::currentChanged(const QModelIndex& current, const QModelIndex& previous)
		{
			if (m_changeStatue==STATUE_DELETE||!current.isValid())return;
			ObjectsItem* item = (ObjectsItem*)m_model->itemFromIndex(current);
			if (item::GraphicsItem::_selected_item != item->m_object)
			{
				item::GraphicsItem* pre = item::GraphicsItem::_selected_item;
				item::GraphicsItem::_selected_item = item->m_object;
				if(pre)pre->update();
				item->m_object->update();
			}
		}

		QStandardItem* TreeObjects::insertItemForBlockSignals(item::GraphicsItem* item)
		{
			//开头，注册名字
			int index = 0;
			QString prefix = registerObjectName(item,&index);
			if (index>0)
			{
				prefix += QString("(%1)").arg(QString::number(index));
			}
			//收尾
			
			//开头，创建自己
			ObjectsItem* standardItem = new ObjectsItem(prefix, item);
			item->standardItem = standardItem;
			//收尾

			for (QGraphicsItem* child : item->childItems())
			{
				standardItem->insertRow(standardItem->rowCount(),insertItemForBlockSignals((item::GraphicsItem*)child));
			}
			return standardItem;
		}
		
	}
}