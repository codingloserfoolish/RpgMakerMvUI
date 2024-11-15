#pragma once
#include<QTreeView>
#include<QStandardItemModel>
#include"../item/GraphicsItem.h"
#include"../scene/GraphicsScene.h"

//详细见deleteItem
#define STATUE_DELETE 0x00000000
#define STATUE_NORMAL 0x00000001

namespace hjdd
{
	namespace widget
	{

		class ObjectsItem :public QStandardItem
		{
		public:
			ObjectsItem(const QString& text, item::GraphicsItem*item);
			item::GraphicsItem* m_object;
		};



		class TreeObjects :public QTreeView
		{
			Q_OBJECT
		public:
			TreeObjects(QWidget* parent = nullptr);
			virtual~TreeObjects();
			/*
			* 如果找到与name相同的名字对应的value加一，返回该值
			* 否则加入该名字，对应的value设为0，返回-1
			*/
			int nameSameIndex(const QString&name);
			/*
			* 如果item没有名字则会分配一个object_(value)，返回前缀名字
			* 否则注册item的名字，返回前缀名字
			* 该函数不会处理重命名的物体，该函数默认所有传入的物体名字不相同
			*/
			QString registerObjectName(item::GraphicsItem*item,int*ret);

			void removeSameName(const QString& name);
			void insertItem(item::GraphicsItem*item,QStandardItem*parent=0);
			/*
			* 删除项
			* 出现的bug，删除项的时候会触发currentChanged于是会更改item::item::GraphicsItem，
			* 导致被删除物体删去ObjectItem而新选中的删除了GraphicsItem，各删一半
			*/
			void deleteItem(item::GraphicsItem* item);
			//直接从场景里加载树形图，会阻塞model信号
			void loadFromScene(scene::GraphicsScene* scene);

		public slots:
			void slot_itemChanged(QStandardItem*item);
			void slot_itemSelected(QStandardItem*item);
		protected:
			virtual void rowsInserted(const QModelIndex& parent, int start, int end);
			virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous);

		private:

			QStandardItem* insertItemForBlockSignals(item::GraphicsItem*item);


			int m_changeStatue;
			QStandardItemModel* m_model;
			QMap<QString, int> m_objectsName;
		};
	}
}