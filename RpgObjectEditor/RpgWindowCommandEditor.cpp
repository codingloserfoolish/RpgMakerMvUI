#include"RpgWindowCommandEditor.h"
#include"../RpgObject/RpgCommandWindow.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<qpushbutton.h>
#include<qlabel.h>
#include<qspinbox.h>
CommandListDelegate::CommandListDelegate(QObject* parent) :QStyledItemDelegate(parent){}
void CommandListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.data(Qt::UserRole + 1).toBool())
	{
		painter->fillRect(option.rect, QColor(255, 0, 0));
	}
	QStyledItemDelegate::paint(painter, option,index);
}

CommandListMenu::CommandListMenu(QWidget* parent)
	:QMenu(parent)
{
	connect(this->addAction("active"), &QAction::triggered, [this]() {emit activeItem(); });
	connect(this->addAction("delete"), &QAction::triggered, [this]() {emit deleteItem(); });
}

CommandListWidget::CommandListWidget(QWidget* parent)
	:QListView(parent),
	m_active(0)
{
	m_model = new QStandardItemModel(this);
	this->setModel(m_model);
	this->setItemDelegate(new CommandListDelegate(this));

	m_ctxMenu = new CommandListMenu(this);
	connect(m_ctxMenu, &CommandListMenu::deleteItem, [this]() {this->removeCurrentCommand(); });
	connect(m_ctxMenu, &CommandListMenu::activeItem, this,&CommandListWidget::slot_activeItem);

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &CommandListWidget::customContextMenuRequested, this, &CommandListWidget::slot_popContextMenu);
}

void CommandListWidget::appendCommand(const QString& command)
{
	QStandardItem* item = new QStandardItem(command);
	item->setData(false,Qt::UserRole+1);
	m_model->appendRow(item);
}

void CommandListWidget::removeCurrentCommand()
{
	if (this->currentIndex().isValid())
	{
		if (m_active && m_active->row() == this->currentIndex().row())m_active = 0;
		m_model->removeRow(this->currentIndex().row());
	}
}

void CommandListWidget::fillRpgObjectItem(GameObject* item)
{
	RpgCommandWindow* temp = (RpgCommandWindow*)item;
	temp->clearCommandList();
	for (int i = 0; i < m_model->rowCount(); ++i)
	{
		temp->appendCommand(m_model->item(i)->text());
	}
	if(m_active)
	temp->setActiveIndex(m_active->row());
}

void CommandListWidget::slot_activeItem()
{
	QModelIndex index = this->currentIndex();
	if (index.isValid())
	{
		QStandardItem* item = m_model->itemFromIndex(index);
		if (m_active == 0)
		{
			m_active = item;
		}
		else
		{
			if (m_active == item)return;
			else
			{
				m_active->setData(false, Qt::UserRole + 1);
				m_active = item;
			}
		}
		m_active->setData(true, Qt::UserRole + 1);
	}
}

void CommandListWidget::slot_popContextMenu(const QPoint& pos)
{
	m_ctxMenu->exec(mapToGlobal(pos));
}

RpgWindowCommandEditor::RpgWindowCommandEditor(QWidget* parent)
	:RpgObjectEditorBase(parent)
{
	QVBoxLayout* main_layer = new QVBoxLayout(this);
	
	QHBoxLayout* btn_layer = new QHBoxLayout;
	main_layer->addLayout(btn_layer);
	QPushButton* add_btn = new QPushButton("append", this);
	QPushButton* rem_btn = new QPushButton("remove", this);
	btn_layer->addWidget(add_btn);
	btn_layer->addWidget(rem_btn);

	QHBoxLayout* display_countLayer = new QHBoxLayout;
	main_layer->addLayout(display_countLayer);
	m_displayCount = new QSpinBox(this);
	display_countLayer->addWidget(new QLabel("displayCount", this));
	display_countLayer->addWidget(m_displayCount);

	m_listWidget = new CommandListWidget(this);
	main_layer->addWidget(m_listWidget);

	QPushButton* apply_btn = new QPushButton("apply", this);
	main_layer->addWidget(apply_btn);

	connect(add_btn, &QPushButton::clicked, this, &RpgWindowCommandEditor::slot_appendItem);
	connect(rem_btn, &QPushButton::clicked, this, &RpgWindowCommandEditor::slot_removeItem);
	connect(apply_btn, &QPushButton::clicked, this,&RpgWindowCommandEditor::slot_refreshGraphics);
	connect(m_displayCount, &QSpinBox::textChanged, this, &RpgWindowCommandEditor::slot_displayCount);
}

void RpgWindowCommandEditor::slot_appendItem()
{
	m_listWidget->appendCommand("item");
}

void RpgWindowCommandEditor::slot_removeItem()
{
	m_listWidget->removeCurrentCommand();
}

void RpgWindowCommandEditor::slot_displayCount()
{
	RpgCommandWindow* temp = (RpgCommandWindow*)(this->item());
	temp->setdisplayCount(m_displayCount->value());
	emit itemChanged();
}

void RpgWindowCommandEditor::slot_refreshGraphics()
{
	m_listWidget->fillRpgObjectItem(this->item());
	emit itemChanged();
}

void RpgWindowCommandEditor::InitEditor(GameObject* item)
{
	RpgCommandWindow* temp = (RpgCommandWindow*)item;
	m_listWidget->clearCommand();
	for (QString& command : temp->commandList())
	{
		m_listWidget->appendCommand(command);
	}
	m_listWidget->setActiveIndex(temp->activeIndex());
	m_displayCount->setValue(temp->displayCout());
}