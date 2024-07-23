#ifndef PIXMAPSELECTEDWIDGET_H_
#define PIXMAPSELECTEDWIDGET_H_
#include<qwidget.h>
#include<qlabel.h>
#include<qpushbutton.h>

class PixmapSelectedWidget :public QWidget
{
	Q_OBJECT
public:
	PixmapSelectedWidget(QWidget* parent = nullptr);
	void setValue(const QString&);
public slots:
	void slot_PixmapChanged();
signals:
	void valueChanged(const QString&);
private:
	QLabel* m_label;
	QPushButton* m_button;
};

#endif // !PIXMAPSELECTEDWIDGET_H_
