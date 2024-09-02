#include"MenuExtra.h"

WinElementMenu::WinElementMenu(QWidget* parent):QMenu(parent)
{
	this->setTitle("WinElements");
	WE_ACTION(wbLinesText);
	WE_ACTION(wbBitmap);
}

WinElementMainMenu::WinElementMainMenu(QWidget* parent):QMenu(parent)
{
	WinElementMenu*menu=new WinElementMenu(this);
	this->addMenu(menu);
	connect(menu, &WinElementMenu::send_wetype, this, &WinElementMainMenu::slot_onSendWEType);
}
