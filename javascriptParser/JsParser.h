#pragma once
#include<QString>
#include<QDomDocument>
#include<QFile>
#include<QTextStream>

/*
* xml解析器那边的问题
* 保存和读取文件指定的文件名没有处理
*/
namespace hjdd
{
	namespace js
	{
		class JsParser
		{
		public:
			JsParser() {};
			~JsParser() {};
			/*
			* xmlsrc界面存档的文件夹
			* jsdes生成路径文件夹
			* 
			*/
			void generateJsFromXml(const QString&xmlsrc,const QString&jsdes,const QString&SceneName);

			void appendScene(QTextStream&stream,const QDomElement root);
			//new Type()
			void appendObject(QDomElement& object,const QString&parent,QTextStream& stream);
			//moveSelf rotation
			void appendTransform(QDomElement& transform,const QString&name ,QTextStream& stream);
			//setSize
			void appendSize(QDomElement& size, const QString& name, QTextStream& stream);
			//children
			void appendSubObject(QDomElement& objects, const QString& parent, QTextStream& stream);

		private:
			QDomDocument m_dom;
		};
	}
}