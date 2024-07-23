#ifndef OBJECTSTREEWIDGET_H_
#define OBJECTSTREEWIDGET_H_
#include<qtreeview.h>
#include"MenuExtend.h"
#include"../RpgObject/GameObject.h"

class ObjectsTreeWidget :public QTreeView
{
	Q_OBJECT
public:
	ObjectsTreeWidget(const QStringList&header,QWidget* parent = nullptr);
	void initMenu();
	void initRoot(GameObject*root);
	QStandardItem* findItemByText(QString&text);
public slots:
	void slot_PopMenu(const QPoint& pos);
	void slot_reciveMenuEvent(int behavior, int param);
	//void slot_responseCreate(GameObject*item);

	void slot_responseClickEvent(const QModelIndex& item);
	void slot_responseDestroy(GameObject* item);
	void slot_responseSelected(GameObject* item);
signals:
	void clickEvent(GameObject* item);
	void requireCreate(int type);
	void requireDestroy();
private:
	QStandardItem* findItemByText_imp(QString& text, QStandardItem* item);

	QStandardItemModel* m_model;
	ContextPopMenu* m_pCtMenu;
};

#endif // !OBJECTSTREEWIDGET_H_
