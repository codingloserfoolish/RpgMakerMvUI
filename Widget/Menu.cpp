#include"Menu.h"
#include"../manager/ObjectLoader.h"
#include <QIntValidator>
#include<QHBoxLayout>
#include<QMouseEvent>
#include<QLabel>

#include<QDebug>
namespace hjdd
{
	namespace widget
	{
		ObjectsListMenu::ObjectsListMenu(QWidget* parent):
			QMenu(parent)
		{
			this->installEventFilter(this);
			this->setTitle("创建");
			for (QMap<QString, manager::Library*>::iterator it = manager::ObjectLoader::instance()->libinfo().begin();
				it != manager::ObjectLoader::instance()->libinfo().end();
				++it)
			{
				this->addAction(it.key());
			}
		}

		bool ObjectsListMenu::eventFilter(QObject* obj, QEvent* e)
		{
			if (e->type() == QEvent::MouseButtonPress)
			{
				QMouseEvent* m_e = (QMouseEvent*)e;
				if (m_e->button() == Qt::LeftButton)
				{
					QAction* action = this->actionAt(m_e->pos());
					if (action)
					{
						int type = manager::ObjectLoader::instance()->loadObjectLib(action->text());
						if (type != -1)
						{
							emit this->rpgobject_type(type);
						}
					}
				}
			}
			return QMenu::eventFilter(obj,e);
		}

		ObjectPopCtxMenu::ObjectPopCtxMenu(QWidget* parent):
			QMenu(parent)
		{
			ObjectsListMenu* objlists_menu = new ObjectsListMenu(this);
			this->addMenu(objlists_menu);
			connect(objlists_menu, &ObjectsListMenu::rpgobject_type, [this](int param) {emit this->triggerId(OBJECT_MENU_CREATE,param); });
			ACTION("编辑", triggerId, OBJECT_MENU_EDIT,0);
			ACTION("删除", triggerId,OBJECT_MENU_DELETE,0);
		}

		FileMenu::FileMenu(QWidget* parent):
			QMenu(parent)
		{
			this->setTitle("文件");
			ACTION("保存", triggerId, FILE_MENU_SAVE);
			ACTION("加载", triggerId, FILE_MENU_LOAD);
		}
		CompileMenu::CompileMenu(QWidget* parent):
			QMenu(parent)
		{
			this->setTitle("生成");
			ACTION("生成Js", triggerId, FILE_MENU_JSCOMPILE);
			ACTION("重新生成Js", triggerId, FILE_MENU_REJSCOMPILE);
		}
		/*废弃
		SizeSettingDlg::SizeSettingDlg(const QSize& size, QWidget* parent):
			QDialog(parent)
		{
			m_widthEdit = new QLineEdit(QString::number(size.width()),this);
			m_heightEdit = new QLineEdit(QString::number(size.height()),this);
			QIntValidator* validator = new QIntValidator(480,2048, this);

			m_widthEdit->setValidator(validator);
			m_heightEdit->setValidator(validator);

			QHBoxLayout* layout = new QHBoxLayout(this);
			this->setLayout(layout);
			layout->addWidget(new QLabel("水平", this));
			layout->addWidget(m_widthEdit);
			layout->addWidget(new QLabel("垂直",this));
			layout->addWidget(m_heightEdit);
		}

		void SizeSettingDlg::closeEvent(QCloseEvent* e)
		{
			m_width = m_widthEdit->text().toInt();
			m_height= m_heightEdit->text().toInt();
			if (m_width < 480)m_width=480;
			if (m_height < 480)m_height = 480;
			this->accept();
		}

		SizeSettingMenu::SizeSettingMenu(QWidget* parent) :
			QMenu(parent)
		{
			this->setTitle("Graphics大小");
			m_group = new QActionGroup(this);
			m_group->setExclusive(true);

			QAction*es= this->addAction("800x600");
			es->setCheckable(true);
			es->setChecked(true);
			m_selected = es;

			QAction* ts = this->addAction("1024x768");
			ts->setCheckable(true);
			
			m_group->addAction(es);
			m_group->addAction(ts);

			m_self = this->addAction("自定");
			m_self->setCheckable(true);

			connect(m_group, &QActionGroup::triggered, this, &SizeSettingMenu::slot_triggerAction);
			connect(m_self, &QAction::triggered, this, &SizeSettingMenu::slot_selfTriggered);
		}
		void SizeSettingMenu::initSize(const QSize& size)
		{
			QString cmp = QString("%1x%2").arg(size.width()).arg(size.height());
			//初始化时候不允许发送消息
			m_group->blockSignals(true);
			for (QAction* action : m_group->actions())
			{
				if (action->text() == cmp)
				{
					action->setChecked(true);
					m_group->blockSignals(false);
					return;
				}
			}
			m_group->blockSignals(true);
			m_selected->setChecked(false);
			m_group->blockSignals(false);

			m_self->setChecked(true);
			m_self->setText(QString("自定(%1)").arg(cmp));
		}
		void SizeSettingMenu::slot_selfTriggered()
		{
			QString old = m_self->text();
			QSize size;
			m_group->blockSignals(true);
			m_selected->setChecked(false);
			m_group->blockSignals(false);

			QRegExp reg("([0-9]+)x([0-9]+)");
			if (-1 == old.lastIndexOf(reg))
			{
				size = QSize(800,600);
			}
			else
			{
				qDebug() << reg.cap(0);
				size = QSize(reg.cap(1).toInt(),reg.cap(2).toInt());
			}

			SizeSettingDlg dlg(size);
			if (QDialog::Accepted==dlg.exec())
			{
				size.setWidth(dlg.swidth());
				size.setWidth(dlg.sheight());

				m_self->setText(QString("自定(%1x%2)").arg(dlg.swidth()).arg(dlg.sheight()));
				m_self->setChecked(true);

				emit this->sendSizeChanged(size);
			}
			
		}
		void SizeSettingMenu::slot_triggerAction(QAction* action)
		{
			if (action->isChecked())
			{
				if (m_self->isChecked())m_self->setChecked(false);
				m_selected = action;
				QString text = action->text();
				QStringList list = text.split('x');
				emit this->sendSizeChanged(QSize(list.at(0).toInt(),list.at(1).toInt()));
			}
			else
			{
				action->setChecked(true);
			}
		}*/
		
}
}