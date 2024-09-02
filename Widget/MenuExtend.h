#ifndef MENUEXTEND_H_
#define MENUEXTEND_H_
#include<qmenu.h>
#include<functional>
class FileMenu :public QMenu
{
	Q_OBJECT
public:
	FileMenu(QWidget* parent = nullptr);
public slots:
	void slot_load_trigger() {emit load_trigger();}
	void slot_save_trigger(){ emit save_trigger();}
signals:
	void load_trigger();
	void save_trigger();
};


#define ACTION_OBJECT(class_name,type)\
connect(this->addAction(#class_name),&QAction::triggered,this,std::bind(&RpgObjectMenu::slot_create_object,this,type))

class RpgObjectMenu :public QMenu
{
	Q_OBJECT
public:
	RpgObjectMenu(QWidget* parent = nullptr);
public slots:
	void slot_create_object(int type) {emit create_object(type);}
signals:
	void create_object(int type);
};


#define CONTEXT_BEHAVIOR_DELETE 1
#define CONTEXT_BEHAVIOR_CREATE 2
#define CONTEXT_BEHAVIOR_EDIT   3

class ContextPopMenu:public QMenu
{
	Q_OBJECT
public:
	ContextPopMenu(QWidget* parent = nullptr);
public slots:
	void slot_sendSignal(int param, int behavior) { emit trigger_event(behavior,param); };
signals:
	void trigger_event(int behavior,int param);
};


#endif // !MENUEXTEND_H_
