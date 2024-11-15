#pragma once
#include"../WindowBaseElement/IWinBaseElement.h"
#include"../ElementEditManager/ElementEditorManager.h"
#include<QWidget>
#include<QMenu>
#include<vector>
#include"Menu.h"
#include"../../rpgobject/WindowBase.h"
namespace hjdd
{
	namespace widget
	{
		class ElementDisplayWidget :public QWidget
		{
			Q_OBJECT
		public:
			ElementDisplayWidget(QWidget* parent = nullptr);
			virtual~ElementDisplayWidget();
			elements::Element* pickup(const QPoint&pos);
			void removeElement(elements::Element*elem);
			void setObject(item::WindowBase* item);

			void clearElements();

		signals:
			void elementChanged();
		public slots:
			void slot_updateEditDisplay();
			void slot_popContxtOperator(int type,int param);
		protected:
			virtual void contextMenuEvent(QContextMenuEvent* event);
			virtual void paintEvent(QPaintEvent* e)override;
			virtual void mousePressEvent(QMouseEvent* e);
			virtual void mouseMoveEvent(QMouseEvent* e);
			virtual void mouseReleaseEvent(QMouseEvent* e);
		private:
			item::WindowBase* m_winBase;
			ElementPopCtxMenu* m_ctxMenu;
		};
	}
}