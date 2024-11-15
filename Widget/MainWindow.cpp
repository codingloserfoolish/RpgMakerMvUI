#include"MainWindow.h"
#include"../manager/EditorLaunch.h"
#include"../manager/ConfigInfo.h"
#include"LauncherWidget.h"
#include<QDockWidget>
#include<QFileDialog>
#include<QRegExpValidator>
#include<QRegExp>
#include<QMessageBox>
namespace hjdd
{
	namespace widget
	{
		SizeLabel::SizeLabel(const QString& text, QWidget* parent):
			QLabel(text,parent){}

		void SizeLabel::mouseDoubleClickEvent(QMouseEvent* event)
		{
			if (event->button() == Qt::LeftButton)
			{
				emit this->doubleClicked();
			}
		}

		MainWindow::MainWindow(QWidget* parent) :
			QMainWindow(parent),
			m_needCompile(true),
			m_isLoaded(false)
		{
		}
		/*
		void MainWindow::loadConfig()
		{
			QFile file("./config/config.txt");
			if (file.open(QIODevice::ReadOnly))
			{
				QString size=file.readLine();
				if (!size.isEmpty())
				{
					QRegExp reg("([0-9]+)x([0-9]+)");
					if (-1 != size.indexOf(reg))
					{
						manager::ConfigInfo::graphicsSize = QSize(reg.cap(1).toInt(), reg.cap(2).toInt());
					}
				}
				else
				{
					manager::ConfigInfo::graphicsSize = QSize(800, 600);
				}
				file.close();
			}
			else
			{
				manager::ConfigInfo::graphicsSize = QSize(800, 600);
			}
		}
		
		void MainWindow::saveConfig()
		{
			QFile file("./config/config.txt");
			if (file.open(QIODevice::WriteOnly))
			{
				QString size = QString("%1x%2").arg(manager::ConfigInfo::graphicsSize.width()).arg(manager::ConfigInfo::graphicsSize.height());
				file.write(size.toUtf8());
				file.close();
			}
		}*/

		void MainWindow::initUI()
		{
			m_graphicsView = new view::GraphicsView(this);
			this->setCentralWidget(m_graphicsView);
			m_graphicsView->setMinimumSize(800,600);
			//菜单栏
			QMenuBar* menuBar = new QMenuBar(this);
			this->setMenuBar(menuBar);
			//文件菜单
			FileMenu* fileMenu = new FileMenu(this);
			menuBar->addMenu(fileMenu);

			CompileMenu* compileMenu = new CompileMenu(this);
			menuBar->addMenu(compileMenu);

			//SizeSettingMenu* sizeMenu = new SizeSettingMenu(this);
			//sizeMenu->initSize(manager::ConfigInfo::graphicsSize);
			//menuBar->addMenu(sizeMenu);

			//树状图
			QDockWidget* treeWidget = new QDockWidget(this);
			treeWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
			m_treeObjectsWidget = new TreeObjects(treeWidget);
			m_treeObjectsWidget->setMinimumWidth(300);

			treeWidget->setWidget(m_treeObjectsWidget);
			this->addDockWidget(Qt::LeftDockWidgetArea, treeWidget);

			//状态栏
			m_sizeEdit = new QLineEdit(this);
			m_sizeInfo = new SizeLabel("800x600", this);
			m_statusInfo = new QLabel("ready", this);
			
			m_sizeEdit->setVisible(false);
			this->statusBar()->addPermanentWidget(m_sizeEdit);
			this->statusBar()->addPermanentWidget(m_sizeInfo);
			this->statusBar()->addPermanentWidget(m_statusInfo);

			//开头，信号连接
			//菜单
			connect(fileMenu, &FileMenu::triggerId, this, &MainWindow::slot_Menu);
			connect(compileMenu, &CompileMenu::triggerId, this, &MainWindow::slot_Menu);
			/*废弃
			connect(sizeMenu, &SizeSettingMenu::sendSizeChanged, [this](const QSize& size) {
				manager::ConfigInfo::graphicsSize = size;
				this->m_graphicsView->setFixedSize(size);
				this->m_treeObjectsWidget->resize(300, size.height());
				this->setFixedHeight(size.height()+68);
				});*/
			//场景与主窗口
			connect((scene::GraphicsScene*)m_graphicsView->scene(), &scene::GraphicsScene::hadChanged, this, &MainWindow::slot_setStatusBarUnsaved);;

			//场景与树状图
			connect((scene::GraphicsScene*)m_graphicsView->scene(), &scene::GraphicsScene::createObject, m_treeObjectsWidget, &TreeObjects::insertItem);
			connect((scene::GraphicsScene*)m_graphicsView->scene(), &scene::GraphicsScene::deleteObject, m_treeObjectsWidget, &TreeObjects::deleteItem);
			connect((scene::GraphicsScene*)m_graphicsView->scene(), &scene::GraphicsScene::selectObject, m_treeObjectsWidget, &TreeObjects::slot_itemSelected);

			//状态栏
			connect(m_sizeInfo, &SizeLabel::doubleClicked, this, &MainWindow::slot_startSizeEdit);
			connect(m_sizeEdit, &QLineEdit::editingFinished, this, &MainWindow::slot_sizeEditFinished);
			//收尾
		}

		void MainWindow::saveGraphics()
		{
			if (scene::GraphicsScene::needToSave)
			{
				xml::XmlParser parser;
				parser.saveSceneToXml(manager::ConfigInfo::instance()->SceneName(), (scene::GraphicsScene*)(m_graphicsView->scene()));
				scene::GraphicsScene::needToSave = false;
				m_needCompile = true;
			}
		}

		bool MainWindow::loadCurrentGraphics()
		{
			xml::XmlParser parser;
			scene::GraphicsScene* scene = (scene::GraphicsScene*)(m_graphicsView->scene());
			if (!parser.loadSceneFromXml(manager::ConfigInfo::instance()->SceneName(), scene))
			{
				return false;
			}
			m_treeObjectsWidget->loadFromScene(scene);
			return true;
		}

		bool MainWindow::loadSelectedGraphics()
		{
			QString filename=QFileDialog::getOpenFileName(nullptr, "选择界面存档(.xml)", manager::ConfigInfo::instance()->savePath(),"Graphics Save(*.xml)");
			if (!filename.isEmpty())
			{
				//////BEGIN 检测文件名、存放位置是否合法以及项目是否完整
				filename.replace('\\', '/');
				int last = filename.lastIndexOf(QRegExp("UIData/[a-zA-Z][a-zA-Z0-9]*.xml$"));
				if (last == -1)
				{
					QMessageBox::information(nullptr, "奇怪的文件", "文件名或路径有误", QMessageBox::Ok);
					return false;
				}
				QString path = filename.left(last);
				if (!QFile::exists(path + "fonts/mplus-1m-regular.ttf"))
				{
					QMessageBox::information(nullptr, "奇怪的文件", "rpgmaker项目缺失", QMessageBox::Ok);
					return false;
				}
				int last_sp = filename.lastIndexOf('/');
				QString sceneName = filename.mid(last_sp+1,filename.length()-last_sp-5);

				//如果导入过了就查重
				if (m_isLoaded)
				{
					if (path + "UIData/" == manager::ConfigInfo::instance()->savePath() && sceneName == manager::ConfigInfo::instance()->SceneName())
						return false;
				}
				//////END

				//通过检测后，先保存界面再加载
				if (scene::GraphicsScene::needToSave)
				{
					this->saveGraphics();
				}
				manager::ConfigInfo::instance()->loadWorkSpace(path);
				manager::ConfigInfo::instance()->setSceneName(sceneName);
				xml::XmlParser parser;
				scene::GraphicsScene* scene = (scene::GraphicsScene*)(m_graphicsView->scene());
				if (!parser.loadSceneFromXml(manager::ConfigInfo::instance()->SceneName(), scene))
				{
					QMessageBox::information(nullptr, "奇怪的文件", "文件内部有误", QMessageBox::Ok);
					return false;
				}
				
				m_treeObjectsWidget->loadFromScene(scene);

				m_needCompile = true;
				//记录新导入的场景
				HistoriesIO hisIo;
				hisIo.loadHistories("./history/history.txt");
				hisIo.recordNewLoad(path, sceneName);

				m_isLoaded = true;

				return true;
			}
		}

		void MainWindow::checkGenerateJsFile(bool checked)
		{
			//先尝试保存
			this->saveGraphics();

			m_statusInfo->setText("saved");
			//接着尝试生成Js
			/*
			* 如果需要保存xml那就需要重新生成
			* 否则就不用重新生成
			* checked是true就需要检测是否需要重新生成
			* 否则就直接重新生成
			*/
			if (m_needCompile||(!checked))
			{
				m_statusInfo->setText("working");
				this->generateJsFile();
				m_statusInfo->setText("completed");
			}
		}

		void MainWindow::generateJsFile()
		{
			js::JsParser parser;
			parser.generateJsFromXml(manager::ConfigInfo::instance()->savePath(),manager::ConfigInfo::instance()->pluginPath(),manager::ConfigInfo::instance()->SceneName());
			m_needCompile = false;
		}

		void MainWindow::slot_Menu(int id)
		{
			if (id == FILE_MENU_SAVE)
			{
				this->saveGraphics();
				m_statusInfo->setText("saved");
			}
			else if (id == FILE_MENU_LOAD)
			{
				if (!this->loadSelectedGraphics())
				{
					m_statusInfo->setText("load fail");
				}
			}
			else if (id == FILE_MENU_JSCOMPILE){this->checkGenerateJsFile(true);}
			else if (id == FILE_MENU_REJSCOMPILE) { this->checkGenerateJsFile(false); }
		}

		void MainWindow::slot_setStatusBarUnsaved()
		{
			m_statusInfo->setText("unsaved");
		}

		void MainWindow::slot_startSizeEdit()
		{
			m_sizeEdit->setText(m_sizeInfo->text());
			m_sizeInfo->setVisible(false);
			m_sizeEdit->setVisible(true);
		}

		void MainWindow::slot_sizeEditFinished()
		{
			QString sizeStr = m_sizeEdit->text();
			if (sizeStr != m_sizeInfo->text())
			{
				//检验输入格式是否正确
				QRegExp reg("^[^0-9]*(\\d+)[^0-9]+(\\d+)[^0-9]*$");
				if (sizeStr.lastIndexOf(reg) == -1)
				{
					return;
				}

				int width = reg.cap(1).toInt();
				int height = reg.cap(2).toInt();
				if (width < 480)width = 480;
				else if (width > 1920)width = 1920;
				if (height < 480)height = 480;
				else if (height > 1080)height = 1080;
				m_sizeInfo->setText(QString("%1x%2").arg(QString::number(width)).arg(QString::number(height)));
				
				this->m_graphicsView->setMinimumSize(width,height);
				this->adjustSize();
			}
			m_sizeEdit->setVisible(false);
			m_sizeInfo->setVisible(true);
		}

		void MainWindow::closeEvent(QCloseEvent* event)
		{
			//this->saveConfig();
			this->saveGraphics();
			manager::EditorLaunch::instance()->closeAll();
			m_graphicsView->releaseScene();
		}
		void MainWindow::resizeEvent(QResizeEvent* event)
		{
			m_sizeInfo->setText(QString("%1x%2").arg(m_graphicsView->width()).arg(m_graphicsView->height()));
		}
		
}
}