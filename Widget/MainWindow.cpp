#include"MainWindow.h"
#include"MenuExtend.h"
#include<QHBoxLayout>
#include<qdockwidget.h>
#include<qmenubar.h>

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
	this->setWindowIcon(QIcon("./icon/icon.png"));
	this->initGraphicsWidget();
	this->initPropertyWidget();
	this->initObjectsTreeWidget();
	this->initMenuBar();

	connect(this, &MainWindow::reload, m_graphicsWidget, &GraphicsWindow::slot_Load);

	connect(m_graphicsWidget, &GraphicsWindow::gameObjectBehavior, this, &MainWindow::slot_gameObjectBehivor);
	connect(m_propertyManager, &QtVariantPropertyManager::valueChanged, m_graphicsWidget, &GraphicsWindow::slot_gameObjectPropertyChanged);

	connect(m_objectsTreeWidget, &ObjectsTreeWidget::requireCreate, m_graphicsWidget, &GraphicsWindow::slot_generateObject);
	connect(m_objectsTreeWidget, &ObjectsTreeWidget::requireDestroy, m_graphicsWidget, std::bind(&GraphicsWindow::slot_responseTriggerPopMenuEvent, m_graphicsWidget, CONTEXT_BEHAVIOR_DELETE, 0));
	connect(m_objectsTreeWidget, &ObjectsTreeWidget::clickEvent, m_graphicsWidget, &GraphicsWindow::slot_gameObjectChanged);
}

MainWindow::~MainWindow()
{
	PixmapManager::destroy();
	PartPropertyManager::destroy();
}

void MainWindow::initGraphicsWidget()
{
	m_graphicsWidget = new GraphicsWindow(this);
	this->setCentralWidget(m_graphicsWidget);
}

void MainWindow::initPropertyWidget()
{
	QDockWidget* propertyDock = new QDockWidget(this);

	m_propertyManager = new QtVariantPropertyManager(propertyDock);
	m_propertyWidget = new QtTreePropertyBrowser(propertyDock);
	m_propertyWidget->setFactoryForManager(m_propertyManager, new QtVariantEditorFactory(propertyDock));
	PartPropertyManager::instance()->setManager(m_propertyManager);

	propertyDock->setWidget(m_propertyWidget);
	this->addDockWidget(Qt::RightDockWidgetArea, propertyDock);
}

void MainWindow::initObjectsTreeWidget()
{
	QDockWidget* treeDock = new QDockWidget(this);

	m_objectsTreeWidget = new ObjectsTreeWidget({ "name" }, treeDock);
	m_objectsTreeWidget->initRoot(m_graphicsWidget->root());

	treeDock->setWidget(m_objectsTreeWidget);
	this->addDockWidget(Qt::LeftDockWidgetArea, treeDock);

}

void MainWindow::initMenuBar()
{
	FileMenu* file_menu = new FileMenu(this);
	this->menuBar()->addMenu(file_menu);
	connect(file_menu, &FileMenu::save_trigger, m_graphicsWidget, &GraphicsWindow::slot_Save);
	connect(file_menu, &FileMenu::load_trigger, m_graphicsWidget, &GraphicsWindow::slot_Load);

	RpgObjectMenu* rpgobject_menu = new RpgObjectMenu(this);
	this->menuBar()->addMenu(rpgobject_menu);
	connect(rpgobject_menu, &RpgObjectMenu::create_object, m_graphicsWidget, &GraphicsWindow::slot_generateObject);
}

void MainWindow::slot_gameObjectBehivor(int behivor, QObject* obj)
{
	disconnect(m_propertyManager,0,m_graphicsWidget,0);
	m_propertyWidget->clear();
	if (behivor == 0) 
	{
		return;
	}
	m_propertyWidget->addProperty(PartPropertyManager::instance()->property_data(obj));
	connect(m_propertyManager, &QtVariantPropertyManager::valueChanged, m_graphicsWidget, &GraphicsWindow::slot_gameObjectPropertyChanged);
}