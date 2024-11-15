#pragma once
#include"SingleModel.hpp"
#include<QLibrary>
#include<QDir>
#include<QMap>
#include"../item/GraphicsItem.h"

namespace hjdd
{
	namespace manager
	{
		struct Library
		{
			QLibrary* _object_lib;
			int _rpgobject_type;
		};
		class ObjectLoader :public SingleModel<ObjectLoader>
		{
			//Item的两个导出函数
			typedef item::GraphicsItem*(*_Pre_Create)();
			typedef int(*_Pre_RpgObjectType)();
			typedef QString(*_Pre_JsInitProperties)(QDomElement&,QString);

			friend class SingleModel<ObjectLoader>;
		public:
			virtual~ObjectLoader();
			void loadObjectsName();
			/*
			* 输入要导入控件的名字
			* 返回控件的typeid
			*/
			int loadObjectLib(const QString& name);
			/*
			* 卸载所有被加载的库
			*/
			void unloadAllLib();

			QString typeToName(int type)const;

			QMap<QString, Library*>& libinfo() { return m_ctrl_tools_info; }
		private:
			ObjectLoader(){}
			QMap<int, QString> m_ctrl_type_mapto_name;
			QMap<QString, Library*> m_ctrl_tools_info;
		};
	}
}