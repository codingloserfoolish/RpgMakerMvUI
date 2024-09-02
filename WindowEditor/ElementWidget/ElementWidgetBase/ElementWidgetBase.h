#ifndef ELEMENTWIDGETBASE_H_
#define ELEMENTWIDGETBASE_H_
#include<qwidget.h>
class SpinBox;
class WinElementBase;
class ElementWidgetBase :public QWidget
{
	Q_OBJECT
public:
	ElementWidgetBase(QWidget* parent = nullptr);
	void setDispValue(int x,int y);
	void updateDispValue();
	void setValue(WinElementBase* element);
	WinElementBase* item()const { return m_item; }
signals:
	void itemChanged();
public slots:
	void slot_sendDispChanged();
protected:
	virtual void setOtherProperty(WinElementBase*)=0;
private:
	SpinBox* m_disp_x;
	SpinBox* m_disp_y;
	WinElementBase* m_item;
};
#endif // !ELEMENTWIDGETBASE_H_
