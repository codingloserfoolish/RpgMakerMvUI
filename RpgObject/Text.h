#pragma once
#include<QImage>
#include<QPainterPath>
#include"../item/GraphicsItem.h"
#include"../manager/ImageManager.h"
#include"TextEditor.h"
#include"../fasttools/tools.h"

#ifdef DLL_TEXT_EXPORT
#define TEXT_EXPORT Q_DECL_EXPORT
#else
#define TEXT_EXPORT Q_DECL_IMPORT
#endif // DLL_TEXT_EXPORT

namespace hjdd
{
	namespace item
	{
		class TEXT_EXPORT Text :public GraphicsItem
		{
		public:
			Text(QGraphicsItem* parent = nullptr);
			virtual~Text();
			
			virtual int rpgobject_type()const { return RPGOBJECT_TEXT; };
			virtual widget::ObjectEditor* editor() { return new widget::TextEditor; }
			/*
			* xml
			* <TextList>
			*	<Text></Text>
			*	...
			* </TextList>
			*/
			virtual QDomElement OthersProperties(QDomDocument& dom);
			virtual void OthersPropertiesLoad(QDomElement& node);

			
			QString& text() { return m_parser.text(); }
			void setText(const QString& text);
			void flashToCanvas(const QString&text);

			void testExchangeDC(int width,int height,QPainter&p);

			QColor textColor(int color);
			void drawText(int x,int y,QString&text,QPainter&painter);
			void drawIcon(int x, int y, int index, QPainter& painter);
			void resizeFontSize(int data,QPainter&painter);
		protected:
			virtual void drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option);
		private:
			
			tools::TextParser m_parser;
			
			QPen m_outlineStyle;

			tools::DynamicPixmap m_textCanvas;

			int m_maxWidth;
			int m_maxHeight;

			static QImage _text_color;
			static manager::Pixmap* _iconset;
		};

		extern "C" TEXT_EXPORT GraphicsItem * create();
		extern "C" TEXT_EXPORT int rpgobject_type();
		extern "C" TEXT_EXPORT QString Js_initProperties(QDomElement & properties, QString objectName);
	}
}
