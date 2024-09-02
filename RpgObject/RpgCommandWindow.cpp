#include"RpgCommandWindow.h"
#include"../Manager/ConfigureInfoContainer.h"
#include"../RpgObjectEditor/RpgWindowCommandEditor.h"
RpgCommandWindow::RpgCommandWindow(QObject* parent)
	:GameObject(240,72,parent),
	m_item_height(36),
	m_displayCount(0),
	m_active_index(0)
{
	m_system_window = PixmapManager::instance()->loadPixmap("system/Window");
}

void RpgCommandWindow::setdisplayCount(int count)
{
	if (count == 0)return; 
	m_canvas_height = count * m_item_height+ConfigureInfoContainer::instance()->window_configure()._standardPadding * 2;
	m_displayCount = count; 
}

void RpgCommandWindow::appendCommand(const QString& command)
{
	for (QString& text : m_command_list)
	{
		if (text == command)return;
	}
	m_command_list.push_back(command);
}

void RpgCommandWindow::removeCommand(int index)
{
	if (m_command_list.size() <= index)return;
	m_command_list.removeAt(index);
}

RpgObjectEditorBase* RpgCommandWindow::createEditor()
{
	return new RpgWindowCommandEditor;
}

void RpgCommandWindow::draw_self(QPainter& p)
{
	int start_pos = m_active_index-m_displayCount+1;
	int pos_x = -m_canvas_width / 2;
	int pos_y = -m_canvas_height / 2;
	p.drawPixmap(pos_x, pos_y, 24, 24, m_system_window->_pixmap, 96, 0, 24, 24);
	p.drawPixmap(24 + pos_x, pos_y, canvas_width() - 48, 24, m_system_window->_pixmap, 120, 0, 48, 24);
	p.drawPixmap(canvas_width() / 2 - 24, pos_y, 24, 24, m_system_window->_pixmap, 168, 0, 24, 24);

	p.drawPixmap(canvas_width() / 2 - 24, 24 + pos_y, 24, canvas_height() - 48, m_system_window->_pixmap, 168, 24, 24, 48);
	p.drawPixmap(canvas_width() / 2 - 24, canvas_height() / 2 - 24, 24, 24, m_system_window->_pixmap, 168, 72, 24, 24);

	p.drawPixmap(24 + pos_x, canvas_height() / 2 - 24, canvas_width() - 48, 24, m_system_window->_pixmap, 120, 72, 48, 24);
	p.drawPixmap(pos_x, canvas_height() / 2 - 24, 24, 24, m_system_window->_pixmap, 96, 72, 24, 24);
	p.drawPixmap(pos_x, 24 + pos_y, 24, canvas_height() - 48, m_system_window->_pixmap, 96, 24, 24, 48);

	if (start_pos < 0)start_pos = 0;

	if (!m_command_list.isEmpty())
	{
		p.fillRect(-m_canvas_width / 2 + ConfigureInfoContainer::instance()->window_configure()._textPadding,
			-m_canvas_height / 2 + ConfigureInfoContainer::instance()->window_configure()._standardPadding + (m_active_index- start_pos) * m_item_height,
			m_canvas_width - 2 * ConfigureInfoContainer::instance()->window_configure()._textPadding,
			m_item_height, QColor(91, 132, 118));
		int size = start_pos + m_displayCount;
		if (m_command_list.size() < start_pos + m_displayCount)
		{
			size = m_command_list.size();
		}
		for (int i = start_pos; i < size; ++i)
		{
			this->drawLineItem(p, m_command_list[i], i - start_pos);
		}
	}
}

QString RpgCommandWindow::Js_NewObject()
{
	return QString();
}

QString RpgCommandWindow::Js_AttributeSet()
{
	return QString();
}

QDomElement RpgCommandWindow::Xml_SaveData(QDomDocument& doc, QDomElement& parent_node)
{
	return QDomElement();
}

QDomNode RpgCommandWindow::Xml_LoadData(QDomNode& self)
{
	return QDomNode();
}

void RpgCommandWindow::drawLineItem(QPainter& p,QString& text, int index)
{
	QFontMetrics metric = p.fontMetrics();
	int x = -m_canvas_width / 2 + ConfigureInfoContainer::instance()->window_configure()._textPadding;
	int y = -m_canvas_height / 2 + metric.ascent()+ ConfigureInfoContainer::instance()->window_configure()._standardPadding + index * m_item_height;
	p.drawText(x, y, text);
}
