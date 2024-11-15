#pragma once
#include"../../manager/SingleModel.hpp"
#include"../WindowBaseElement/IWinBaseElement.h"
#include<QMap>

#ifdef DLL_ELEMENT_MANAGER_EXPORT
#define ELEMENT_MANAGER_EXPORT Q_DECL_EXPORT
#else
#define ELEMENT_MANAGER_EXPORT Q_DECL_IMPORT
#endif // DLL_ELEMENT_MANAGER_EXPORT

namespace hjdd
{
	namespace manager
	{
		class ELEMENT_MANAGER_EXPORT ElementsFactory:public SingleModel<ElementsFactory>
		{
			typedef elements::Element* (*_Pre_CreateElement)();
			typedef QString(*_Pre_JsRestParams)(QDomElement&);

			struct DLLExportMethod
			{
				_Pre_CreateElement _create;
				_Pre_JsRestParams _js_init;
				DLLExportMethod(_Pre_CreateElement create, _Pre_JsRestParams params):_create(create),_js_init(params){}
			};
		public:
			virtual~ElementsFactory();
			void registerProducts(int type, _Pre_CreateElement create, _Pre_JsRestParams init);

			elements::Element* create(int type);
			QString Js_RestParams(int type, QDomElement& element);
		private:
			ElementsFactory();

			QMap<int, DLLExportMethod>m_producer;
			friend class SingleModel<ElementsFactory>;
		};
	}
}