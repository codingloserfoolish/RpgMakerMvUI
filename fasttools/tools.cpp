#include"tools.h"
#include<QPainter>

namespace hjdd
{
	namespace tools
	{
		TextParser::TextParser()
		{
		}

		bool TextParser::isEnd(int index)
		{
			return index < m_text.length() && m_text[index] == ']';
		}

		void TextParser::errorStatus(int nextIndex, TextParser::Error errorType, ParserStatus* status)
		{
			status->_end_index = -1;
			if (errorType == ERROR_UNEXPECTED_CHAR)status->_next_index = nextIndex - 1;
			else status->_next_index = m_text.length();
		}

		void TextParser::makeFontStatus(int index, bool enlarge, ParserStatus* status)
		{
			status->_data = enlarge ? 12 : (-12);
			status->_end_index = index - 1;
			status->_next_index = index;
			status->_type = FONTSIZE;
		}

		void TextParser::parseNumber(int start_index, ParserStatus* status)
		{
			int next_index = start_index;
			while (next_index < m_text.length())
			{
				if (m_text[next_index] >= '0' && m_text[next_index] <= '9')
				{
					++next_index;
				}
				else if (m_text[next_index] == ']')
				{
					status->_data = (m_text.mid(start_index, next_index - start_index)).toInt();
					status->_end_index = start_index - 3;
					status->_next_index = next_index;
					status->_count = 0;
					return;
				}
				else
				{
					this->errorStatus(next_index, ERROR_UNEXPECTED_CHAR, status);
					return;
				}
			}
			this->errorStatus(next_index, ERROR_OUT_BOUNDING, status);
		}

		void TextParser::parseStart(int index, TextParser::Type type, ParserStatus* status)
		{
			if (index >= m_text.length())
			{
				this->errorStatus(index, ERROR_OUT_BOUNDING, status);
				return;
			}
			if (m_text[index] == '\\')
			{
				this->selectType(index + 1, status);
				if (status->_end_index != -1 && status->_type == VARIABLE)
				{
					if (this->isEnd(status->_next_index + 1))
					{
						++(status->_count);
						status->_end_index -= 3;
						++(status->_next_index);
						status->_type = type;
					}
				}
				return;
			}
			status->_type = type;
			this->parseNumber(index, status);
		}

		void TextParser::parsePreStart(int index, TextParser::Type type, ParserStatus* status)
		{
			if (index >= m_text.length())
			{
				this->errorStatus(index, ERROR_OUT_BOUNDING, status);
				return;
			}
			if (m_text[index] != '[')
			{
				this->errorStatus(index, ERROR_UNEXPECTED_CHAR, status);
				return;
			}
			this->parseStart(index + 1, type, status);
		}

		void TextParser::selectType(int index, ParserStatus* status)
		{
			if (index >= m_text.length())
			{
				this->errorStatus(index, ERROR_OUT_BOUNDING, status);
				return;
			}
			QChar symbol = m_text[index];
			if (symbol == 'V')this->parsePreStart(index + 1, VARIABLE, status);
			else if (symbol == 'I')this->parsePreStart(index + 1, ICON, status);
			else if (symbol == 'C')this->parsePreStart(index + 1, COLOR, status);
			else if (symbol == '{') this->makeFontStatus(index, true, status);
			else if (symbol == '}')this->makeFontStatus(index, false, status);
			else this->errorStatus(index, ERROR_UNEXPECTED_CHAR, status);
		}
		//-------------------------
		DynamicPixmap::DynamicPixmap(int width, int height)
		{
			m_preDestroy = 0;
			int t_w = width == 0 ? WIDTH_LOW_SEC_LAYER : width;
			int t_h = height == 0 ? HEIGHT_LOW_FIR_LAYER : height;
			m_canvas = new QPixmap(t_w, t_h);
			m_canvas->fill(Qt::transparent);
		}

		DynamicPixmap::~DynamicPixmap()
		{
			if (m_canvas)delete m_canvas;
		}

		int DynamicPixmap::selectWidth(int width)
		{
			SELECT_LAYER(width,WIDTH);
		}

		int DynamicPixmap::selectHeight(int height)
		{
			SELECT_LAYER(height, HEIGHT);
		}

		bool DynamicPixmap::testAdjustSize(int width, int height)
		{
			int reWidth = m_canvas->width();
			int reHeight = m_canvas->height();
			if (width > reWidth)reWidth = this->selectWidth(width);
			if (height > reHeight)reHeight = this->selectHeight(height);

			if (reWidth != m_canvas->width() || reHeight != m_canvas->height())
			{
				QPixmap* temp = new QPixmap(reWidth, reHeight);
				temp->fill(Qt::transparent);

				QPainter p(temp);
				p.drawPixmap(0, 0, *m_canvas);
				p.end();
				m_preDestroy = m_canvas;

				m_canvas = temp;
				return true;
			}
			return false;
		}

}
}