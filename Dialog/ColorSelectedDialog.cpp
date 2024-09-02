#include"ColorSelectedDialog.h"
#include<qpainter.h>
#include<qpushbutton.h>
#include<QMouseEvent>
#include<QVBoxLayout>
FontColorWidget::FontColorWidget(QWidget* parent)
	:QWidget(parent),
	m_selectedColor(0)
{
	this->setFixedSize(384, 192);
	m_colorPart = PixmapManager::instance()->loadPixmap("system/Window");
}

void FontColorWidget::paintEvent(QPaintEvent* e)
{
	if (m_colorPart == 0)return;
	QPainter p(this);
	p.drawPixmap(0, 0, 384, 192,m_colorPart->_pixmap, 96, 144, 96, 48);
	p.setPen(QPen(QColor(127, 127, 127), 2));
	p.drawRect(48 * (m_selectedColor % 8), 48 * (m_selectedColor / 8), 46, 46);
	p.end();
}

void FontColorWidget::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		m_selectedColor = (e->pos().x() / 48) + 8 * (e->pos().y() / 48);
		this->update();
	}
}

ColorSelectedDialog::ColorSelectedDialog(QWidget* parent)
	:QDialog(parent)
{
	this->setWindowTitle("select font color");
	QVBoxLayout* main_layer = new QVBoxLayout(this);
	m_fontColorGraphics = new FontColorWidget(this);
	QPushButton* ensure_btn = new QPushButton("ensure", this);
	main_layer->addWidget(m_fontColorGraphics);
	main_layer->addWidget(ensure_btn);

	connect(ensure_btn, &QPushButton::clicked, [this]() {this->accept(); });
}

int ColorSelectedDialog::GetColorIndex()
{
	ColorSelectedDialog w;
	if (QDialog::Accepted == w.exec())
	{
		return w.colorIndex();
	}
	return -1;
}


