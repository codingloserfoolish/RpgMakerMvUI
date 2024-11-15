#include"LauncherWidget.h"
#include<QHBoxLayout>
#include<QRegExp>
#include"../manager/ConfigInfo.h"
#include"../manager/EditorLaunch.h"
#include<QDebug>
namespace hjdd
{
	namespace widget
	{

		HistoriesIO::HistoriesIO() :
			m_count(0),
			m_hasNew(false),
			m_head(0),
			m_last(0)
		{
		}

		HistoriesIO::~HistoriesIO()
		{
			if (m_head)
			{
				HistoryItem* before = m_head;
				HistoryItem* next = m_head->_next;
				while (next)
				{
					delete before;
					before = next;
					next = next->_next;
				}
				delete before;
			}
		}

		void HistoriesIO::loadHistories(const QString& filename)
		{
			if (m_head)return;
			QFile file(filename);
			if (!file.open(QIODevice::ReadOnly))return;
			QTextStream stream(&file);

			m_head = new HistoryItem;
			m_head->_next = 0;
			m_last = m_head;

			while (m_count < MAX_HISTORY_ITEM_COUNT-1&& !stream.atEnd())
			{
				HistoryItem* item = new HistoryItem;
				item->_project = stream.readLine();
				item->_path = stream.readLine();
				item->_time = stream.readLine();
				item->_next = 0;
				m_last->_next = item;
				m_last = item;
				++m_count;
			}
			file.close();
		}

		void HistoriesIO::recordNewCreate(const QString& path, const QString& sceneName)
		{
			m_head->_project = sceneName;
			m_head->_path = path;
			m_head->_time = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm");
			m_hasNew = true;
		}

		void HistoriesIO::recordNewLoad(const QString& path, const QString& sceneName)
		{
			//先查重
			HistoryItem* back = m_head;
			HistoryItem* next = m_head->_next;
			while (next)
			{
				if (next->_path == path && next->_project == sceneName)break;
				back = next;
				next = next->_next;
			}
			if (next)
			{
				next->_time = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm");
				back->_next = next->_next;
				next->_next = m_head->_next;
				m_head->_next = next;

				//back可能是m_head;
				if (back->_next == 0)m_last = back;
				else m_last = back->_next;
			}
			else
			{
				m_head->_path = path;
				m_head->_project = sceneName;
				m_head->_time = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm");
				++m_count;
			}
			m_hasNew = true;
		}

		void HistoriesIO::removeRecord(int row)
		{
			HistoryItem* back = m_head;
			HistoryItem* next = m_head->_next;
			while (next && row)
			{
				back = next;
				next = next->_next;
				--row;
			}
			if (next)
			{
				if (next->_next == 0)m_last = back;
				back->_next = next->_next;
				delete next;
				--m_count;
				m_hasNew = true;
			}
		}

		void HistoriesIO::updateHistoryItem(int index)
		{
			HistoryItem* back = m_head;
			HistoryItem* next = m_head->_next;

			while (next && index) { back = next; next = next->_next; --index; }
			if (next)
			{
				next->_time = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm");
				back->_next = next->_next;
				next->_next = m_head->_next;
				m_head->_next = next;
				
				//back可能是m_head;
				if (back->_next == 0)m_last = back;
				else m_last = back->_next;

				m_hasNew = true;
			}
		}

		void HistoriesIO::saveHistories()
		{
			if (m_hasNew)
			{
				QFile save("./history/history.txt");
				if (save.open(QIODevice::WriteOnly))
				{
					QTextStream stream(&save);
					HistoryItem* next = m_head->_project.isEmpty() ? m_head->_next : m_head;
					int count = MAX_HISTORY_ITEM_COUNT;
					while (next&&(--count))
					{
						stream << next->_project << "\n";
						stream << next->_path << "\n";
						stream << next->_time << "\n";
						next = next->_next;
					}
					save.close();
					m_hasNew = false;
				}
			}
		}
		HistoriesIO::HistoryItem* HistoriesIO::item(int row)
		{
			HistoryItem* next = m_head->_next;
			while (row && next) { next = next->_next; --row; }
			return next;
		}
		//------------
		HistoryListModel::HistoryListModel(QObject* parent) :
			QAbstractListModel(parent),
			m_itemcount(0),
			m_histroyItems(0)
		{
		}
		HistoryListModel::~HistoryListModel()
		{
			if (m_histroyItems)delete[]m_histroyItems;
		}
		int HistoryListModel::rowCount(const QModelIndex& parent) const
		{
			return parent.isValid() ? 0 : (m_itemcount>=MAX_HISTORY_ITEM_COUNT? MAX_HISTORY_ITEM_COUNT-1: m_itemcount);
		}
		QVariant HistoryListModel::data(const QModelIndex& index, int role) const
		{
			if (!index.isValid())return QVariant();
			switch (role)
			{
			case PROJECT_ROLE:
				return m_histroyItems[index.row()]->_project;
			case PATH_ROLE:
				return m_histroyItems[index.row()]->_path;
			case DATE_ROLE:
				return m_histroyItems[index.row()]->_time;
			default:
				break;
			}
			return QVariant();
		}
		void HistoryListModel::appendHistoryItems(HistoriesIO& histories)
		{
			if (!m_histroyItems)
			{
				HistoriesIO::HistoryItem* next = histories.m_head->_next;
				m_itemcount = histories.size();
				m_histroyItems = new HistoriesIO::HistoryItem * [m_itemcount];

				for (int i = 0;next&&i< m_itemcount; ++i, next = next->_next) { m_histroyItems[i] = next; }
			}
		}
		void HistoryListModel::removeHistoryItem(HistoriesIO& histories, int row)
		{
			/*
			* len=2
			* [0 1]
			* 
			*/
			beginRemoveRows(QModelIndex(), row, row);
			histories.removeRecord(row);
			while (row < m_itemcount-1)
			{
				*(m_histroyItems + row) = *(m_histroyItems + row + 1);
				++row;
			}
			*(m_histroyItems + row) = 0;
			--m_itemcount;
			endRemoveRows();

		}
		//-----------
		HistoryListDelegate::HistoryListDelegate(QObject* parent) :
			QStyledItemDelegate(parent)
		{
		}

		HistoryListDelegate::~HistoryListDelegate()
		{
		}

		void HistoryListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
		{
			QRect rect = option.rect;

			if (option.state & QStyle::State_MouseOver)
			{
				painter->fillRect(rect, Qt::gray);
			}
			QFont font = painter->font();
			font.setPixelSize(36);
			painter->setFont(font);
			painter->drawText(rect, Qt::AlignLeft, index.data(PROJECT_ROLE).toString());

			font.setPixelSize(24);
			painter->setFont(font);
			painter->drawText(rect, Qt::AlignBottom, index.data(PATH_ROLE).toString());

			font.setPixelSize(16);
			painter->setFont(font);
			painter->drawText(rect, Qt::AlignRight | Qt::AlignCenter, index.data(DATE_ROLE).toString());

		}

		QSize HistoryListDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
		{
			return QSize(option.rect.width(), 96);
		}

		bool HistoryListDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
		{
			if (event->type() == QEvent::MouseButtonPress)
			{
				QMouseEvent* m_e = (QMouseEvent*)event;
				if (m_e->button() == Qt::LeftButton)
				{
					emit this->clickedEvent(Qt::LeftButton, index.row());
					return true;
				}
				else if (m_e->button() == Qt::RightButton)
				{
					emit this->clickedEvent(Qt::RightButton, index.row());
					return true;
				}
			}
			return false;
		}
		//----------------
		HisListMenu::HisListMenu(QWidget* parent):
			QMenu(parent)
		{
			this->installEventFilter(this);
			this->addAction("删除");
		}

		HisListMenu::~HisListMenu()
		{
		}

		bool HisListMenu::eventFilter(QObject*obj, QEvent*e)
		{
			if (e->type() == QEvent::MouseButtonPress)
			{
				QMouseEvent* m_e = (QMouseEvent*)e;
				if (m_e->button() == Qt::LeftButton)
				{
					this->triggeredType(MENU_TYPE_DELETE);
					this->close();
					return true;
				}
			}
			return QMenu::eventFilter(obj,e);
		}

		//----------------
		HistoryListView::HistoryListView(QWidget* parent) :
			QListView(parent),
			m_clickedRow(-1)
		{
			m_model = new HistoryListModel(this);
			this->setModel(m_model);
			HistoryListDelegate* his_delegate = new HistoryListDelegate(this);
			this->setItemDelegate(his_delegate);

			m_ctxMenu = new HisListMenu;


			connect(his_delegate, &HistoryListDelegate::clickedEvent, this, &HistoryListView::slot_responseClickedEvent);
			connect(m_ctxMenu, &HisListMenu::triggeredType, this, &HistoryListView::slot_menuCentre);
		}

		HistoryListView::~HistoryListView()
		{
			if (m_ctxMenu)delete m_ctxMenu;
		}

		void HistoryListView::loadHistory()
		{
			m_historiesIO.loadHistories("./history/history.txt");
			m_model->appendHistoryItems(m_historiesIO);
		}

		void HistoryListView::createNewHistory(const QString& path, const QString& sceneName)
		{
			m_historiesIO.recordNewCreate(path, sceneName);
		}

		void HistoryListView::Closure()
		{
			m_historiesIO.saveHistories();
		}

		void HistoryListView::removeCallBack()
		{
			if (m_clickedRow != -1)
			{
				m_model->removeHistoryItem(m_historiesIO, m_clickedRow);
				m_clickedRow = -1;
			}
		}

		void HistoryListView::slot_menuCentre(int type)
		{
			if (type == MENU_TYPE_DELETE)
			{
				if (m_clickedRow != -1)
				{
					m_model->removeHistoryItem(m_historiesIO, m_clickedRow);
				}
			}
		}

		void HistoryListView::slot_responseClickedEvent(int type, int row)
		{
			m_clickedRow = row;
			switch (type)
			{
			case Qt::LeftButton:
			{
				HistoriesIO::HistoryItem* item = m_historiesIO.item(row);
				if (item)
				{
					QString workspace = item->_path;
					QString saveFile = workspace + "UIData/"+item->_project+".xml";
					if (QFile::exists(saveFile)&& manager::ConfigInfo::instance()->loadWorkSpace(workspace))
					{
						manager::ConfigInfo::instance()->setSceneName(item->_project);
						m_historiesIO.updateHistoryItem(row);
						emit this->OperatorInstruction(OPERATOR_EXCHANGED_MAIN);
					}
					else
					{
						emit this->OperatorInstruction(OPERATOR_MESSAGE_NOFOUND);
					}
				}
			}
			break;
			case Qt::RightButton:
			{
				if (m_ctxMenu)
				{
					m_ctxMenu->move(QCursor::pos());
					m_ctxMenu->show();
				}
			}
			break;
			default:
				break;
			}
		}
		//----------------

		OptionsWidget::OptionsWidget(QWidget* parent) :
			QWidget(parent)
		{	
			this->setFixedWidth(192);
		}

		void OptionsWidget::initUI()
		{
			QVBoxLayout* vlayout = new QVBoxLayout(this);
			vlayout->setMargin(0);
			vlayout->setSpacing(0);
			this->setLayout(vlayout);

			QPushButton* create_btn = new QPushButton("新建", this);
			create_btn->setFixedHeight(48);
			QPushButton* load_btn = new QPushButton("加载其他", this);
			load_btn->setFixedHeight(48);

			/*QPushButton* template_btn = new QPushButton("其他模板", this);
			template_btn->setFixedHeight(48);*/


			vlayout->addWidget(create_btn);
			vlayout->addWidget(load_btn);
			/*vlayout->addWidget(template_btn);*/
			vlayout->addSpacerItem(new QSpacerItem(this->width(), 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

			connect(create_btn, &QPushButton::clicked, [this]() {emit this->buttonType(OPTION_CREATE); });
			connect(load_btn, &QPushButton::clicked, [this]() {emit this->buttonType(OPTION_LOAD); });
			//connect(template_btn, &QPushButton::clicked, [this]() {emit this->buttonType(OPTION_TEMPLATE); });

		}
		//---------------------
		WelcomeSection::WelcomeSection(QWidget* parent):
			QWidget(parent)
		{
		}
		void WelcomeSection::setupUi(HistoryListView* view, OptionsWidget* option)
		{
			QHBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);
			this->setLayout(layout);

			layout->addWidget(view);

			layout->addWidget(option);
		}

		//--------------------
		CreateSection::CreateSection(QWidget* parent):
			QWidget(parent),
			m_scene_name(0),
			m_path(0)
		{
		}
		void CreateSection::setupUi()
		{
			QVBoxLayout* layout = new QVBoxLayout(this);
			QLabel* sceneName = new QLabel("场景名字",this);
			QHBoxLayout* s_hlayout = new QHBoxLayout;
			m_scene_name = new QLineEdit(this);
			s_hlayout->addWidget(m_scene_name);
			s_hlayout->addSpacerItem(new QSpacerItem(192, 36, QSizePolicy::Expanding, QSizePolicy::Fixed));

			QLabel* path = new QLabel("选择rpgproject位置", this);
			QHBoxLayout* p_hlayout = new QHBoxLayout;
			p_hlayout->setSpacing(0);
			m_path = new QLineEdit(this);
			QPushButton* selected = new QPushButton("...", this);//
			selected->setFixedWidth(36);
			p_hlayout->addWidget(m_path);
			p_hlayout->addWidget(selected);
			p_hlayout->addSpacerItem(new QSpacerItem(192, 36, QSizePolicy::Expanding, QSizePolicy::Fixed));

			
			//button
			QPushButton* ok_btn = new QPushButton("确认", this);//
			QPushButton* back_btn = new QPushButton("返回", this);//
			QHBoxLayout* b_hlayout = new QHBoxLayout;
			b_hlayout->addSpacerItem(new QSpacerItem(586,48,QSizePolicy::Expanding,QSizePolicy::Fixed));
			b_hlayout->addWidget(ok_btn);
			b_hlayout->addWidget(back_btn);

			layout->addWidget(sceneName);
			layout->addLayout(s_hlayout);

			m_scene_warning = new QLabel(this);
			m_scene_warning->setStyleSheet("QLabel { color : red; }");
			m_scene_warning->setVisible(false);
			layout->addWidget(m_scene_warning);

			layout->addWidget(path);
			layout->addLayout(p_hlayout);

			m_path_warning = new QLabel(this);
			m_path_warning->setStyleSheet("QLabel { color : red; }");
			m_path_warning->setVisible(false);
			layout->addWidget(m_path_warning);

			//space
			layout->addSpacerItem(new QSpacerItem(0, 196, QSizePolicy::Minimum, QSizePolicy::Expanding));
			layout->addLayout(b_hlayout);

			connect(selected, &QPushButton::clicked, this, &CreateSection::slot_selectPath);
			connect(ok_btn, &QPushButton::clicked, this, &CreateSection::slot_checkLineEdit);
			connect(back_btn, &QPushButton::clicked, [this]() {emit this->backEnter(); });
		}

		void CreateSection::slot_selectPath()
		{
			QString filename = QFileDialog::getOpenFileName(nullptr, "选择.rpgproject", "./", "rpgmaker (*.rpgproject)");
			if (!filename.isEmpty())
			{
				filename.replace('\\', '/');
				int last = filename.lastIndexOf('/');
				this->m_path->setText(filename.left(last + 1));
			}
		}

		void CreateSection::slot_checkLineEdit()
		{
			m_scene_warning->setVisible(false);
			m_path_warning->setVisible(false);
			QString sceneName = m_scene_name->text();
			QString path = m_path->text();
			if (sceneName.isEmpty())
			{
				m_scene_warning->setText("名称不能为空");
				m_scene_warning->setVisible(true);
				return;
			}
			if (path.isEmpty())
			{
				m_path_warning->setText("路径不能为空");
				m_path_warning->setVisible(true);
				return;
			}

			if (!sceneName.contains(QRegExp("^[a-zA-Z][a-zA-Z0-9_]*$")))
			{
				m_scene_warning->setText("名称只能包含字母且开头，数字,_");
				m_scene_warning->setVisible(true);
				return;
			}
			if (!QFile::exists(path + "fonts/mplus-1m-regular.ttf"))
			{
				m_scene_warning->setText("不正确的项目路径,示例C:/xxx/xxx/");
				m_scene_warning->setVisible(true);
				return;
			}

			QDir dir(path);
			if (!dir.exists("UIData"))
			{
				dir.mkdir("UIData");
			}
			else
			{
				dir.cd("UIData");
				QString cmpname = sceneName + ".xml";
				for (QFileInfo& info : dir.entryInfoList({ "*.xml" }, QDir::Files))
				{
					if (info.fileName() == cmpname)
					{
						m_scene_warning->setText("已存在该创建");
						m_scene_warning->setVisible(true);
						return;
					}
				}
			}
			emit this->createEnter();
			
		}
		//---------------------
		LauncherWidget::LauncherWidget(QWidget* parent) :
			QStackedWidget(parent),
			m_dllnameLoaded(false),
			m_createSection(0),
			m_historyListView(0),
			m_optionsWidget(0),
			m_mainWindow(0)
		{
		}

		LauncherWidget::~LauncherWidget()
		{
			
		}

		void LauncherWidget::setupUi()
		{
			//欢迎页
			WelcomeSection* w_s = new WelcomeSection(this);
			m_historyListView = new HistoryListView(w_s);
			m_optionsWidget = new OptionsWidget(w_s);

			m_createSection = new CreateSection(this);

			w_s->setupUi(m_historyListView, m_optionsWidget);
			m_createSection->setupUi();

			this->addWidget(w_s);
			this->addWidget(m_createSection);
			
			connect(m_historyListView, &HistoryListView::OperatorInstruction, this, &LauncherWidget::slot_repsonseView);
			connect(m_optionsWidget, &OptionsWidget::buttonType, this, &LauncherWidget::slot_responseOption);
			connect(m_createSection, &CreateSection::createEnter, this, &LauncherWidget::slot_createScene);
			connect(m_createSection, &CreateSection::backEnter, this, &LauncherWidget::slot_backPage);
		}

		void LauncherWidget::initLoad()
		{
			if (m_historyListView)m_historyListView->loadHistory();
			if (m_optionsWidget)m_optionsWidget->initUI();
		}
		void LauncherWidget::startMainWindow()
		{
			if (m_mainWindow)
			{
				if (!m_dllnameLoaded)
				{
					manager::ObjectLoader::instance()->loadObjectsName();
					//m_mainWindow->loadConfig();
					m_mainWindow->initUI();

					m_mainWindow->adjustSize();
					m_dllnameLoaded = true;
				}
				
				//loadCurrentGraphics不会对历史记录做出任何操作
				if (m_mainWindow->loadCurrentGraphics())
				{
					m_mainWindow->show();
					manager::EditorLaunch::instance()->moveEdit(m_mainWindow->pos() + QPoint(m_mainWindow->width(),0));
					this->close();
					deleteLater();
				}
				else
				{
					m_historyListView->removeCallBack();
				}
			}
		}
		void LauncherWidget::loadMainWindow()
		{
			if (m_mainWindow)
			{
				if (!m_dllnameLoaded)
				{
					manager::ObjectLoader::instance()->loadObjectsName();
					//m_mainWindow->loadConfig();
					m_mainWindow->initUI();

					m_mainWindow->adjustSize();
					m_dllnameLoaded = true;
				}

				//这里loadSelectedGraphics会自动识别删除History
				if (m_mainWindow->loadSelectedGraphics())
				{
					m_mainWindow->show();
					manager::EditorLaunch::instance()->moveEdit(m_mainWindow->pos() + QPoint(m_mainWindow->width(), 0));
					this->close();
					deleteLater();
				}
			}
		}
		void LauncherWidget::slot_repsonseView(int type)
		{
			switch (type)
			{
			case OPERATOR_EXCHANGED_MAIN:
			{
				this->startMainWindow();
			}
			break;
			case OPERATOR_MESSAGE_NOFOUND:
			{
				if (QMessageBox::Yes == QMessageBox::information(0,"文件消失","文件缺失,是否移除该项",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
				{
					m_historyListView->removeCallBack();
				}
			}
			break;
			default:
				break;
			}
		}
		void LauncherWidget::slot_responseOption(int which)
		{
			if (which == OPTION_CREATE)
			{
				this->setCurrentIndex(CREATE_PAGE);
			}
			else if (which == OPTION_LOAD)
			{
				this->loadMainWindow();
			}
			else if (which == OPTION_TEMPLATE)
			{

			}
		}
		void LauncherWidget::slot_createScene()
		{
			QString path = m_createSection->path();
			QString sceneName = m_createSection->sceneName();
			m_historyListView->createNewHistory(path, sceneName);
			manager::ConfigInfo::instance()->loadWorkSpace(path);
			manager::ConfigInfo::instance()->setSceneName(sceneName);

			QFile file(path + "UIData/" + sceneName + ".xml");
			if (file.open(QIODevice::WriteOnly))
			{
				QTextStream stream(&file);
				stream << "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n";
				stream << QString("<%1></%1>").arg(sceneName);
				file.close();
				this->startMainWindow();
			}
		}
		void LauncherWidget::slot_backPage()
		{
			this->setCurrentIndex(WELCOME_PAGE);
		}
		void LauncherWidget::closeEvent(QCloseEvent* event)
		{
			m_historyListView->Closure();
		}
	}
}