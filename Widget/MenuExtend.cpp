#include"MenuExtend.h"
#include<qaction.h>
FileMenu::FileMenu(QWidget* parent):QMenu("File", parent)
{
	QAction* load_action = new QAction("load", this);
	this->addAction(load_action);
	connect(load_action, &QAction::triggered, this, &FileMenu::slot_load_trigger);

	QAction* save_action = new QAction("save", this);
	this->addAction(save_action);
	connect(save_action, &QAction::triggered, this, &FileMenu::slot_save_trigger);
}

RpgObjectMenu::RpgObjectMenu(QWidget* parent) :QMenu("RpgObject", parent)
{
	ACTION_OBJECT(window_base, Window_Base, 0);
	ACTION_OBJECT(sprite, Sprite, 1);
	ACTION_OBJECT(text, Text, 2);
}

ContextPopMenu::ContextPopMenu(QWidget* parent):QMenu(parent)
{
	RpgObjectMenu* createMenu = new RpgObjectMenu(this);
	createMenu->setTitle("create");
	this->addMenu(createMenu);
	this->addSeparator();
	QAction*deleteAction=this->addAction("delete");

	connect(createMenu, &RpgObjectMenu::create_object, this, std::bind(&ContextPopMenu::slot_sendSignal,this,std::placeholders::_1,CONTEXT_BEHAVIOR_CREATE));
	connect(deleteAction, &QAction::triggered, this, std::bind(&ContextPopMenu::slot_sendSignal, this, 0, CONTEXT_BEHAVIOR_DELETE));
}
