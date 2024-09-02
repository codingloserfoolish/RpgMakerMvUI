#ifndef ICONSELECTEDDIALOG_H_
#define ICONSELECTEDDIALOG_H_
#include<qdialog.h>
#include<qpixmap.h>
#include<qlabel.h>
#include"../Manager/PixmapManager.h"
class IconPicture :public QWidget
{
	Q_OBJECT
public:
	IconPicture(QWidget* parent = nullptr);
	void setSystemIcon(const QString& iconSystemName);
	void adjustRectByPoint(QPoint& pos);
signals:
	void selectedChanged(int index);
protected:
	void paintEvent(QPaintEvent*)override;
	void mousePressEvent(QMouseEvent* e)override;
private:
	Pixmap* m_systemIcon;
	QRect m_seletedArea;
};

class IconSelectedDialog :public QDialog
{
	Q_OBJECT
public:
	IconSelectedDialog(QWidget* parent = nullptr);
	static int GetIconId();
public slots:
	void slot_indexChanged(int icon_index);
	void slot_setExecMsg();
private:
	QLabel* m_iconIdLabel;
	static int _send_Id;
};
#endif // !ICONSELECTEDDIALOG_H_
