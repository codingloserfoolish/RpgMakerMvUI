#pragma once
#include"../scene/GraphicsScene.h"
#include"../item/GraphicsItem.h"
#include<QFile>
#include<QTextStream>

//对于QDomText只能通过QDomNode获取值
#define INT(element) element.firstChild().nodeValue().toInt()
#define DOUBLE(element) element.firstChild().nodeValue().toDouble()
#define VALUE(element) element.firstChild().nodeValue()
namespace hjdd
{
	namespace xml
	{
		/*
		* <?xml version="1.0" encoding="UTF-8"?>
		* <Scene>
		*	<Object name="objectName()" dllname="">
		*		<Transform>
		*			<x></x>
		*			<y></y>
		*			<rotation></rotation>
		*		</Transform>
		*		<Size>
		*			<width></width>
		*			<height></height>
		*		</Size>
		*		....OthersProperties....
		*		<SubObjects>
		*			...
		*		</SubObjects>
		*	</Object>
		* ....
		* </Scene>
		*/

		/*
		* 命名说明
		* 带	object	后缀的全是	item::GraphicsItem*
		* 带	element	后缀的全是	QDomElement
		*/
		class XmlParser
		{
		public:
			XmlParser();
			void saveSceneToXml(const QString&SceneName,scene::GraphicsScene*scene);
			bool loadSceneFromXml(const QString& SceneName, scene::GraphicsScene* scene);

			//根据item添加xml元素
			void appendElement(item::GraphicsItem*item,QDomElement&parentElement);
			//根据xml元素加载item
			void loadItemFromElement(scene::GraphicsScene*scene,QDomElement&itemElement);


			//基本属性转为xml元素
			QDomElement TransformProperty(item::GraphicsItem*object);
			QDomElement SizeProperty(item::GraphicsItem* object);
			//加载操作
			void LoadTransformProperty(item::GraphicsItem* object,QDomElement transformNode);
			void LoadSizeProperty(item::GraphicsItem* object,QDomElement sizeNode);
			void LoadSubObjects(scene::GraphicsScene* scene,item::GraphicsItem*parentObject,QDomElement&SubObjectsElement);

		private:
			QDomDocument m_dom;
		};
	}
}