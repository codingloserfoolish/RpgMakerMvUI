#include"Menu.h"
#include <QIntValidator>
#include<QHBoxLayout>
#include<QMouseEvent>
#include<QLabel>
#include"../ElementEditManager/ElementLoader.h"
#include<QDebug>
namespace hjdd
{
	namespace widget
	{
		ElementsListMenu::ElementsListMenu(QWidget* parent):
			QMenu(parent)
		{
			this->installEventFilter(this);
			this->setTitle("创建");
			for (QMap<QString, manager::ElementLibrary*>::iterator it = manager::ElementLibraryLoader::instance()->libinfo().begin();
				it != manager::ElementLibraryLoader::instance()->libinfo().end();
				++it)
			{
				this->addAction(it.key());
			}
		}

		bool ElementsListMenu::eventFilter(QObject* obj, QEvent* e)
		{
			if (e->type() == QEvent::MouseButtonPress)
			{
				QMouseEvent* m_e = (QMouseEvent*)e;
				if (m_e->button() == Qt::LeftButton)
				{
					QAction* action = this->actionAt(m_e->pos());
					if (action)
					{
						int type = manager::ElementLibraryLoader::instance()->loadElementLib(action->text());
						if (type != -1)
						{
							emit this->element_type(type);
						}
					}
				}
			}
			return QMenu::eventFilter(obj,e);
		}

		ElementPopCtxMenu::ElementPopCtxMenu(QWidget* parent):
			QMenu(parent)
		{
			ElementsListMenu* objlists_menu = new ElementsListMenu(this);
			this->addMenu(objlists_menu);
			connect(objlists_menu, &ElementsListMenu::element_type, [this](int param) {emit this->triggerId(OBJECT_MENU_CREATE,param); });
			ACTION("编辑", triggerId, OBJECT_MENU_EDIT,0);
			ACTION("删除", triggerId,OBJECT_MENU_DELETE,0);
		}
	}
}