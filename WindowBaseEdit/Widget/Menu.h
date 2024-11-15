#pragma once
#include<QMenu>
#include<QLineEdit>
#include<QPushButton>
#include<QDialog>

#define OBJECT_MENU_DELETE	0x0000
#define OBJECT_MENU_CREATE	0x0001
#define OBJECT_MENU_EDIT	0x0002

#define ACTION(name,signalFunc,...) connect(this->addAction(name), &QAction::triggered,this,std::bind([this](){emit this->signalFunc(__VA_ARGS__);}))


namespace hjdd
{
	namespace widget
	{
		class ElementsListMenu :public QMenu
		{
			Q_OBJECT
		public:
			ElementsListMenu(QWidget* parent = nullptr);
			virtual~ElementsListMenu(){}
		signals:
			void element_type(int type);
		protected:
			virtual bool eventFilter(QObject* obj, QEvent* e)override;
		};
		class ElementPopCtxMenu:public QMenu
		{
			Q_OBJECT
		public:
			ElementPopCtxMenu(QWidget* parent = nullptr);
			virtual~ElementPopCtxMenu() {}
		signals:
			void triggerId(int id,int param);
		};
	}
}