#include "JsImageReserveContainer.h"
JsImageReserveContainer*JsImageReserveContainer::m_instance = 0;
SplitImgFloder::SplitImgFloder(QString& str)
{
	_back = 0;
	_next = 0;
	_exist_ref = true;
	QStringList list = str.split('/');
	this->_floder = "\'img/" + list[0] + "/\'";
	this->_filename = '\'' + list[1]+'\'';
}

SplitFloderList::~SplitFloderList()
{
	if (!m_head)return;
	SplitImgFloder* temp = m_head->_next;
	while (temp)
	{
		delete temp->_back;
		temp = temp->_next;
	}
	delete m_last;
}

SplitImgFloder* SplitFloderList::addFloder(QString& str)
{
	if (!m_head)
	{
		m_head = new SplitImgFloder(str);
		m_last = m_head;
		return m_head;
	}
	else
	{
		SplitImgFloder* temp = m_head;
		while (temp)
		{
			if (temp->_orginal_Str == str)
			{
				temp->_exist_ref = true;
				return temp;
			}
			temp = temp->_next;
		}
		temp = new SplitImgFloder(str);
		m_last->_next = temp;
		temp->_back = m_last;
		m_last = temp;
		return temp;
	}
}

void SplitFloderList::declineAll()
{
	SplitImgFloder* temp = m_head;
	while (temp)
	{
		temp->_exist_ref = false;
		temp = temp->_next;
	}
}

void SplitFloderList::clearFalse()
{
	SplitImgFloder* temp = m_head;
	SplitImgFloder* current = 0;
	while (temp)
	{
		if (!temp->_exist_ref)
		{
			if (temp->_back != 0)
			{
				temp->_back->_next = temp->_next;
			}
			if (temp->_next != 0)
			{
				temp->_next->_back = temp->_back;
			}
			current = temp;
			temp = temp->_next;
			delete current;
			continue;
		}
		temp = temp->_next;
	}
}

void JsImageReserveContainer::JsPreReserveBitmapWriter(QTextStream& stream)
{
	QString imgReserve = QString("ImageManager.reserveBitmap(%1,%2);\n");
	SplitImgFloder* item =0;
	m_floder_list.reset();
	while (0!=(item=m_floder_list.current()))
	{
		stream << imgReserve.arg(item->_floder).arg(item->_filename);
		m_floder_list.move_next();
	}
}
