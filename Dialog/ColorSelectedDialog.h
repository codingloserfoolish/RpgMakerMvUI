#ifndef COLORSELECTEDDIALOG_H_
#define COLORSELECTEDDIALOG_H_
#include<qdialog.h>
#include"../Manager/PixmapManager.h"
//Ò»¸ñ48
class FontColorWidget :public QWidget
{
	Q_OBJECT
public:
	FontColorWidget(QWidget* parent = nullptr);
	int colorIndex()const { return m_selectedColor; }
protected:
	virtual void paintEvent(QPaintEvent* e)override;
	virtual void mousePressEvent(QMouseEvent* e)override;
private:
	Pixmap* m_colorPart;
	int m_selectedColor;
};


class ColorSelectedDialog :public QDialog
{
	Q_OBJECT
public:
	ColorSelectedDialog(QWidget* parent = nullptr);
	int colorIndex()const { return m_fontColorGraphics->colorIndex(); }
	static int GetColorIndex();
private:
	FontColorWidget* m_fontColorGraphics;
};
#endif // !COLORSELECTEDDIALOG_H_
