#pragma once
#include"../../manager/SingleModel.hpp"
#include"../WindowBaseElement/IWinBaseElement.h"
#include<memory>
#include<QMap>
namespace hjdd
{
	namespace manager
	{
		class ElementEditorManager:public QObject,public SingleModel<ElementEditorManager>
		{
			Q_OBJECT
		public:
			ElementEditorManager(const ElementEditorManager&)=delete;
			ElementEditorManager& operator=(const ElementEditorManager&) = delete;
			virtual~ElementEditorManager();

			void show(elements::Element*ele);
			//有断开连接的功能
			void close();
			void clearDeletedElementConnect(elements::Element*ele);
			void destroyAll();
		signals:
			void propertiesChanged();
		public slots:
			void slot_sendpropertiesChanged() { emit this->propertiesChanged(); }
		private:
			ElementEditorManager();
			QMap<int, elements::ElementEdit*> m_editors;
			QMap<int, elements::ElementEdit*>::iterator m_current_editor;

			friend class SingleModel<ElementEditorManager>;
		};
	}
}