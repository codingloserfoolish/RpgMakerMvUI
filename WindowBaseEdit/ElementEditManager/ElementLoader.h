#pragma once
#include"../../manager/SingleModel.hpp"
#include"../WindowBaseElement/IWinBaseElement.h"
#include<QLibrary>
#include<QDir>
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
		struct ELEMENT_MANAGER_EXPORT ElementLibrary
		{
			QLibrary* _element_lib;
			int _element_type;
		};

		class ELEMENT_MANAGER_EXPORT ElementLibraryLoader :public SingleModel<ElementLibraryLoader>
		{
			
			typedef elements::Element* (*_Pre_CreateElement)();
			typedef int(*_Pre_ElementType)();
			typedef QString(*_Pre_JsRestParams)(QDomElement&);

			friend class SingleModel<ElementLibraryLoader>;
		public:
			virtual~ElementLibraryLoader();
			void loadElementsName();
			/*
			* 输入要导入控件的名字
			* 返回控件的typeid
			*/
			int loadElementLib(const QString& name);
			/*
			* 卸载所有被加载的库
			*/
			void unloadAllLib();

			QString typeToName(int type)const;

			QMap<QString, ElementLibrary*>& libinfo() { return m_ctrl_tools_info; }
		private:
			ElementLibraryLoader() {this->loadElementsName();}
			QMap<int, QString> m_ctrl_type_mapto_name;
			QMap<QString, ElementLibrary*> m_ctrl_tools_info;
		};
	}
}