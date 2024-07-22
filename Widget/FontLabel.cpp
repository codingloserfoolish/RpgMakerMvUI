#include"FontLabel.h"
#include<qpainterpath.h>
#include<QPainter>
//#include<qgraphicseffect.h>
//#include<qdebug.h>
FontLabel::FontLabel(QWidget* parent):
    QLabel(parent),
    m_leading(0),
    m_ascent(0),
    m_font_height(0),
    m_outLinePen(QColor(0,0,0),2),
    m_contentColor(255,255,255),
    m_font("Arial",16),
    m_texts("text")
{
    this->adjust_size();
    this->setAttribute(Qt::WA_TranslucentBackground);
}
FontLabel::FontLabel(const QString& text,QWidget* parent):
    QLabel(parent),
    m_leading(0),
    m_ascent(0),
    m_font_height(0),
    m_outLinePen(QColor(0, 0, 0), 2),
    m_contentColor(255, 255, 255),
    m_font("Arial", 16)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setContentTextWithAdjustSize(text);
}

void FontLabel::setFontWithAdjustSize(QFont& font)
{
    m_font = font;
    this->resize(adjust_size());
}

void FontLabel::setContentTextWithAdjustSize(const QString& text)
{
    m_texts=text.split('\n');
    this->resize(adjust_size());
    //qDebug() << this->size();
}

QSize FontLabel::adjust_size()
{
    if (!m_texts.isEmpty())
    {
        int max_width = 0;
        int transition = 0;
        QFontMetrics metrics(m_font);
        m_leading = metrics.leading();
        m_ascent = metrics.ascent();
        m_font_height = metrics.height();
        int max_height = m_font_height;
        for (QString& str : m_texts)
        {
            transition = metrics.width(str);
            if (transition > max_width)max_width = transition;
            max_height += (m_leading + m_font_height);
        }
        max_height -= (m_leading + m_font_height);
        return QSize(max_width, max_height);
    }
    return this->size();
}

void FontLabel::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);

    QPainterPath path;
    
    for (int i = 0; i < m_texts.size(); ++i)
    {
        path.addText(0,(m_leading+m_font_height)*i+m_ascent, m_font,m_texts[i]);
    }
    

    painter.strokePath(path, m_outLinePen);
    painter.fillPath(path, QBrush(m_contentColor));

    QWidget::paintEvent(event);
}
