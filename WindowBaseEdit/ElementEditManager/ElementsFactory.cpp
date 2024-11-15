#include"ElementsFactory.h"
namespace hjdd
{
	namespace manager
	{
		ElementsFactory::~ElementsFactory()
		{
		}
		void ElementsFactory::registerProducts(int type, _Pre_CreateElement create, _Pre_JsRestParams init)
		{
			if (m_producer.find(type) == m_producer.end())
			{
				m_producer.insert(type, DLLExportMethod(create,init));
			}
		}
		elements::Element* ElementsFactory::create(int type)
		{
			QMap<int, DLLExportMethod>::iterator it_create = m_producer.find(type);
			if (it_create != m_producer.end())
			{
				if((*it_create)._create)return (*it_create)._create();
			}
			return 0;
		}
		QString ElementsFactory::Js_RestParams(int type, QDomElement& element)
		{
			QMap<int, DLLExportMethod>::iterator it_params = m_producer.find(type);
			if (it_params != m_producer.end())
			{
				if((*it_params)._js_init)return (*it_params)._js_init(element);
			}
			return "";
		}
		ElementsFactory::ElementsFactory()
		{
		}
	}
}