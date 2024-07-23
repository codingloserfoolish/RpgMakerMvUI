#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include<qmainwindow.h>
#include<qsplitter.h>
#include"../QtPropertyBrowser/qttreepropertybrowser.h"


#include"../Manager/PixmapManager.h"
#include"../Manager/PartPropertyManager.h"
#include"GraphicsWindow.h"
#include"ObjectsTreeWidget.h"

class MainWindow :public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent=nullptr);
	~MainWindow();
	void initGraphicsWidget();
	void initPropertyWidget();
	void initObjectsTreeWidget();
	void initMenuBar();
public slots:
	void slot_gameObjectBehivor(int behivor, QObject* obj);
private:
	GraphicsWindow* m_graphicsWidget;
	QtVariantPropertyManager* m_propertyManager;
	QtTreePropertyBrowser* m_propertyWidget;
	ObjectsTreeWidget* m_objectsTreeWidget;
};
#endif // !MAINWINDOW_H_