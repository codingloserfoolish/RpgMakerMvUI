#include "ElementEditLaunchWidget.h"
#include "ElementEditLaunchWidget.h"
#include"ElementEditLaunchWidget.h"
#include"LinesTextEditWidget/LinesTextEditWidget.h"
#include"BitmapEditWidget/BitmapEditWidget.h"

ElementEditLaunchWidget::ElementEditLaunchWidget(QObject* parent)
	:QObject(parent),
	m_current(0),
	m_weType(C_UNKNOW)
{
}

ElementEditLaunchWidget::~ElementEditLaunchWidget()
{
	if (m_current) 
	{
		delete m_current; 
	}
}

void ElementEditLaunchWidget::LaunchEditWidget(WinElementBase* item)
{
	if (!item)return;
	if (item->we_type() == m_weType)
	{
		this->updateWidget(item);
	}
	else
	{
		m_weType = (WinElementType)item->we_type();
		if (m_current)
		{
			m_current->close();
			disconnect(m_current, 0, this, 0);
			delete m_current;
			m_current = 0;
		}
		m_current = item->createEditor();
		connect(m_current, &ElementWidgetBase::itemChanged, this, &ElementEditLaunchWidget::slot_itemChanged);
		m_current->show();
	}
}

void ElementEditLaunchWidget::updateWidget(WinElementBase* item)
{
	if (!m_current->isVisible())m_current->show();
	m_current->setValue(item);
}

void ElementEditLaunchWidget::updateDispPos()
{
	if (m_current&&m_current->isVisible())
	{
		m_current->updateDispValue();
	}
}

void ElementEditLaunchWidget::closeAll()
{
	if (m_current && m_current->isVisible())
	{
		m_current->close();
	}
}
