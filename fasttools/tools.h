#pragma once
#pragma once
#include"config.h"
#include<QString>
#include<QPixmap>
#define WIDTH_MAX_LAYER		800
#define WIDTH_MID_LAYER		736
#define WIDTH_LOW_LAYER		608
#define WIDTH_LOW_SEC_LAYER 416
#define WIDTH_LOW_FIR_LAYER 160

#define HEIGHT_MAX_LAYER 600
#define HEIGHT_MID_LAYER 520
#define HEIGHT_LOW_LAYER 400
#define HEIGHT_LOW_SEC_LAYER 280
#define HEIGHT_LOW_FIR_LAYER 120


#define SELECT_LAYER(param,which)\
if(param<which##_LOW_FIR_LAYER)return which##_LOW_FIR_LAYER;\
else if (param < which##_LOW_SEC_LAYER)return which##_LOW_SEC_LAYER;\
else if (param < which##_LOW_LAYER)return which##_LOW_LAYER;\
else if (param < which##_MID_LAYER)return which##_MID_LAYER;\
else return which##_MAX_LAYER;


namespace hjdd
{
	namespace tools
	{
		class EXPORT TextParser
		{
		public:
			enum Type
			{
				VARIABLE,
				COLOR,
				ICON,
				FONTSIZE
			};
			enum Error
			{
				ERROR_OUT_BOUNDING,
				ERROR_UNEXPECTED_CHAR
			};
			struct ParserStatus
			{
				int _data;
				int _end_index;
				int _next_index;
				int _count;//是否是直接数据
				TextParser::Type _type;
			};
		public:
			TextParser();
			void setText(const QString& text) { m_text = text; }
			bool equal(const QString& text) { return text == m_text; }
			QString& text() { return m_text; }
			bool isEnd(int index);
			void errorStatus(int nextIndex, Error errorType, ParserStatus* status);
			void makeFontStatus(int index, bool enlarge, ParserStatus* status);
			//123456]
			void parseNumber(int start_index, ParserStatus* status);
			//检查'\'
			void parseStart(int index, Type type, ParserStatus* status);
			//检查'[
			void parsePreStart(int index, Type type, ParserStatus* status);
			//检查V,I,C
			void selectType(int index, ParserStatus* status);
		private:
			QString m_text;
		};
		//可变图片
		class EXPORT DynamicPixmap
		{
		public:
			DynamicPixmap(int width=0, int height=0);
			~DynamicPixmap();
			QPixmap* pixmap()const { return m_canvas; }
			int width()const { return m_canvas->width(); }
			int height()const { return m_canvas->height(); }

			void clear() { if (m_canvas)m_canvas->fill(Qt::transparent); }

			int selectWidth(int width);
			int selectHeight(int height);

			bool testAdjustSize(int width, int height);
			void testDestroyOld() { if (m_preDestroy) { delete m_preDestroy; m_preDestroy = 0; } };
		private:
			QPixmap* m_preDestroy;
			QPixmap* m_canvas;
		};
	}
}