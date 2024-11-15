#include"JsParser.h"
#include"../manager/ConfigInfo.h"
#include"../manager/ObjectsFactory.h"
#include"../manager/ObjectLoader.h"
namespace hjdd
{
	namespace js
	{
		void JsParser::generateJsFromXml(const QString& xmlsrc, const QString& jsdes, const QString& SceneName)
		{
			QFile file(xmlsrc +SceneName + ".xml");
			if (!file.open(QIODevice::ReadOnly))
			{
				return;
			}
			if (!m_dom.setContent(&file))
			{
				file.close();
				return;
			}
			file.close();

			QFile jsFile(jsdes + SceneName + ".js");
			if (!jsFile.open(QIODevice::WriteOnly))
			{
				return;
			}
			QTextStream stream(&jsFile);
			//¼Ì³Ð³¡¾°
			QDomElement root = m_dom.documentElement();
			/*
			* function Scene(){this.initialize.apply(this,arguments);}
			* Scene.prototype=Object.create(hjdd.MainScene.prototype);
			* Scene.prototype.constructor=Scene;
			* 
			* Scene.prototype.initialize=function(){hjdd.MainScene.prototype.initialize.call(this);}
			* 
			*/
			this->appendScene(stream,root);
			/*¿Ø¼þÌí¼Ó@override
			* Scene.prototype.setupUi=function(){
			*/
			stream << QString("%1.prototype.setupUi=function(){\n").arg(root.tagName());
			this->appendSubObject(root, "this", stream);
			/* 
			* }
			*/
			stream << "}";

			jsFile.close();
		}
		void JsParser::appendScene(QTextStream& stream, const QDomElement root)
		{
			QString scene = 
				"function %1(){this.initialize.apply(this,arguments);}\n"
				"%1.prototype=Object.create(hjdd.MainScene.prototype);\n"
				"%1.prototype.constructor=%1;\n"
				"%1.prototype.initialize=function(){hjdd.MainScene.prototype.initialize.call(this);}\n";
			stream << scene.arg(root.tagName());
		}
		void JsParser::appendObject(QDomElement& object,const QString& parent, QTextStream& stream)
		{
			QString objectName = "this."+object.attribute("name");
			QString dllname = object.attribute("dllname");
			//this.object=new Type();
			stream << QString("%1 = new hjdd.%2();\n").arg(objectName).arg(dllname);
			//this.xxx.addObject(this.object);
			stream << QString("%1.addObject(%2);\n").arg(parent).arg(objectName);
			
			QDomElement element = object.firstChildElement();
			while (!element.isNull())
			{
				//moveSelf rotation
				if (element.tagName() == "Transform")
				{
					this->appendTransform(element,objectName,stream);
				}
				else if (element.tagName() == "Size")
				{
					this->appendSize(element,objectName,stream);
				}
				else if (element.tagName() == "SubObjects")
				{
					this->appendSubObject(element,objectName,stream);
				}
				else
				{
					int type = manager::ObjectLoader::instance()->loadObjectLib(dllname);
					if (type != -1)
					{
						stream << manager::ObjectsFactory::instance()->Js_InitProperties(type, element, objectName);
					}
				}
				element = element.nextSiblingElement();
			}

		}
		void JsParser::appendTransform(QDomElement& transform, const QString& name, QTextStream& stream)
		{
			QDomElement x = transform.firstChildElement();
			QDomElement y = x.nextSiblingElement();
			QDomElement angle = y.nextSiblingElement();
			qreal rotation = angle.firstChild().nodeValue().toDouble();
			stream << QString(
				"%1.moveSelf(%2,%3);\n"
				"%1.rotation=%4;\n")
				.arg(name)
				.arg(x.firstChild().nodeValue())
				.arg(y.firstChild().nodeValue())
				.arg(QString::number(rotation*pi/180.0f,'f',1));
		}
		void JsParser::appendSize(QDomElement& size, const QString& name, QTextStream& stream)
		{
			QDomElement width = size.firstChildElement();
			QDomElement height = width.nextSiblingElement();
			stream << QString("%1.setSize(%2,%3);\n")
				.arg(name)
				.arg(width.firstChild().nodeValue())
				.arg(height.firstChild().nodeValue());
		}
		void JsParser::appendSubObject(QDomElement& objects, const QString& parent, QTextStream& stream)
		{
			QDomElement child = objects.firstChildElement();
			while (!child.isNull())
			{
				this->appendObject(child, parent, stream);
				child = child.nextSiblingElement();
			}
		}
	}
}