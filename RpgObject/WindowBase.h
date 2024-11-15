#pragma once
#include"../item/GraphicsItem.h"
#include"../manager/ImageManager.h"
#include"../WindowBaseEdit/WindowBaseElement/IWinBaseElement.h"
#include"WindowBaseEditor.h"
#include<vector>
#ifdef DLL_WINDOWBASE_EXPORT
#define WINDOWBASE_EXPORT Q_DECL_EXPORT
#else
#define WINDOWBASE_EXPORT Q_DECL_IMPORT
#endif // RPGOBJECT_EXPORT

namespace hjdd
{
	namespace item
	{
		class WINDOWBASE_EXPORT WindowBase :public GraphicsItem
		{
		public:
			WindowBase(QGraphicsItem* parent = nullptr);
			WindowBase(int width,int height,QGraphicsItem* parent = nullptr);
			virtual~WindowBase();

			virtual int rpgobject_type()const { return RPGOBJECT_WINDOWBASE; }
			virtual widget::ObjectEditor*editor(){return new widget::WindowBaseEditor;}

			/*
			* <Elements>
			*	<Element dllname=''>
			*		<x></x>
			*		<y></y>
			*		<自定义></自定义>
			*	</Element>
			* 
			* </Elements>
			*/
			virtual QDomElement OthersProperties(QDomDocument& dom);
			virtual void OthersPropertiesLoad(QDomElement& node);

			std::vector<elements::Element*>& elements() { return m_elements; }
			void refreshElementsCanvas();
		protected:
			virtual void drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option);
		private:

			

			QPixmap*m_elements_canvas;
			static manager::Pixmap* _winskin;
			std::vector<elements::Element*>m_elements;
		};



		extern "C" WINDOWBASE_EXPORT GraphicsItem * create();
		extern "C" WINDOWBASE_EXPORT int rpgobject_type();
		extern "C" WINDOWBASE_EXPORT QString Js_initProperties(QDomElement & properties, QString objectName);
	}
}