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
			//Item��������������
			typedef item::GraphicsItem*(*_Pre_Create)();
			typedef int(*_Pre_RpgObjectType)();
			typedef QString(*_Pre_JsInitProperties)(QDomElement&,QString);

			friend class SingleModel<ObjectLoader>;
		public:
			virtual~ObjectLoader();
			void loadObjectsName();
			/*
			* ����Ҫ����ؼ�������
			* ���ؿؼ���typeid
			*/
			int loadObjectLib(const QString& name);
			/*
			* ж�����б����صĿ�
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