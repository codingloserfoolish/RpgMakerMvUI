#include"PixmapManager.h"
#include"PathManager.h"

PixmapManager*PixmapManager::m_instance = 0;

PixmapManager::PixmapManager() 
	:m_winskin(PathManager::instance()->image_path() + "system/Window.png"), 
	m_tempIVPixmap("./icon/IconVariable.png"),
	m_head(0),
	m_last(0),
	m_size(0),
	m_empty{"",QPixmap(100,100),0}
{ m_empty._pixmap.fill(Qt::black); }
PixmapManager::~PixmapManager()
{
	if(m_head)
	delete m_head;
}
PixmapManager*PixmapManager::instance()
{
	if (!m_instance)
	{
		m_instance = new PixmapManager;
	}
	return m_instance;
}
void PixmapManager::destroy()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = 0;
	}
}
Pixmap* PixmapManager::loadPixmap(const QString& name)
{
	QString img_path = PathManager::instance()->image_path();
	if (name.isEmpty()||name==" ")return 0;
	Pixmap* temp = m_head;
	Pixmap* before_before = 0;
	Pixmap* before = 0;
	while (temp)
	{
		if (temp->_name == name)
		{
			if (before != 0)
			{
				if (temp == m_last)
				{
					m_last = before;
					m_last->_next = 0;
				}
				else
				{
					before->_next = temp->_next;
				}
				temp->_next = m_head;
				m_head = temp;
			}
			return temp;
		}
		before_before = before;
		before = temp;
		temp = temp->_next;
	}
	if (m_size >= MAX_SIZE)
	{
		m_last->_name = name;
		m_last->_pixmap.load(img_path + name+".png");
		if (before_before != 0)
		{
			before_before->_next = 0;
			m_last->_next = m_head;
			m_head = m_last;
			m_last = before_before;
		}
		temp = m_head;
	}
	else
	{
		temp = new Pixmap;
		temp->_name = name;
		temp->_pixmap.load(img_path + name+ ".png");
		temp->_next = m_head;
		m_head = temp;
		if (m_last == 0)m_last = m_head;
		++m_size;
	}
	return temp;
}