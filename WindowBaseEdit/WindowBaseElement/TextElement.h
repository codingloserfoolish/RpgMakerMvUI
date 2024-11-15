#pragma once
#include"IWinBaseElement.h"
#include"../../fasttools/tools.h"
#include"../../manager/ImageManager.h"
#include<QPlainTextEdit>
#ifdef DLLTEXTELEMENTEXPORT
#define TEXTELEMENTEXPORT Q_DECL_EXPORT
#else
#define TEXTELEMENTEXPORT Q_DECL_IMPORT
#endif // DLLTEXTELEMENTEXPORT
#define TEXT_ELEMENT_TYPE 0x0003

namespace hjdd
{
	namespace elements
	{
		class TextElement;
		class TEXTELEMENTEXPORT TextElementEdit :public ElementEdit
		{
			Q_OBJECT
		public:
			TextElementEdit(QWidget* parent = nullptr);
			virtual~TextElementEdit();
		public slots:
			void slot_changeText();
		protected:
			virtual void setProperties(Element* ele);
			virtual void OthersConnect(Element*ele);
			virtual void OthersDisConnect(Element*ele);
		private:
			QPlainTextEdit* m_textedit;
		};
		//[x,y,text]
		class TEXTELEMENTEXPORT TextElement :public Element
		{
		public:
			TextElement();
			virtual~TextElement();

			//编辑窗口弹出
			virtual ElementEdit* editor() { return new TextElementEdit; }
			virtual int element_type()const { return TEXT_ELEMENT_TYPE; }
			//xml-Js输出
			virtual QDomElement xmlSave(QDomDocument& dom);
			virtual void xmlLoad(QDomElement& selfPrototype);

			QString& text() { return m_parser.text(); }
			//只把文字刷到DynamicCanvas上
			void setText(const QString& text);
			void flashToCanvas(const QString& text);

			void testExchangeDC(int width, int height, QPainter& p);

			QColor textColor(int color);
			void drawText(int x, int y, QString& text, QPainter& painter);
			void drawIcon(int x, int y, int index, QPainter& painter);
			void resizeFontSize(int data, QPainter& painter);
		protected:
			virtual void drawBackground(QPainter* painter);
		private:

			tools::TextParser m_parser;

			QPen m_outlineStyle;

			tools::DynamicPixmap m_textCanvas;

			int m_maxWidth;
			int m_maxHeight;

			static QImage _text_color;
			static manager::Pixmap* _iconset;
		};

		extern "C" TEXTELEMENTEXPORT Element * create();
		extern "C" TEXTELEMENTEXPORT int element_type();
		extern "C" TEXTELEMENTEXPORT QString Js_RestParams(QDomElement & properties);

	}
}