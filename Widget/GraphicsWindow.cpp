#include"GraphicsWindow.h"
#include<QMouseEvent>
#include<qpainter.h>
#include<qdebug.h>

GraphicsWindow::GraphicsWindow(QWidget* parent):QWidget(parent), m_active_object(0),m_scene(800,600)
{
	this->setFixedSize(800, 600);
	this->initContextPopMenu();
}

void GraphicsWindow::initContextPopMenu()
{
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	m_pCtMenu = new ContextPopMenu(this);
	connect(this, &GraphicsWindow::customContextMenuRequested, this, &GraphicsWindow::slot_mousePopMenuEvent);
	connect(m_pCtMenu, &ContextPopMenu::trigger_event, this, &GraphicsWindow::slot_responseTriggerPopMenuEvent);
}

void GraphicsWindow::generateJs()
{
	QFile Jsfile(this->Jsfile_name());
	Jsfile.open(QIODevice::WriteOnly);
	QTextStream stream(&Jsfile);
	m_scene.Js_Context(stream);
	Jsfile.close();
	qDebug() << "over";
}

void GraphicsWindow::saveXml()
{
	QFile Xmlfile(this->Xmlfile_name());
	Xmlfile.open(QIODevice::WriteOnly);

	QDomDocument document;
	QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version='1.0' encoding='utf-8'");
	document.appendChild(instruction);

	m_scene.Xml_Save_Content(document, document.createElement(""));

	QTextStream stream(&Xmlfile);
	document.save(stream, 2);
	Xmlfile.close();
}

void GraphicsWindow::slot_mousePopMenuEvent(const QPoint& pos)
{
	m_menuPos = pos;
	m_pCtMenu->exec(this->mapToGlobal(pos));
}

void GraphicsWindow::slot_gameObjectChanged(GameObject* active)
{
	if (this->m_active_object != active)
	{
		if (this->m_active_object) 
		{ 
			this->m_active_object->setActive(false);
		}
		this->m_active_object = active;
		this->m_active_object->setActive(true);
		emit gameObjectBehavior(GAMEOBJECT_CHANGE, active);
	}
}

void GraphicsWindow::slot_gameObjectPropertyChanged(QtProperty*_property_, const QVariant&value)
{
	const QMetaObject* metaobj = m_active_object->metaObject();

	//Ãû×Ö
	if (_property_->propertyName() == metaobj->property(0).name())
	{
		QString name = value.toString();
		if (name.isEmpty())return;
		QString new_name=m_active_object->guarantee_name_low(value.toString());
		if (name != new_name)
		{
			m_active_object->setObjectName(new_name);
			m_active_object->standard_item()->setText(name);
			emit gameObjectBehavior(GAMEOBJECT_NAME_CHANGE, m_active_object);
		}
		else
		{
			new_name = m_active_object->guarantee_name_super(value.toString());
			if (name != new_name)
			{
				m_active_object->setObjectName(new_name);
				m_active_object->standard_item()->setText(name);
				emit gameObjectBehavior(GAMEOBJECT_NAME_CHANGE, m_active_object);
			}
			else
			{
				m_active_object->setObjectName(name);
				m_active_object->standard_item()->setText(name);
				emit gameObjectBehavior(GAMEOBJECT_NAME_CHANGE, m_active_object);
			}
		}
		return;
	}


	for (int i = 1; i < metaobj->propertyCount(); ++i)
	{
		if ((metaobj->property(i)).name() == _property_->propertyName())
		{
			metaobj->property(i).write(m_active_object, value);
			break;
		}
	}
	emit gameObjectBehavior(GAMEOBJECT_PROPERTY_UPDATE, m_active_object);
	update();
}

void GraphicsWindow::slot_generateObject(int type)
{
	m_scene.CreateObject(m_active_object, 0, 0, type);
	//emit create_completed(item->objectName());
	this->update();
}

void GraphicsWindow::slot_responseTriggerPopMenuEvent(int behavior, int param)
{
	if(behavior== CONTEXT_BEHAVIOR_DELETE)
	{
		if (m_active_object&&m_active_object!=&m_scene)
		{
			m_active_object->gm_parent()->removeChild(m_active_object);
		}
	}
	else if(behavior==CONTEXT_BEHAVIOR_CREATE)
	{
		m_scene.CreateObject(m_active_object, m_menuPos.x()-m_scene.pos().x(), m_menuPos.y()-m_scene.pos().y(), param);
	}
	this->update();
}

void GraphicsWindow::slot_Save()
{
	this->generateJs();
	this->saveXml();
}

void GraphicsWindow::slot_Load()
{
	m_scene.clear();
	QFile Xmlfile(this->Xmlfile_name());
	Xmlfile.open(QIODevice::ReadOnly);
	QDomDocument doc;
	doc.setContent(&Xmlfile);
	m_scene.Xml_Load_Content(doc.lastChild());
	Xmlfile.close();

	this->update();
}


void GraphicsWindow::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	m_scene.draw(p);
	p.end();
}

void GraphicsWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		GameObject*temp= m_scene.pickup(event->pos().x(), event->pos().y(), &m_diff);
		int signal_type = 0;
		QObject* value = 0;
		bool is_emit = false;
		if (temp == 0)
		{
			if (m_active_object)
			{
				m_active_object->setActive(false);
				signal_type = GAMEOBJECT_DEACTIVE;
				value = 0;
				is_emit = true;
			}
			m_active_object = temp;
			if (is_emit)emit gameObjectBehavior(signal_type, value);
		}
		else if (temp != m_active_object)
		{
			if (!m_active_object)
			{
				signal_type = GAMEOBJECT_ACTIVE;
				value = temp;
				is_emit = true;
			}
			else
			{
				m_active_object->setActive(false);
				signal_type = GAMEOBJECT_CHANGE;
				value = temp;
				is_emit = true;
			}
			m_active_object = temp;
			m_active_object->setActive(true);
			if(is_emit)emit gameObjectBehavior(signal_type, value);
		}
	}
}

void GraphicsWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (m_active_object&&event->buttons()&Qt::LeftButton)
	{
		Matrix3 mat = m_active_object->global_getTransMatrixIn();
		Vector3 point = { event->x(),event->y(),1 };
		point = Mat_Vec_mutiply(mat, point);
		m_active_object->translate(point._1 - m_diff.x(), point._2 - m_diff.y());
		update();
		emit gameObjectBehavior(GAMEOBJECT_MOVE, m_active_object);
	}
}
