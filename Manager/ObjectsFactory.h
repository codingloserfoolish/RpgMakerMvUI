#pragma once
#include"SingleModel.hpp"
#include"../item/GraphicsItem.h"
#include<QMap>
namespace hjdd
{
	namespace manager
	{
		class ObjectsFactory:public SingleModel<ObjectsFactory>
		{
			typedef item::GraphicsItem* (*_Pre_Create)();
			typedef QString(*_Pre_JsInitProperties)(QDomElement&,QString);

			struct DLLExportMethod
			{
				_Pre_Create _create;
				_Pre_JsInitProperties _js_init;
				DLLExportMethod(_Pre_Create create,_Pre_JsInitProperties init):_create(create),_js_init(init){}
			};
		public:
			virtual~ObjectsFactory();
			void registerProducts(int type, _Pre_Create create, _Pre_JsInitProperties init);

			item::GraphicsItem* create(int type);
			QString Js_InitProperties(int type, QDomElement& element,QString name);
		private:
			ObjectsFactory();

			QMap<int, DLLExportMethod>m_producer;
			friend class SingleModel<ObjectsFactory>;
		};
	}
}