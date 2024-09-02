#include "WindowEditorMainWidget.h"
#include<QMouseEvent>
#include<qdebug.h>
#include"../Manager/WinFontCalculate.h"
#include"../RpgObject/RpgWindowBase.h"
#include"MenuExtra.h"
#include"WindowElement/WinElementFactory.h"
#include"../Manager/ConfigureInfoContainer.h"
WindowEditorMainWidget::WindowEditorMainWidget(QWidget* parent):
	RpgObjectEditorBase(parent),
	m_active(0),
	m_weType(C_UNKNOW)
{
	connect(&m_editLauncher, &ElementEditLaunchWidget::itemChanged, this, &WindowEditorMainWidget::slot_updateWidget);
	this->initMenu();
}

void WindowEditorMainWidget::initMenu()
{
	QAction*edit_action= m_popContextMenu.addAction("Edit");
	connect(edit_action, &QAction::triggered, this, &WindowEditorMainWidget::slot_showEditWidget);

	m_wepopCtxMenu = new WinElementMainMenu(this);
	connect(m_wepopCtxMenu, &WinElementMainMenu::send_wetype, this, &WindowEditorMainWidget::slot_createElement);
}

WinElementBase* WindowEditorMainWidget::pickupElement(const QPoint& point)
{
	for (WinElementBase* element : m_window->elements())
	{
		if (element->contain(point))
		{
			return element;
		}
	}
	return 0;
}

void WindowEditorMainWidget::append_WinElement(WinElementBase* item)
{
	if (m_window)
	{
		m_window->elements().push_back(item);
		this->update();
	}
}

void WindowEditorMainWidget::slot_createElement(const QString&type)
{
	WinElementBase* item = WinElementFactory::create(type);
	m_window->elements().push_back(item);
	item->move(m_create_pos);
	this->update();
}

void WindowEditorMainWidget::slot_showEditWidget()
{
	m_editLauncher.LaunchEditWidget(m_active);
}

void WindowEditorMainWidget::paintEvent(QPaintEvent* pe)
{
	if (!this->item())return;
	QPixmap pixmap(this->width(),this->height());
	QPainter p(&pixmap);
	p.setFont(WinFontCalculate::instance()->winFont());
	for (WinElementBase* element : m_window->elements())
	{
		element->drawElement(p);
	}
	if (m_active)
	{
		p.setPen(QColor(255, 0, 0));
		m_active->adjust_rect();
		p.drawRect(m_active->rect());
	}
	p.end();

	QPainter p2(this);
	p2.drawPixmap(0,0, pixmap);
	p2.end();
}

void WindowEditorMainWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_active = this->pickupElement(event->pos());
		if(m_active)
		m_diff = event->pos()- m_active->position();
		this->update();
	}
	else if (event->button() == Qt::RightButton)
	{
		if (m_active && m_active->rect().contains(event->pos()))
		{
			m_popContextMenu.exec(mapToGlobal(event->pos()));
		}
		else
		{
			m_create_pos = event->pos();
			m_wepopCtxMenu->exec(mapToGlobal(m_create_pos));
		}
	}
}

void WindowEditorMainWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_active&&event->buttons()&Qt::LeftButton)
	{
		m_active->move(event->pos() - m_diff);
		m_editLauncher.updateDispPos();
		this->update();
	}
}

void WindowEditorMainWidget::closeEvent(QCloseEvent* event)
{
	m_editLauncher.closeAll();
}

void WindowEditorMainWidget::InitEditor(GameObject* item)
{
	m_window = (RpgWindow*)item;
	this->setFixedSize(item->canvas_width(), item->canvas_height());
}