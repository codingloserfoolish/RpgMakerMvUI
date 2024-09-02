#ifndef BITMAPEDITWIDGET_H_
#define BITMAPEDITWIDGET_H_
#include<qwidget.h>
#include<qlineedit.h>
#include"../ElementWidgetBase/ElementWidgetBase.h"
class wbBitmap;
class BitmapEditWidget:public QWidget
{
	Q_OBJECT
public:
	BitmapEditWidget(QWidget* parent = nullptr);
	void setPixmap(wbBitmap* bitmap);
	wbBitmap* item()const { return m_bitmap; }
	QRect adjustFrameRect(int dx,int dy);
signals:
	void frameDispChanged();
protected:
	virtual void paintEvent(QPaintEvent*)override;
	virtual void mousePressEvent(QMouseEvent* e)override;
	virtual void mouseMoveEvent(QMouseEvent* e)override;
private:
	wbBitmap* m_bitmap;
	QPen m_frame_color;
	QPoint m_diff;
};

class BitmapEditMainWidget:public ElementWidgetBase
{
	Q_OBJECT
public:
	BitmapEditMainWidget(QWidget* parent = nullptr);
	void setBitmap(wbBitmap*item);
public slots:
	void slot_frameChanged();
	void slot_pixmapChanged();
	void slot_rectWChanged();
	void slot_rectHChanged();
	void slot_frameXChanged();
	void slot_frameYChanged();
	void slot_frameWChanged();
	void slot_frameHChanged();
	void slot_pixmapPathChanged();
	void slot_searchBtnClicked();
protected:
	virtual void setOtherProperty(WinElementBase*)override;
private:
	void setFrameProperty(QRect&rect);
	QLineEdit* m_select_path;

	SpinBox* m_rect_w;
	SpinBox* m_rect_h;

	SpinBox* m_frame_x;
	SpinBox* m_frame_y;
	SpinBox* m_frame_w;
	SpinBox* m_frame_h;

	BitmapEditWidget* m_pix_widget;
};
#endif // !BITMAPEDITWIDGET_H_
