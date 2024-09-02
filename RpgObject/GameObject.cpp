#include"GameObject.h"
#include<qpixmap.h>
#include<cmath>
#include"RpgObjectFactory.h"
#include"../Manager/RpgObjectEditorManager.h"
#include"../RpgObjectEditor/RpgObjectEditorBase.h"
int GameObject::_object_name_index = 0;

GameObject::GameObject(int canvas_w, int canvas_h, QObject* parent)
	:Transform2D(parent),
	m_active(false),
	m_canvas_width(canvas_w),
	m_canvas_height(canvas_h),
	m_parent(0)
{
	this->setObjectName(this->create_base_name());


	m_bind_standardItem = new QStandardItem(this->objectName());
	m_bind_standardItem->setEditable(false);
	m_bind_standardItem->setData(QVariant::fromValue(GameObject_Data(this)), GAMEOBJECT_ROLE);
}

GameObject::~GameObject()
{
	for (GameObject*& child : m_children) { 
		delete child;
		child = 0; 
	}
	this->on_destroy();
	if(m_parent)
	m_parent->standard_item()->removeRow(m_bind_standardItem->row());
}

void GameObject::clear()
{
	for (GameObject*& child : m_children) { delete child;}
	m_children.clear();
}

void GameObject::draw(QPainter& p)
{
	QPixmap canvas(m_canvas_width, m_canvas_height);
	canvas.fill(Qt::transparent);
	QPainter p_self(&canvas);
	p_self.setFont(p.font());
	p_self.translate(m_canvas_width / 2, m_canvas_height / 2);
	this->draw_self(p_self);
	for (GameObject*& child : m_children)
	{
		child->draw(p_self);
	}
	p_self.end();

	p.save();
	transform_painter(p);
	p.drawPixmap(-m_canvas_width / 2, -m_canvas_height / 2,canvas);
	p.restore();
}

GameObject* GameObject::pickup(float x, float y,QPointF* diff)
{
	GameObject* item = 0;
	QPointF l_pos = transform_point(x, y);
	if (this->contain(l_pos))
	{
		for (GameObject*& child : m_children)
		{
			if (0!=(item = child->pickup(l_pos.x(), l_pos.y(),diff)))
			{
				return item;
			}
		}
		*diff = l_pos;
		return this;
	}
	return 0;
}

Matrix3 GameObject::global_getTransMatrixIn()
{
	Matrix3 mat = getTransMatrixIn();
	Matrix3 right;
	GameObject* parent = m_parent;
	while (parent)
	{
		right = parent->getTransMatrixIn();
		mat = Mat_mutiply(mat, right);
		parent = parent->m_parent;
	}
	return mat;
}

Matrix3 GameObject::global_getTransMatrixOut()
{
	Matrix3 mat = getTransMatrixOut();
	Matrix3 left;
	GameObject* parent = m_parent;
	while (parent)
	{
		left = parent->getTransMatrixOut();
		mat = Mat_mutiply(left,mat);
		parent = parent->m_parent;
	}
	return mat;
}

float GameObject::global_angle()
{
	float result=m_angle;
	GameObject* parent = m_parent;
	while (parent)
	{
		result += parent->m_angle;
		parent = parent->gm_parent();
	}
	return result;
}

void GameObject::addChild(GameObject* object)
{
	object->m_parent = this;
	m_children.push_back(object);
	m_bind_standardItem->setChild(m_bind_standardItem->rowCount(), object->standard_item());
}

void GameObject::removeChild(GameObject* object)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if (*it == object)
		{
			RpgObjectEditorBase* win = RpgObjectEditorManager::instance()->editor(object);
			if (win->isVisible())
			{
				win->close();
			}
			m_children.erase(it);
			delete object;
			return;
		}
	}
}

bool GameObject::contain(QPointF pos)
{
	return std::fabsf(pos.x()) <= m_canvas_width / 2 && std::fabsf(pos.y()) <= m_canvas_height / 2;
}

QString GameObject::guarantee_name_super(QString& name)
{
	GameObject* parent = this->m_parent;
	if (!parent)
	{
		if (name == this->objectName())
		{
			return QString("%1_%2").arg(name).arg(_object_name_index++);
		}
		return name;
	}
	for (std::vector<GameObject*>::reverse_iterator it = parent->m_children.rbegin(); it != parent->m_children.rend(); ++it)
	{
		if ((*it)->objectName() == name)
		{
			return QString("%1_%2").arg(name).arg(_object_name_index++);
		}
	}
	return parent->guarantee_name_super(name);
}

QString GameObject::guarantee_name_low(QString& name)
{
	QString ret= name;
	for (std::vector<GameObject*>::reverse_iterator it = this->m_children.rbegin(); it != this->m_children.rend(); ++it)
	{
		if (name != (ret=(*it)->guarantee_name_low(name)))
		{
			return ret;
		}
	}
	if (name == this->objectName())return name + _object_name_index++;
	else return ret;
}

QString GameObject::create_base_name()
{
	return QString("object%1").arg(_object_name_index++);
}

void GameObject::Js_Context(QTextStream&textstream)
{
	textstream << Js_NewObject();
	textstream << Js_AttributeSet()<<"\n";
	this->Js_ExtraData(textstream);
	for (GameObject* child : m_children)
	{
		child->Js_Context(textstream);
	}
}

void GameObject::Xml_Save_Content(QDomDocument& doc,QDomElement& parent_node)
{
	QDomElement item = this->Xml_SaveData(doc,parent_node);
	for (GameObject* child : m_children)
	{
		child->Xml_Save_Content(doc,item);
	}
}

void GameObject::Xml_Load_Content(QDomNode& self)
{
	QDomNode child=this->Xml_LoadData(self);
	GameObject* item = 0;
	while (!child.isNull())
	{
		item = RpgObjectFactory::create(child.nodeName());
		if (item)
		{
			item->Xml_Load_Content(child);
			this->addChild(item);
		}
		item = 0;
		child = child.nextSibling();
	}
}
