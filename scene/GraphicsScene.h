#pragma once
#include<QGraphicsScene>
#include<QKeyEvent>
#include"../widget/Menu.h"
#include<QStandardItem>

namespace hjdd
{
	namespace item { class GraphicsItem; }
	namespace scene
	{
		/*
		* 该类不会释放QGraphicsItem
		* 需要手动delete
		*/
		class GraphicsScene :public QGraphicsScene
		{
			Q_OBJECT
		public:
			GraphicsScene(QObject* parent = nullptr);
			virtual~GraphicsScene();

			bool existActiveObject();
			void deactiveObj();
			QList<item::GraphicsItem*>& childItems() { return m_directItems; }
			//添加物体
			item::GraphicsItem* appendItem(int type);

			/*
			* 重置场景参数
			* m_exist_active_obj
			* m_directItems
			* item::GraphicsItem::_selected_item
			*/
			void resetScene();

			void sendChangedMessage();

			static bool needToSave;
		signals:
			void createObject(item::GraphicsItem*item,QStandardItem*parent);
			void deleteObject(item::GraphicsItem*item);
			void selectObject(QStandardItem*item);
			//通知主窗口，场景窗口有变换，只通知一次
			void hadChanged();
		public slots:
			void objectPopContextMenuEvent(int id,int param);
		protected:
			virtual void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
			virtual void keyReleaseEvent(QKeyEvent* keyEvent);
			virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* contextMenuEvent);

		private:
			//直接子对象
			QList<item::GraphicsItem*>m_directItems;
			widget::ObjectPopCtxMenu* m_ObjCtxMenu;
		};
	}
}