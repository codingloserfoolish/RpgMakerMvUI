#ifndef RPGWINDOWCOMMANDEDITOR_H_
#define RPGWINDOWCOMMANDEDITOR_H_
#include<qlistview.h>
#include<qstandarditemmodel.h>
#include<qstyleditemdelegate.h>
#include<qmenu.h>
#include"RpgObjectEditorBase.h"
class QSpinBox;
class RpgCommandWindow;
class CommandListDelegate :public QStyledItemDelegate
{
	Q_OBJECT
public:
	CommandListDelegate(QObject* parent = nullptr);
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

};

class CommandListMenu :public QMenu
{
	Q_OBJECT
public:
	CommandListMenu(QWidget* parent = nullptr);
signals:
	void deleteItem();
	void activeItem();
};

class CommandListWidget :public QListView
{
	Q_OBJECT
public:
	CommandListWidget(QWidget* parent = nullptr);
	void appendCommand(const QString& command);
	void removeCurrentCommand();
	void clearCommand() { m_model->clear(); }
	void fillRpgObjectItem(GameObject*item);
	void setActiveIndex(int row) { m_active = m_model->item(row);if(m_active) m_active->setData(true, Qt::UserRole + 1); }
	//int getActiveRow() { if (m_active)return m_active->row(); return 0; }
public slots:
	void slot_activeItem();
	void slot_popContextMenu(const QPoint& pos);
private:
	QStandardItemModel* m_model;
	QStandardItem* m_active;
	CommandListMenu* m_ctxMenu;
};

class RpgWindowCommandEditor :public RpgObjectEditorBase
{
	Q_OBJECT
public:
	RpgWindowCommandEditor(QWidget* parent = nullptr);
public slots:
	void slot_appendItem();
	void slot_removeItem();
	void slot_displayCount();
	void slot_refreshGraphics();
protected:
	virtual void InitEditor(GameObject* item)override;
private:
	CommandListWidget* m_listWidget;
	QSpinBox* m_displayCount;
};

#endif // !RPGWINDOWCOMMANDEDITOR_H_
