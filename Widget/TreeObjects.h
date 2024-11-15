#pragma once
#include<QTreeView>
#include<QStandardItemModel>
#include"../item/GraphicsItem.h"
#include"../scene/GraphicsScene.h"

//��ϸ��deleteItem
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
			* ����ҵ���name��ͬ�����ֶ�Ӧ��value��һ�����ظ�ֵ
			* �����������֣���Ӧ��value��Ϊ0������-1
			*/
			int nameSameIndex(const QString&name);
			/*
			* ���itemû������������һ��object_(value)������ǰ׺����
			* ����ע��item�����֣�����ǰ׺����
			* �ú������ᴦ�������������壬�ú���Ĭ�����д�����������ֲ���ͬ
			*/
			QString registerObjectName(item::GraphicsItem*item,int*ret);

			void removeSameName(const QString& name);
			void insertItem(item::GraphicsItem*item,QStandardItem*parent=0);
			/*
			* ɾ����
			* ���ֵ�bug��ɾ�����ʱ��ᴥ��currentChanged���ǻ����item::item::GraphicsItem��
			* ���±�ɾ������ɾȥObjectItem����ѡ�е�ɾ����GraphicsItem����ɾһ��
			*/
			void deleteItem(item::GraphicsItem* item);
			//ֱ�Ӵӳ������������ͼ��������model�ź�
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