#include"xmlParser.h"
#include"../manager/ObjectLoader.h"
#include"../manager/ConfigInfo.h"
#include<QDebug>
namespace hjdd
{
	namespace xml
	{
		XmlParser::XmlParser()
		{
		}
		void XmlParser::saveSceneToXml(const QString& SceneName, scene::GraphicsScene* scene)
		{	
			/*
			* 保存路径以及文件名
			* ....
			*/
			m_dom.clear();
			m_dom.appendChild(m_dom.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
			QDomElement root = m_dom.createElement(SceneName);
			m_dom.appendChild(root);
			for (QGraphicsItem* item : scene->childItems())
			{
				this->appendElement((item::GraphicsItem*)item, root);
			}

			QFile saveFile(manager::ConfigInfo::instance()->savePath()+SceneName+".xml");
			if (saveFile.open(QIODevice::WriteOnly))
			{
				QTextStream stream(&saveFile);
				m_dom.save(stream,4);
				saveFile.close();
			}

		}
		bool XmlParser::loadSceneFromXml(const QString& SceneName, scene::GraphicsScene* scene)
		{
			m_dom.clear();
			QFile xmlFile(manager::ConfigInfo::instance()->savePath()+SceneName+".xml");
			if (!xmlFile.open(QIODevice::ReadOnly))
			{
				return false;
			}
			if (!m_dom.setContent(&xmlFile))
			{
				xmlFile.close();
				return false;
			}
			xmlFile.close();
			//开始
			const QDomElement root = m_dom.documentElement();
			if (root.tagName() != SceneName)return false;

			QDomElement object = root.firstChildElement();
			scene->blockSignals(true);
			scene->resetScene();
			while (!object.isNull())
			{
				loadItemFromElement(scene, object);
				object = object.nextSiblingElement();
			}
			scene->blockSignals(false);
			for (item::GraphicsItem* child : scene->childItems())
			{
				child->update();
			}
			//qDebug() << scene->sceneRect();
			//收尾
			return true;
		}



		void XmlParser::appendElement(item::GraphicsItem* item, QDomElement& parentElement)
		{
			QDomElement object = m_dom.createElement("Object");
			object.setAttribute("dllname", manager::ObjectLoader::instance()->typeToName(item->rpgobject_type()));
			object.setAttribute("name", item->objectName());
			object.appendChild(TransformProperty(item));
			object.appendChild(SizeProperty(item));
			/*
			* 如何给物体添加自定义属性
			*/
			object.appendChild(item->OthersProperties(m_dom));
			
			//添加子物体
			if (item->childItems().size() != 0)
			{
				QDomElement subobjects = m_dom.createElement("SubObjects");
				for (QGraphicsItem* child : item->childItems())
				{
					appendElement((item::GraphicsItem*)child, subobjects);
				}
				object.appendChild(subobjects);
			}
			//收尾
			parentElement.appendChild(object);
		}
		void XmlParser::loadItemFromElement(scene::GraphicsScene* scene, QDomElement& itemElement)
		{
			item::GraphicsItem* object = scene->appendItem(manager::ObjectLoader::instance()->loadObjectLib(itemElement.attribute("dllname")));
			object->setObjectName(itemElement.attribute("name"));
			
			QDomElement element = itemElement.firstChildElement();
			QString tag;
			while (!element.isNull())
			{
				tag = element.tagName();
				if (tag == "Transform")
				{
					LoadTransformProperty(object, element);
				}
				else if (tag == "Size")
				{
					LoadSizeProperty(object, element);
				}
				else if (tag == "SubObjects")
				{
					LoadSubObjects(scene, object, element);
				}
				else
				{
					object->OthersPropertiesLoad(element);
				}
				element = element.nextSiblingElement();
			}
		}


		QDomElement XmlParser::TransformProperty(item::GraphicsItem* object)
		{
			QDomElement transformNode = m_dom.createElement("Transform");
			//x,y,rotation
			QDomElement x = m_dom.createElement("x");
			x.appendChild(m_dom.createTextNode(QString::number(object->x(),'f',1)));
			QDomElement y = m_dom.createElement("y");
			y.appendChild(m_dom.createTextNode(QString::number(object->y(), 'f', 1)));
			QDomElement rotation = m_dom.createElement("rotation");
			rotation.appendChild(m_dom.createTextNode(QString::number(object->rotation(), 'f', 1)));

			transformNode.appendChild(x);
			transformNode.appendChild(y);
			transformNode.appendChild(rotation);
			return transformNode;

		}
		QDomElement XmlParser::SizeProperty(item::GraphicsItem* object)
		{
			QDomElement sizeNode = m_dom.createElement("Size");
			//width,height
			QDomElement width = m_dom.createElement("width");
			width.appendChild(m_dom.createTextNode(QString::number(object->width())));
			QDomElement height = m_dom.createElement("height");
			height.appendChild(m_dom.createTextNode(QString::number(object->height())));

			sizeNode.appendChild(width);
			sizeNode.appendChild(height);
			return sizeNode;
		}
		void XmlParser::LoadTransformProperty(item::GraphicsItem* object, QDomElement transformNode)
		{
			if (transformNode.isNull())return;
			QDomElement x = transformNode.firstChildElement();
			QDomElement y = x.nextSiblingElement();
			QDomElement rotation =y.nextSiblingElement();
			object->setPos(DOUBLE(x), DOUBLE(y));
			object->setRotation(DOUBLE(rotation));
		}
		void XmlParser::LoadSizeProperty(item::GraphicsItem* object, QDomElement sizeNode)
		{
			QDomElement width = sizeNode.firstChildElement();
			QDomElement height = width.nextSiblingElement();
			
			object->setWidth(INT(width));
			object->setHeight(INT(height));
		}
		void XmlParser::LoadSubObjects(scene::GraphicsScene* scene, item::GraphicsItem* parentObject, QDomElement& SubObjectsElement)
		{
			QDomElement child = SubObjectsElement.firstChildElement();

			item::GraphicsItem* pre = item::GraphicsItem::_selected_item;//保存原来的被选中物
			//开头，切换物体，开始添加
			item::GraphicsItem::_selected_item = parentObject;
			parentObject->active();

			while (!child.isNull())
			{
				loadItemFromElement(scene, child);
				child = child.nextSiblingElement();
			}
			//收尾，让物体失活
			parentObject->deactive();
			//还原现场
			item::GraphicsItem::_selected_item = pre;
		}
	}
}