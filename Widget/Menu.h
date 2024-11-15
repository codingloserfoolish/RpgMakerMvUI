#pragma once
#include<QMenu>
#include<QLineEdit>
#include<QPushButton>
#include<QDialog>

#define OBJECT_MENU_DELETE	0x0000
#define OBJECT_MENU_CREATE	0x0001
#define OBJECT_MENU_EDIT	0x0002

#define FILE_MENU_SAVE			0x0001
#define FILE_MENU_LOAD			0x0002
#define FILE_MENU_JSCOMPILE		0x0003
#define FILE_MENU_REJSCOMPILE	0x0004
#define ACTION(name,signalFunc,...) connect(this->addAction(name), &QAction::triggered,this,std::bind([this](){emit this->signalFunc(__VA_ARGS__);}))


namespace hjdd
{
	namespace widget
	{
		class ObjectsListMenu :public QMenu
		{
			Q_OBJECT
		public:
			ObjectsListMenu(QWidget* parent = nullptr);
			virtual~ObjectsListMenu(){}
		signals:
			void rpgobject_type(int type);
		protected:
			virtual bool eventFilter(QObject* obj, QEvent* e)override;
		};
		class ObjectPopCtxMenu:public QMenu
		{
			Q_OBJECT
		public:
			ObjectPopCtxMenu(QWidget* parent = nullptr);
			virtual~ObjectPopCtxMenu() {}
		signals:
			void triggerId(int id,int param);
		};


		class FileMenu:public QMenu
		{
			Q_OBJECT
		public:
			FileMenu(QWidget* parent=nullptr);
			virtual~FileMenu(){}
		signals:
			void triggerId(int id);
		};

		class CompileMenu :public QMenu
		{
			Q_OBJECT
		public:
			CompileMenu(QWidget* parent = nullptr);
			virtual~CompileMenu() {}
		signals:
			void triggerId(int id);
		};

		/*·ÏÆú
		class SizeSettingDlg :public QDialog
		{
			Q_OBJECT
		public:
			SizeSettingDlg(const QSize&size,QWidget* parent = nullptr);
			virtual~SizeSettingDlg() {};
			int swidth()const { return m_width; }
			int sheight()const { return m_height; }
		protected:
			virtual void closeEvent(QCloseEvent*e);
		private:
			QLineEdit* m_widthEdit;
			QLineEdit* m_heightEdit;
			int m_width;
			int m_height;
		};


		class SizeSettingMenu :public QMenu
		{
			Q_OBJECT
		public:
			SizeSettingMenu(QWidget* parent = nullptr);
			virtual~SizeSettingMenu() {}
			void initSize(const QSize& size);
		signals:
			void sendSizeChanged(QSize);
		public slots:
			void slot_triggerAction(QAction* action);
			void slot_selfTriggered();
		private:
			QActionGroup* m_group;
			QAction* m_selected;
			QAction* m_self;
		};
		*/
	}
}