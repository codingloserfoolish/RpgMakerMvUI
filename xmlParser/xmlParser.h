#pragma once
#include"../scene/GraphicsScene.h"
#include"../item/GraphicsItem.h"
#include<QFile>
#include<QTextStream>

//����QDomTextֻ��ͨ��QDomNode��ȡֵ
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
		* ����˵��
		* ��	object	��׺��ȫ��	item::GraphicsItem*
		* ��	element	��׺��ȫ��	QDomElement
		*/
		class XmlParser
		{
		public:
			XmlParser();
			void saveSceneToXml(const QString&SceneName,scene::GraphicsScene*scene);
			bool loadSceneFromXml(const QString& SceneName, scene::GraphicsScene* scene);

			//����item���xmlԪ��
			void appendElement(item::GraphicsItem*item,QDomElement&parentElement);
			//����xmlԪ�ؼ���item
			void loadItemFromElement(scene::GraphicsScene*scene,QDomElement&itemElement);


			//��������תΪxmlԪ��
			QDomElement TransformProperty(item::GraphicsItem*object);
			QDomElement SizeProperty(item::GraphicsItem* object);
			//���ز���
			void LoadTransformProperty(item::GraphicsItem* object,QDomElement transformNode);
			void LoadSizeProperty(item::GraphicsItem* object,QDomElement sizeNode);
			void LoadSubObjects(scene::GraphicsScene* scene,item::GraphicsItem*parentObject,QDomElement&SubObjectsElement);

		private:
			QDomDocument m_dom;
		};
	}
}