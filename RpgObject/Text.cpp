#include"Text.h"
#include<QPainterPath>
#include"../manager/ConfigInfo.h"
namespace hjdd
{
	namespace item
	{
		QImage Text::_text_color;
		manager::Pixmap* Text::_iconset=0;
		Text::Text(QGraphicsItem* parent)
			:GraphicsItem(410,120,parent),
			m_outlineStyle(QColor(0,0,0,127),4)
		{
			if (!_iconset)
			{
				manager::Pixmap* winskin = LOAD_IMAGE("system/Window.png");
				_text_color = winskin->_pixmap.toImage().copy(96, 144, 96, 48);
				RELEASE_IMAGE(winskin);
				_iconset = LOAD_IMAGE("system/IconSet.png");
			}
			this->setText("rpgtext");
		}

		Text::~Text()
		{
			if (_iconset && _iconset->_count)
			{
				RELEASE_IMAGE(_iconset);
			}
		}

		QDomElement Text::OthersProperties(QDomDocument& dom)
		{
			QDomElement textNode = dom.createElement("TextList");
			QStringList textList = m_parser.text().split('\n');
			for (QString& str : textList)
			{
				QDomElement text = dom.createElement("Text");
				str.replace('\\', "\\\\");
				text.appendChild(dom.createTextNode(str));
				textNode.appendChild(text);
			}
			return textNode;
		}

		void Text::OthersPropertiesLoad(QDomElement& node)
		{
			QDomElement text = node.firstChildElement();
			QStringList textList;
			QString str;
			while (!text.isNull())
			{
				str = text.firstChild().nodeValue();
				str.replace("\\\\", "\\");
				textList.append(str);
				text = text.nextSiblingElement();
			}
			this->setText(textList.join('\n'));
		}

		void Text::setText(const QString& text)
		{
			if (!m_parser.equal(text))
			{
				m_parser.setText(text);
				this->flashToCanvas(text);
			}
			this->update();
			this->setWidth(this->m_maxWidth);
			this->setHeight(this->m_maxHeight);
		}

		void Text::flashToCanvas(const QString& text)
		{
			m_textCanvas.clear();
			tools::TextParser::ParserStatus status{0,0,0,0,tools::TextParser::VARIABLE};
			int beginSlice = 0;
			int maxWidth = 0;
			int maxLineHeight = 0;
			QFont gameFont(manager::ConfigInfo::instance()->fontFamily());
			gameFont.setPixelSize(28);

			int s_x = 0, s_y = 0;

			QPainter painter(m_textCanvas.pixmap());
			painter.setFont(gameFont);
			painter.setPen(QPen(Qt::white));

			maxLineHeight = painter.fontMetrics().height();

			for (int i = 0; i < text.length(); ++i)
			{
				if (text[i] == '\\')
				{
					m_parser.selectType(i + 1, &status);
					i = status._next_index;
					if (status._end_index != -1)
					{
						if (beginSlice != status._end_index)
						{
							QString wouldToDraw = text.mid(beginSlice, status._end_index - beginSlice);
							int preWidth = s_x + painter.fontMetrics().width(wouldToDraw);
							this->testExchangeDC(preWidth, s_y + maxLineHeight, painter);
							this->drawText(s_x, s_y, wouldToDraw, painter);
							s_x = preWidth;
						}
						beginSlice = i + 1;
						switch (status._type)
						{
						case tools::TextParser::VARIABLE:
						{
							this->testExchangeDC(s_x+32, s_y + maxLineHeight, painter);
							painter.drawPixmap(s_x,s_y,IV_ICON,0,0,32,32);
							s_x += 32;
						}break;
						case tools::TextParser::COLOR:
						{
							if (status._count == 0)
							{
								QPen pen = painter.pen();
								pen.setColor(this->textColor(status._data));
								painter.setPen(pen);
							}
						}break;
						case tools::TextParser::ICON:
						{
							this->testExchangeDC(s_x + 32, s_y + maxLineHeight, painter);
							if (status._count == 0)
							{
								this->drawIcon(s_x, s_y, status._data, painter);
							}
							else
							{
								painter.drawPixmap(s_x, s_y, IV_ICON, 32, 0, 32, 32);
							}
							s_x += 32;
						}break;
						case tools::TextParser::FONTSIZE:
						{
							this->resizeFontSize(status._data, painter);
							if (maxLineHeight < painter.fontMetrics().height())
							{
								maxLineHeight = painter.fontMetrics().height();
							}
						}break;
						default:
							break;
						}
					}
				}
				else if(text[i]=='\n')
				{
					if (beginSlice != i)
					{
						QString wouldToDraw = text.mid(beginSlice, i - beginSlice);
						int preWidth = s_x + painter.fontMetrics().width(wouldToDraw);
						this->testExchangeDC(preWidth, s_y +2*maxLineHeight, painter);
						this->drawText(s_x, s_y, wouldToDraw, painter);
						s_x = preWidth;
						beginSlice = i + 1;
					}
					if (maxWidth < s_x)maxWidth = s_x;
					s_x = 0;
					s_y += maxLineHeight;
					maxLineHeight = painter.fontMetrics().height();
				}
			}
			if (beginSlice != text.length())
			{
				QString wouldToDraw = text.mid(beginSlice, text.length() - beginSlice);
				int preWidth = s_x + painter.fontMetrics().width(wouldToDraw);
				this->testExchangeDC(preWidth, s_y + maxLineHeight, painter);
				this->drawText(s_x, s_y, wouldToDraw, painter);
				s_x = preWidth;
			}
			if (maxWidth < s_x)maxWidth = s_x;
			painter.end();
			this->m_maxWidth = maxWidth;
			this->m_maxHeight = (s_y + maxLineHeight);
		}

		void Text::testExchangeDC(int width, int height, QPainter& p)
		{
			if (m_textCanvas.testAdjustSize(width, height))
			{
				QFont font = p.font();
				QPen pen = p.pen();
				p.end();
				m_textCanvas.testDestroyOld();
				p.begin(m_textCanvas.pixmap());
				p.setFont(font);
				p.setPen(pen);
			}
		}

		QColor Text::textColor(int color)
		{
			if (color >= 32)return Qt::black;
			return _text_color.pixel((color % 8) * 12 + 6, (color / 8) * 12 + 6);
		}

		void Text::drawText(int x, int y, QString& text, QPainter& painter)
		{
			QPainterPath path;
			path.addText(x, y+painter.fontMetrics().ascent(), painter.font(), text);
			painter.strokePath(path, m_outlineStyle);
			painter.drawPath(path);
			painter.fillPath(path,painter.pen().color());
		}

		void Text::drawIcon(int x, int y, int index, QPainter& painter)
		{
			int s_x = (index % 16) * 32;
			int s_y = (index / 16) * 32;
			painter.drawPixmap(x,y,Text::_iconset->_pixmap,s_x,s_y,32,32);
		}

		void Text::resizeFontSize(int data, QPainter& painter)
		{
			int pixelSize = painter.font().pixelSize() + data;
			if (pixelSize >= 24 && pixelSize <= 96)
			{
				QFont gameFont = painter.font();
				gameFont.setPixelSize(pixelSize);
				painter.setFont(gameFont);
			}
		}

		void Text::drawBackground(QPainter* painter, const QStyleOptionGraphicsItem* option)
		{
			painter->drawPixmap(-this->width()/2,-this->height()/2, *(m_textCanvas.pixmap()), 0, 0, this->width(), this->height());
		}

		GraphicsItem* create()
		{
			return new Text();
		}
		int rpgobject_type()
		{
			return RPGOBJECT_TEXT;
		}

		QString Js_initProperties(QDomElement& properties,QString objectName)
		{
			QDomElement next = properties.firstChildElement();
			QStringList textlist;
			while (!next.isNull())
			{
				textlist.push_back(next.firstChild().nodeValue());
				next = next.nextSiblingElement();
			}
			QString js_insert = QString("%1.setText(\"%2\");\n%1.displayLinesText();\n").arg(objectName).arg(textlist.join("\\n"));

			return js_insert;
		}
	}
}