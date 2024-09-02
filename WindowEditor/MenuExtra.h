#ifndef MENUEXTRA_H_
#define MENUEXTRA_H_
#include<qmenu.h>
#define WE_ACTION(name)\
QAction*v_##name=this->addAction(#name);\
connect(v_##name,&QAction::triggered,std::bind(&WinElementMenu::slot_onSendWEType,this,#name));

class WinElementMenu :public QMenu
{
	Q_OBJECT
public:
	WinElementMenu(QWidget* parent = nullptr);
signals:
	void send_wetype(const QString&);
public slots:
	void slot_onSendWEType(const QString& name) { emit send_wetype(name); }
};

class WinElementMainMenu :public QMenu
{
	Q_OBJECT
public:
	WinElementMainMenu(QWidget* parent = nullptr);
signals:
	void send_wetype(const QString&);
public slots:
	void slot_onSendWEType(const QString& name) { emit send_wetype(name); }

};
#endif // !MENUEXTRA_H_
