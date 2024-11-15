#include"ElementLoader.h"
#include"ElementsFactory.h"

namespace hjdd
{
	namespace manager
	{
		ElementLibraryLoader::~ElementLibraryLoader()
		{
		}

		void ElementLibraryLoader::loadElementsName()
		{
			QDir dir("./plugin/elementdll");
			QFileInfoList dll_list = dir.entryInfoList({ "*.dll" }, QDir::Files);
			for (QFileInfo& info : dll_list)
			{
				QString name = info.fileName().section('.', 0, 0);
				//初始化Library
				ElementLibrary* lib = new ElementLibrary;
				lib->_element_lib = new QLibrary(info.filePath());
				lib->_element_type = -1;

				m_ctrl_tools_info.insert(name, lib);
			}
		}

		int ElementLibraryLoader::loadElementLib(const QString& name)
		{
			QMap<QString, ElementLibrary*>::iterator f_it = m_ctrl_tools_info.find(name);
			if (f_it != m_ctrl_tools_info.end())
			{
				if (!((*f_it)->_element_lib->isLoaded()))
				{
					if ((*f_it)->_element_lib->load())
					{
						QLibrary* lib = (*f_it)->_element_lib;
						//导入函数并填充_rpgobject_type
						_Pre_CreateElement create = (_Pre_CreateElement)lib->resolve("create");
						_Pre_ElementType element_type = (_Pre_ElementType)lib->resolve("element_type");
						_Pre_JsRestParams jsParams = (_Pre_JsRestParams)lib->resolve("Js_RestParams");
						if (create && element_type)
						{
							int type = element_type();
							(*f_it)->_element_type = type;
							ElementsFactory::instance()->registerProducts(type, create, jsParams);
							m_ctrl_type_mapto_name.insert(type,name);
						}
						else
						{
							lib->unload();
						}
					}
				}
				return (*f_it)->_element_type;
			}
			return -1;
		}

		void ElementLibraryLoader::unloadAllLib()
		{
			for (QMap<QString, ElementLibrary*>::iterator it = m_ctrl_tools_info.begin(); it != m_ctrl_tools_info.end(); ++it)
			{
				if ((*it)->_element_lib->isLoaded())
				{
					(*it)->_element_lib->unload();
				}
				delete (*it)->_element_lib;
				delete (*it);
			}
		}

		QString ElementLibraryLoader::typeToName(int type) const
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
