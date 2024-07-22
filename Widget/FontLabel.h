#ifndef FONTLABEL_H_
#define FONTLABEL_H_
#include<qlabel.h>
#include<qpen.h>

enum JoinType
{
    MITER=Qt::MiterJoin,
    BEVEL=Qt::BevelJoin,
    ROUND=Qt::RoundJoin
};


class FontLabel :public QLabel
{
	Q_OBJECT
public:
    FontLabel(QWidget* parent = nullptr);
	FontLabel(const QString&text,QWidget* parent = nullptr);

    void setOutLineJoin(JoinType type) { m_outLinePen.setMiterLimit(type); }
    void setOutLineColor(const QColor& color) { m_outLinePen.setColor(color); }
    void setOutLineWidth(int width) { m_outLinePen.setWidth(width); };
    void setContentColor(QColor color) { m_contentColor = color;}
    void setFontWithAdjustSize(QFont&font);
    void setContentTextWithAdjustSize(const QString&text);
    QSize adjust_size();

    QPen& outline() { return m_outLinePen; }
    QColor& contentColor() { return m_contentColor; }
    QFont& font() { return m_font; }
protected:
	void paintEvent(QPaintEvent*)override;

    int m_ascent;
    int m_leading;
    int m_font_height;
    QPen m_outLinePen;
    QColor m_contentColor;
    QFont m_font;
    QStringList m_texts;
};

#endif // !FONTLABEL_H_
