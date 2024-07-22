#include"ObjectsTreeWidget.h"
#include<QMouseEvent>
ObjectsTreeWidget::ObjectsTreeWidget(const QStringList& header,QWidget* parent):
	QTreeView(parent)
{
	m_model = new QStandardItemModel(this);
	m_model->setHorizontalHeaderLabels(header);
	this->setModel(m_model);
	this->initMenu();

	connect(this, &QTreeView::pressed, this, &ObjectsTreeWidget::slot_responseClickEvent);
}

void ObjectsTreeWidget::initMenu()
{
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	m_pCtMenu = new ContextPopMenu(this);
	connect(this, &QTreeView::customContextMenuRequested, this, &ObjectsTreeWidget::slot_PopMenu);
	connect(m_pCtMenu, &ContextPopMenu::trigger_event, this, &ObjectsTreeWidget::slot_reciveMenuEvent);
}

void ObjectsTreeWidget::initRoot(GameObject* root)
{
	m_model->setItem(0,root->standard_item());
}

QStandardItem* ObjectsTreeWidget::findItemByText(QString&text)
{
	QStandardItem* start = m_model->item(0);
	if (!start||text.isEmpty())return 0;
	if (start->text() == text)return start;
	this->findItemByText_imp(text, start);
}

void ObjectsTreeWidget::slot_PopMenu(const QPoint& pos)
{
	m_pCtMenu->exec(this->mapToGlobal(pos));
}

void ObjectsTreeWidget::slot_reciveMenuEvent(int behavior, int param)
{
	if (behavior == CONTEXT_BEHAVIOR_CREATE) { emit requireCreate(param); }
	else if (behavior == CONTEXT_BEHAVIOR_DELETE) { emit requireDestroy(); }
}

void ObjectsTreeWidget::slot_responseClickEvent(const QModelIndex& item)
{
	QStandardItem* s_item = m_model->itemFromIndex(item);
	GameObject* gameObject = qvariant_cast<GameObject_Data>(s_item->data(GAMEOBJECT_ROLE)).gameObject();
	emit clickEvent(gameObject);
}

//void ObjectsTreeWidget::slot_responseCreate(GameObject* item)
//{
//	QModelIndex selected = this->currentIndex();
//	if (selected.isValid()&&item)
//	{
//		QStandardItem* parent = m_model->itemFromIndex(selected);
//		parent->setChild(parent->rowCount(), item->standard_item());
//	}
//}


void ObjectsTreeWidget::slot_responseDestroy(GameObject* item)
{
	if (!item)return;
	QModelIndex destroyed_item = m_model->indexFromItem(item->standard_item());
	m_model->removeRow(destroyed_item.row(), destroyed_item.parent());
}

void ObjectsTreeWidget::slot_responseSelected(GameObject* item)
{
	this->setCurrentIndex(m_model->indexFromItem(item->standard_item()));
}

QStandardItem* ObjectsTreeWidget::findItemByText_imp(QString& text, QStandardItem* item)
{
	if (!item)return 0;
	QStandardItem* result=0;
	for (int i = 0; i < item->rowCount(); ++i)
	{
		result = item->child(i);
		if (result->text() == text)return result;
		result = findItemByText_imp(text, result);
		if (result)return result;
	}
	return 0;
}
