#include"WindowBaseEditor.h"
#include"../WindowBaseEdit/Widget/ElementDisplayWidget.h"
#include"../WindowBaseEdit/ElementEditManager/ElementLoader.h"
namespace hjdd
{
	namespace widget
	{
		WindowBaseEditor::WindowBaseEditor(QWidget* parent):
			ObjectEditor(parent),
			m_tinyWorld(0){}
		WindowBaseEditor::~WindowBaseEditor()
		{
			manager::ElementEditorManager::instance()->destroyAll();
		}
		void WindowBaseEditor::slot_updateItem()
		{
			if (this->item())
			{
				((item::WindowBase*)this->item())->refreshElementsCanvas();
				this->item()->update();
			}
		}
		void WindowBaseEditor::initLayOut(QVBoxLayout* vlayout)
		{
			m_tinyWorld = new ElementDisplayWidget(this);
			vlayout->addWidget(m_tinyWorld);
		}
		void WindowBaseEditor::setProperties(item::GraphicsItem* item)
		{
			item::WindowBase* win = (item::WindowBase*)item;
			m_tinyWorld->setObject(win);
			m_tinyWorld->setMinimumSize(win->width(),win->height());
		}
		void WindowBaseEditor::Connection()
		{
			connect(m_tinyWorld, &ElementDisplayWidget::elementChanged, this,&WindowBaseEditor::slot_updateItem);
		}
		void WindowBaseEditor::DisConnection()
		{
			disconnect(m_tinyWorld, 0 ,this,0);
			m_tinyWorld->setObject(0);
		}
		void WindowBaseEditor::closeEvent(QCloseEvent* e)
		{
			manager::ElementEditorManager::instance()->close();
		}
	}
}