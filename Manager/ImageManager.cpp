#include"ImageManager.h"

namespace hjdd
{
	namespace manager
	{
		ImageManager::~ImageManager()
		{
			Pixmap* temp = m_head;
			while (temp->_next)
			{
				temp = temp->_next;
				temp->_back->_next = 0;
				delete temp->_back;
				temp->_back = 0;
			}
			delete temp;
		}
		Pixmap* ImageManager::loadImage(const QString& filename)
		{
			if (m_head == 0)
			{
				m_head = new Pixmap(filename);
				m_last = m_head;
				++m_size;
				++(m_head->_count);
				return m_head;
			}

			//如果包含该图片就增加引用次数
			Pixmap* temp = this->contain(filename);
			if (temp != 0)
			{
				++(temp->_count);
				move_head(temp);
				return temp;
			}
			QString img_filename = ConfigInfo::instance()->imgPath() + filename;
			//如果大于MAX_PIXMAP_COUNT就替换引用次数为0的图片
			temp = m_last;
			if (m_size >= MAX_PIXMAP_COUNT)
			{
				while (temp&&temp->_count == 0){temp = temp->_back;}
				if (!temp)
				{
					temp = m_head;
					temp->_pixmap.load(img_filename);
					temp->_filename = filename;
					++(temp->_count);
					return temp;
				}
				else if (temp->_next != 0)
				{
					temp = temp->_next;
					temp->_pixmap.load(img_filename);
					++(temp->_count);
					temp->_filename = filename;
					return temp;
				}
			}
			return this->addNode(filename);
		}
		void ImageManager::releseImage(Pixmap* node)
		{
			--(node->_count);
			if (node->_count == 0)
			{
				move_last(node);
				if (m_size >= MAX_EXPLODE_COUNT)
				{
					m_last = m_last->_back;
					delete m_last->_next;
					m_last->_next = 0;
					--m_size;
				}
			}
		}
		Pixmap* ImageManager::contain(const QString& filename)
		{
			Pixmap* temp = m_head;
			while (temp && temp->_filename != filename)temp = temp->_next;
			return temp;
		}
		void ImageManager::move_head(Pixmap* item)
		{
			if (item == m_head)return;
			if (item->_back)
			{
				item->_back->_next = item->_next;
			}
			if (item->_next)
			{
				item->_next->_back = item->_back;
			}
			m_head->_back = item;
			item->_next = m_head;
			item->_back = 0;
			m_head = item;
		}
		void ImageManager::move_last(Pixmap* item)
		{
			if (item == m_last)return;
			if (item->_back)
			{
				item->_back->_next = item->_next;
			}
			if (item->_next)
			{
				item->_next->_back = item->_back;
			}
			item->_next = 0;
			item->_back = m_last;
			m_last->_next = item;
			m_last = item;
		}
		Pixmap* ImageManager::addNode(const QString& filename)
		{
			Pixmap* node = new Pixmap(filename);
			node->_back = m_last;
			m_last->_next = node;
			++(node->_count);
			m_last = node;
			++m_size;
			return node;
		}
		ImageManager::ImageManager()
			:m_head(0),
			m_last(0),
			m_size(0),
			ivIcon("./icon/IconVariable.png")
		{
		}
	}
}