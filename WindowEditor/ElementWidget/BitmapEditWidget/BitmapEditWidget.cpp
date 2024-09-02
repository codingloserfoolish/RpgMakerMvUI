#include"BitmapEditWidget.h"
#include<qdebug.h>
#include<QMouseEvent>
#include<qpainter.h>
#include<qlabel.h>
#include<qpushbutton.h>
#include<qscrollarea.h>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QGridLayout>
#include<qfiledialog.h>
#include"../../../Manager/PathManager.h"
#include"../SpinBox/SpinBox.h"
#include"../../WindowElement/wbBitmap.h"
BitmapEditWidget::BitmapEditWidget(QWidget* parent):QWidget(parent),m_bitmap(0)
{
	m_frame_color.setColor(QColor(192, 192, 192));
	m_frame_color.setWidth(2);
}

void BitmapEditWidget::setPixmap(wbBitmap* bitmap)
{
	if (!bitmap)return; 
	m_bitmap = bitmap; 
	this->resize(bitmap->rect().size());
}

QRect BitmapEditWidget::adjustFrameRect(int dx,int dy)
{
	return QRect(m_bitmap->frame().x()+dx,m_bitmap->frame().y()+dy,m_bitmap->frame().width(),m_bitmap->frame().height());
}

void BitmapEditWidget::paintEvent(QPaintEvent*)
{
	if (!m_bitmap||!m_bitmap->bitmap())return;
	QPixmap canvas(this->width(),this->height());
	QPainter p(&canvas);
	p.drawPixmap(0,0, m_bitmap->bitmap()->_pixmap);
	p.fillRect(0, 0, canvas.width(), canvas.height(), QColor(192, 192, 192, 127));
	p.setPen(m_frame_color);
	p.drawPixmap(m_bitmap->frame(),m_bitmap->bitmap()->_pixmap, m_bitmap->frame());
	p.drawRect(m_bitmap->frame());
	p.end();

	QPainter p2(this);
	p2.drawPixmap(0, 0, canvas);
	p2.end();

}

void BitmapEditWidget::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (m_bitmap && m_bitmap->frame().contains(e->pos()))
		{
			m_diff = e->pos() - m_bitmap->frame().topLeft();
		}
	}
}

void BitmapEditWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (m_bitmap)
	{
		if (e->buttons() & Qt::LeftButton&&m_bitmap->frame().contains(e->pos()))
		{
			m_bitmap->frame().moveTo(e->pos() - m_diff);
			emit frameDispChanged();
			this->update();
		}
	}
}

BitmapEditMainWidget::BitmapEditMainWidget(QWidget* parent):ElementWidgetBase(parent)
{
	QVBoxLayout* main_layer = (QVBoxLayout*)this->layout();

	QHBoxLayout* disp_path_layer = new QHBoxLayout;
	main_layer->addLayout(disp_path_layer);
	QLabel* path_lab = new QLabel("picture", this);
	m_select_path = new QLineEdit(this);
	m_select_path->setEnabled(false);
	QPushButton* search_btn = new QPushButton("...", this);
	disp_path_layer->addWidget(path_lab);
	disp_path_layer->addWidget(m_select_path);
	disp_path_layer->addWidget(search_btn);

	QGridLayout* frame_layer = new QGridLayout;
	main_layer->addLayout(frame_layer);
	QLabel* rect_w_lab = new QLabel("width", this);
	m_rect_w = new SpinBox(this);
	QLabel* rect_h_lab = new QLabel("height", this);
	m_rect_h = new SpinBox(this);

	QLabel* frame_x_lab = new QLabel("frame_x", this);
	m_frame_x = new SpinBox(this);
	QLabel* frame_y_lab = new QLabel("frame_y", this);
	m_frame_y = new SpinBox(this);
	QLabel* frame_w_lab = new QLabel("frame_w", this);
	m_frame_w = new SpinBox(this);
	QLabel* frame_h_lab = new QLabel("frame_h", this);
	m_frame_h = new SpinBox(this);

	frame_layer->addWidget(rect_w_lab, 0, 0);
	frame_layer->addWidget(m_rect_w, 0, 1);
	frame_layer->addWidget(rect_h_lab, 1, 0);
	frame_layer->addWidget(m_rect_h, 1, 1);

	frame_layer->addWidget(frame_x_lab,2,0);
	frame_layer->addWidget(m_frame_x,2,1);
	frame_layer->addWidget(frame_y_lab,3,0);
	frame_layer->addWidget(m_frame_y,3,1);
	frame_layer->addWidget(frame_w_lab,4,0);
	frame_layer->addWidget(m_frame_w,4,1);
	frame_layer->addWidget(frame_h_lab,5,0);
	frame_layer->addWidget(m_frame_h,5,1);
	QScrollArea* scroll = new QScrollArea(this);
	m_pix_widget = new BitmapEditWidget(this);
	m_pix_widget->resize(400,300);
	scroll->setWidget(m_pix_widget);
	main_layer->addWidget(scroll);

	connect(m_rect_w, &QSpinBox::textChanged, this, &BitmapEditMainWidget::slot_rectWChanged);
	connect(m_rect_h, &QSpinBox::textChanged, this, &BitmapEditMainWidget::slot_rectHChanged);

	connect(m_frame_x, &QSpinBox::textChanged, this,&BitmapEditMainWidget::slot_frameXChanged);
	connect(m_frame_y, &QSpinBox::textChanged, this, &BitmapEditMainWidget::slot_frameYChanged);
	connect(m_frame_w, &QSpinBox::textChanged, this, &BitmapEditMainWidget::slot_frameWChanged);
	connect(m_frame_h, &QSpinBox::textChanged, this, &BitmapEditMainWidget::slot_frameHChanged);
	connect(search_btn, &QPushButton::clicked, this, &BitmapEditMainWidget::slot_searchBtnClicked);
	connect(m_select_path, &QLineEdit::textChanged, this, &BitmapEditMainWidget::slot_pixmapPathChanged);
	connect(m_pix_widget, &BitmapEditWidget::frameDispChanged, this, &BitmapEditMainWidget::slot_frameChanged);
}

void BitmapEditMainWidget::setBitmap(wbBitmap* item)
{
	m_pix_widget->setPixmap(item);
}

void BitmapEditMainWidget::slot_frameChanged()
{
	wbBitmap* temp = (wbBitmap*)this->item();
	m_frame_x->blockSignals(true);
	m_frame_y->blockSignals(true);
	m_frame_x->setValue(temp->frame().x());
	m_frame_x->setValue(temp->frame().y());
	m_frame_x->blockSignals(false);
	m_frame_y->blockSignals(false);
	emit itemChanged();
}

void BitmapEditMainWidget::slot_pixmapChanged()
{
	wbBitmap* temp = (wbBitmap*)this->item();
	temp->frame().setRect(m_frame_x->value(), m_frame_y->value(), m_frame_w->value(), m_frame_h->value());
	m_pix_widget->update();
	emit itemChanged();
}

void BitmapEditMainWidget::slot_rectWChanged()
{
	m_pix_widget->item()->rect().setWidth(m_rect_w->value());
	m_pix_widget->update();
	emit itemChanged();
}

void BitmapEditMainWidget::slot_rectHChanged()
{
	m_pix_widget->item()->rect().setHeight(m_rect_h->value());
	m_pix_widget->update();
	emit itemChanged();
}

void BitmapEditMainWidget::slot_frameXChanged()
{
	m_pix_widget->item()->frame().setX(m_frame_x->value());
	m_pix_widget->update();
	emit itemChanged();
}
void BitmapEditMainWidget::slot_frameYChanged()
{
	m_pix_widget->item()->frame().setY(m_frame_y->value());
	m_pix_widget->update();
	emit itemChanged();
}
void BitmapEditMainWidget::slot_frameWChanged()
{
	m_pix_widget->item()->frame().setWidth(m_frame_w->value());
	m_pix_widget->update();
	emit itemChanged();
}
void BitmapEditMainWidget::slot_frameHChanged()
{
	m_pix_widget->item()->frame().setHeight(m_frame_h->value());
	m_pix_widget->update();
	emit itemChanged();
}

void BitmapEditMainWidget::slot_pixmapPathChanged()
{
	QString name = m_select_path->text();
	if (!name.isEmpty())
	{
		m_pix_widget->item()->setPixmap(name);
		this->setOtherProperty(m_pix_widget->item());
		m_pix_widget->update();
		emit itemChanged();
	}
}

void BitmapEditMainWidget::slot_searchBtnClicked()
{
	QString img_path = PathManager::instance()->image_path();
	QString path = QFileDialog::getOpenFileName(this, "select picture", img_path, "Image File(*.png)");
	if (!path.isEmpty())
	{
		int last = img_path.length();
		m_select_path->setText(path.mid(last,path.length()- last -4));
	}
}

void BitmapEditMainWidget::setOtherProperty(WinElementBase*item)
{
	wbBitmap* temp = (wbBitmap*)item;
	this->setBitmap(temp);
	m_rect_w->blockSignals(true);
	m_rect_h->blockSignals(true);
	m_frame_x->blockSignals(true);
	m_frame_y->blockSignals(true);
	m_frame_w->blockSignals(true);
	m_frame_h->blockSignals(true);
	m_select_path->blockSignals(true);
	m_rect_w->setValue(item->rect().width());
	m_rect_h->setValue(item->rect().height());
	this->setFrameProperty(temp->frame());
	m_select_path->setText(temp->bitmap()->_name);
	m_select_path->blockSignals(false);
	m_rect_w->blockSignals(false);
	m_rect_h->blockSignals(false);
	m_frame_x->blockSignals(false);
	m_frame_y->blockSignals(false);
	m_frame_w->blockSignals(false);
	m_frame_h->blockSignals(false);
}

void BitmapEditMainWidget::setFrameProperty(QRect& rect)
{
	m_frame_x->setValue(rect.x());
	m_frame_y->setValue(rect.y());
	m_frame_w->setValue(rect.width());
	m_frame_h->setValue(rect.height());
}
