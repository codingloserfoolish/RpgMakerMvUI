#pragma once
#include<QMainWindow>
#include<QMenuBar>
#include<QLabel>
#include"../view/GraphicsView.h"
#include"../xmlParser/xmlParser.h"
#include"../javascriptParser/JsParser.h"
#include"TreeObjects.h"
#include<QStatusBar>
#include"Menu.h"
namespace hjdd
{
	namespace widget
	{

		class SizeLabel :public QLabel
		{
			Q_OBJECT
		public:
			SizeLabel(const QString&text,QWidget* parent = nullptr);
			virtual~SizeLabel() {}
		signals:
			void doubleClicked();
		protected:
			virtual void mouseDoubleClickEvent(QMouseEvent* event);
		};

		class MainWindow :public QMainWindow
		{
			Q_OBJECT
		public:
			MainWindow(QWidget*parent=nullptr);
			virtual~MainWindow() {}

			//void loadConfig();
			//void saveConfig();

			void initUI();

			void saveGraphics();
			bool loadCurrentGraphics();
			bool loadSelectedGraphics();
			void checkGenerateJsFile(bool checked);
			void generateJsFile();
		public slots:
			void slot_Menu(int id);
			void slot_setStatusBarUnsaved();
			void slot_startSizeEdit();
			void slot_sizeEditFinished();
		protected:
			virtual void closeEvent(QCloseEvent* event);
			virtual void resizeEvent(QResizeEvent* event);
		private:
			bool m_isLoaded;
			bool m_needCompile;
			TreeObjects* m_treeObjectsWidget;
			view::GraphicsView* m_graphicsView;
			QLabel* m_statusInfo;
			SizeLabel* m_sizeInfo;
			QLineEdit* m_sizeEdit;
		};
	}
}