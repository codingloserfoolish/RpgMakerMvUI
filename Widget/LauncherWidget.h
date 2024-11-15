#pragma once
#include<QWidget>
#include<QStackedWidget>
#include<QListView>
#include<QMenu>
#include<QPushButton>
#include<QLineEdit>
#include<QLabel>
#include<QAbstractListModel>
#include<QStyledItemDelegate>
#include<QFile>
#include<QTextStream>
#include<QFileDialog>
#include<QDateTime>
#include<QPainter>
#include<QMouseEvent>
#include<QDir>
#include<QMessageBox>
#include"MainWindow.h"
#include"../manager/ObjectLoader.h"
/*
* 这里的_project就是Scene名
* 太多了不好改
* xml保存路径 xxx/UIData/xx.xml
*/

#define MAX_HISTORY_ITEM_COUNT 15

#define PROJECT_ROLE	257
#define PATH_ROLE		258
#define DATE_ROLE		259

//ListView
#define SIGNAL_VIEW 0x0001
#define OPERATOR_EXCHANGED_MAIN		0x0001
#define OPERATOR_MESSAGE_NOFOUND	0x0002

//OptionWidget
#define SIGNAL_OPTION	0x0002
#define OPTION_CREATE	0x0001
#define OPTION_LOAD		0x0002
#define OPTION_TEMPLATE 0x0003

//鼠标右键
#define MENU_TYPE_DELETE			0x0001


#define WELCOME_PAGE	0
#define CREATE_PAGE		1
#define TEMPLATE_PAGE	2

namespace hjdd
{
	namespace widget
	{
		class HistoryListModel;
		class HistoriesIO
		{
		public:
			struct HistoryItem
			{
				QString _project;
				QString _path;
				QString _time;
				HistoryItem* _next;
			};
			
			HistoriesIO();
			~HistoriesIO();
			//project path time;
			void loadHistories(const QString&filename);
			void recordNewCreate(const QString&path,const QString&sceneName);
			void recordNewLoad(const QString& path, const QString& sceneName);
			void removeRecord(int row);
			void updateHistoryItem(int index);
			void saveHistories();
			HistoryItem* item(int row);

			int size() { return m_count >= MAX_HISTORY_ITEM_COUNT ? MAX_HISTORY_ITEM_COUNT - 1 : m_count; }

		private:
			int m_count;
			bool m_hasNew;
			
			HistoryItem* m_head;
			HistoryItem* m_last;
			friend class HistoryListModel;
		};

		class HistoryListModel :public QAbstractListModel
		{
			Q_OBJECT
		public:
			HistoryListModel(QObject* parent = nullptr);
			virtual ~HistoryListModel();
			virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
			virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
			void appendHistoryItems(HistoriesIO&histories);
			void removeHistoryItem(HistoriesIO&histories,int row);
		private:
			int m_itemcount;
			HistoriesIO::HistoryItem** m_histroyItems;
		};

		class HistoryListDelegate :public QStyledItemDelegate
		{
			Q_OBJECT
		public:
			HistoryListDelegate(QObject*parent=nullptr);
			virtual~HistoryListDelegate();
			void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
			QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
		signals:
			void clickedEvent(int type,int row);
		protected:
			bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
		};

		class HisListMenu :public QMenu
		{
			Q_OBJECT
		public:
			HisListMenu(QWidget* parent = nullptr);
			virtual~HisListMenu();
		signals:
			void triggeredType(int type);
		protected:
			virtual bool eventFilter(QObject*, QEvent*)override;
		};

		class HistoryListView:public QListView
		{
			Q_OBJECT
		public:
			HistoryListView(QWidget* parent = nullptr);
			virtual ~HistoryListView();

			void loadHistory();
			void createNewHistory(const QString& path, const QString& sceneName);

			void Closure();
			void removeCallBack();
		signals:
			/*
			* 向LauncherWidget发送指令
			* 可以切换
			* 历史记录不存在，发出消息提示
			* 
			*/
			void OperatorInstruction(int type);
		public slots:
			void slot_responseClickedEvent(int type, int row);
			void slot_menuCentre(int type);
		private:
			int m_clickedRow;;
			HistoryListModel* m_model;
			HistoriesIO m_historiesIO;

			HisListMenu* m_ctxMenu;
		};

		class OptionsWidget :public QWidget
		{
			Q_OBJECT
		public:
			OptionsWidget(QWidget* parent = nullptr);
			virtual~OptionsWidget(){}

			void initUI();
		signals:
			void buttonType(int type);
		};

		class WelcomeSection :public QWidget
		{
			Q_OBJECT
		public:
			WelcomeSection(QWidget* parent=nullptr);
			virtual ~WelcomeSection(){}

			void setupUi(HistoryListView*view, OptionsWidget*option);
		};

		class CreateSection :public QWidget
		{
			Q_OBJECT
		public:
			CreateSection(QWidget* parent = nullptr);
			virtual~CreateSection() {};

			void setupUi();
			QString path()const { return m_path->text(); }
			QString sceneName()const { return m_scene_name->text(); }
		public slots:
			void slot_checkLineEdit();
			void slot_selectPath();
		signals:
			void createEnter();
			void backEnter();
		private:
			QLabel* m_scene_warning;
			QLineEdit* m_scene_name;
			QLabel* m_path_warning;
			QLineEdit* m_path;
		};

		class LauncherWidget:public QStackedWidget
		{
			Q_OBJECT
		public:
			LauncherWidget(QWidget* parent = nullptr);
			virtual~LauncherWidget();
			void setupUi();
			void initLoad();
			void setStartWindow(MainWindow* w) { m_mainWindow = w; }


			void startMainWindow();
			void loadMainWindow();

		public slots:
			void slot_repsonseView(int type);
			void slot_responseOption(int which);
			void slot_createScene();
			void slot_backPage();
		protected:
			virtual void closeEvent(QCloseEvent* event)override;
		private:
			bool m_dllnameLoaded;
			CreateSection* m_createSection;
			HistoryListView* m_historyListView;
			OptionsWidget* m_optionsWidget;
			MainWindow* m_mainWindow;
		};
	} 
}