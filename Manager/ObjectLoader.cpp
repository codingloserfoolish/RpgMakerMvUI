#include"ObjectLoader.h"
#include"ObjectsFactory.h"
namespace hjdd
{
	namespace manager
	{
		ObjectLoader::~ObjectLoader()
		{
		}

		void ObjectLoader::loadObjectsName()
		{
			QDir dir("./plugin/dll");
			QFileInfoList dll_list = dir.entryInfoList({ "*.dll" }, QDir::Files);
			for (QFileInfo& info : dll_list)
			{
				QString name = info.fileName().section('.', 0, 0);
				//初始化Library
				Library* lib = new Library;
				lib->_object_lib = new QLibrary(info.filePath());
				lib->_rpgobject_type = -1;

				m_ctrl_tools_info.insert(name, lib);
			}
		}

		int ObjectLoader::loadObjectLib(const QString& name)
		{
			QMap<QString, Library*>::iterator f_it = m_ctrl_tools_info.find(name);
			if (f_it != m_ctrl_tools_info.end())
			{
				if (!((*f_it)->_object_lib->isLoaded()))
				{
					if ((*f_it)->_object_lib->load())
					{
						QLibrary* lib = (*f_it)->_object_lib;
						//导入函数并填充_rpgobject_type
						_Pre_Create create = (_Pre_Create)lib->resolve("create");
						_Pre_RpgObjectType rpgobject_type = (_Pre_RpgObjectType)lib->resolve("rpgobject_type");
						_Pre_JsInitProperties jsInit = (_Pre_JsInitProperties)lib->resolve("Js_initProperties");
						if (create && rpgobject_type)
						{
							int type = rpgobject_type();
							(*f_it)->_rpgobject_type = type;
							ObjectsFactory::instance()->registerProducts(type, create, jsInit);
							m_ctrl_type_mapto_name.insert(type,name);
						}
						else
						{
							lib->unload();
						}
					}
				}
				return (*f_it)->_rpgobject_type;
			}
			return -1;
		}

		void ObjectLoader::unloadAllLib()
		{
			for (QMap<QString, Library*>::iterator it = m_ctrl_tools_info.begin(); it != m_ctrl_tools_info.end(); ++it)
			{
				if ((*it)->_object_lib->isLoaded())
				{
					(*it)->_object_lib->unload();
				}
				delete (*it)->_object_lib;
				delete (*it);
			}
		}

		QString ObjectLoader::typeToName(int type) const
		{
			auto n_it = m_ctrl_type_mapto_name.find(type);
			if (n_it != m_ctrl_type_mapto_name.end())
			{
				return (*n_it);
			}
			return "";
		}

	}
}
