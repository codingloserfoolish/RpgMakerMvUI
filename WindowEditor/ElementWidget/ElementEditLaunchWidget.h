#ifndef ELEMENTEDITLUNCHWIDGET_H_
#define ELEMENTEDITLUNCHWIDGET_H_
#include<qwidget.h>
#include"../WindowElement/WinElementBase.h"
#include"ElementWidgetBase/ElementWidgetBase.h"

class ElementEditLaunchWidget:public QObject
{
	Q_OBJECT
public:
	ElementEditLaunchWidget(QObject*parent=nullptr);
	~ElementEditLaunchWidget();
	void LaunchEditWidget(WinElementBase*item);
	void updateWidget(WinElementBase* item);
	void updateDispPos();
	void closeAll();
signals:
	void itemChanged();
public slots:
	void slot_itemChanged() { emit itemChanged(); }
private:
	ElementWidgetBase* m_current;
	WinElementType m_weType;
};

#endif // !ELEMENTEDITLUNCHWIDGET_H_
