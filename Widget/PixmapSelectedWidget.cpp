#include"PixmapSelectedWidget.h"
#include<QHBoxLayout>
#include<qfiledialog.h>
#include"../Manager/PathManager.h"
PixmapSelectedWidget::PixmapSelectedWidget(QWidget* parent):QWidget(parent),m_label(0),m_button(0)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);
	m_label = new QLabel(".png", this);
	m_button = new QPushButton("select", this);
	m_button->setFixedWidth(60);
	layout->addWidget(m_label);
	layout->addWidget(m_button);
	this->setLayout(layout);
	connect(m_button, &QPushButton::clicked, this, &PixmapSelectedWidget::slot_PixmapChanged);
}

void PixmapSelectedWidget::setValue(const QString&name)
{
	m_label->setText(name + ".png");
}

void PixmapSelectedWidget::slot_PixmapChanged()
{
	QString img_path = PathManager::instance()->image_path();
	QString path = QFileDialog::getOpenFileName(this, "selected picture", img_path, QString("Image File(*.png)"));
	if (path.isEmpty())return;
	int last_splitter = img_path.length();
	QString name = path.mid(last_splitter, path.length() - last_splitter - 4);
	this->setValue(name);
	emit valueChanged(name);
}