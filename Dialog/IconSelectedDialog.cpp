#include"IconSelectedDialog.h"
#include<QVBoxLayout>
#include<qscrollarea.h>
#include<qpushbutton.h>
#include<qpainter.h>
#include<QMouseEvent>

int IconSelectedDialog::_send_Id = 0;
IconPicture::IconPicture(QWidget* parent) :
	QWidget(parent),
	m_systemIcon(0),
	m_seletedArea(2, 2, 30, 30)
{
	this->setSystemIcon("system/IconSet");
}

void IconPicture::setSystemIcon(const QString& iconSystemName)
{
	m_systemIcon = PixmapManager::instance()->loadPixmap(iconSystemName);
	this->resize(m_systemIcon->width(), m_systemIcon->height());
}

void IconPicture::adjustRectByPoint(QPoint& pos)
{
	m_seletedArea.moveTo(32 * (int)(pos.x() / 32) + 2, 32 * (int)(pos.y() / 32) + 2);
}

void IconPicture::paintEvent(QPaintEvent*)
{
	if (!m_systemIcon)return;
	QPainter p(this);
	p.drawPixmap(0, 0, m_systemIcon->_pixmap);
	p.setPen(QPen(QColor(255, 0, 0), 4));
	p.drawRect(m_seletedArea);
	p.end();
}

void IconPicture::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		this->adjustRectByPoint(e->pos());
		this->update();
		int count_x = m_seletedArea.x() / 32;
		int count_y = m_seletedArea.y() / 32;
		emit selectedChanged(count_x + count_y * m_systemIcon->width() / 32);
	}
}

IconSelectedDialog::IconSelectedDialog(QWidget* parent) :QDialog(parent)
{
	this->setWindowTitle("select icon");
	QVBoxLayout* main_layer = new QVBoxLayout(this);

	QScrollArea* area = new QScrollArea(this);
	main_layer->addWidget(area);

	IconPicture* pixwidget = new IconPicture(this);
	area->setWidget(pixwidget);
	area->setAlignment(Qt::AlignCenter);

	m_iconIdLabel = new QLabel("id:", this);
	main_layer->addWidget(m_iconIdLabel);

	QPushButton* button = new QPushButton("ensure", this);
	main_layer->addWidget(button);

	connect(pixwidget, &IconPicture::selectedChanged, this, &IconSelectedDialog::slot_indexChanged);
	connect(button, &QPushButton::clicked, this, &IconSelectedDialog::slot_setExecMsg);

}

int IconSelectedDialog::GetIconId()
{
	IconSelectedDialog w;
	if (QDialog::Accepted == w.exec())
	{
		return IconSelectedDialog::_send_Id;
	}
	return 0;
}

void IconSelectedDialog::slot_setExecMsg()
{
	if (IconSelectedDialog::_send_Id != 0)
	{
		this->accept();
	}
	else
	{
		this->reject();
	}
}

void IconSelectedDialog::slot_indexChanged(int icon_index)
{
	m_iconIdLabel->setText(QString("id:%1").arg(icon_index));
	_send_Id = icon_index;
}