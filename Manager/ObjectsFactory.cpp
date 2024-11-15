#include"ObjectsFactory.h"
namespace hjdd
{
	namespace manager
	{
		ObjectsFactory::~ObjectsFactory()
		{
		}
		void ObjectsFactory::registerProducts(int type, _Pre_Create create, _Pre_JsInitProperties init)
		{
			if (m_producer.find(type) == m_producer.end())
			{
				m_producer.insert(type, DLLExportMethod(create,init));
			}
		}
		item::GraphicsItem* ObjectsFactory::create(int type)
		{
			QMap<int, DLLExportMethod>::iterator it_create = m_producer.find(type);
			if (it_create != m_producer.end())
			{
				if((*it_create)._create)return (*it_create)._create();
			}
			return 0;
		}
		QString ObjectsFactory::Js_InitProperties(int type, QDomElement& element,QString name)
		{
			QMap<int, DLLExportMethod>::iterator it_init = m_producer.find(type);
			if (it_init != m_producer.end())
			{
				if((*it_init)._js_init)return (*it_init)._js_init(element, name);
			}
			return "";
		}
		ObjectsFactory::ObjectsFactory()
		{
		}
	}
}