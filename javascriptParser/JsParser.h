#pragma once
#include<QString>
#include<QDomDocument>
#include<QFile>
#include<QTextStream>

/*
* xml�������Ǳߵ�����
* ����Ͷ�ȡ�ļ�ָ�����ļ���û�д���
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
			* xmlsrc����浵���ļ���
			* jsdes����·���ļ���
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