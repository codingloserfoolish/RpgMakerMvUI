#ifndef LINEEDITORENTERWIDGET_H_
#define LINEEDITORENTERWIDGET_H_
#include<qwidget.h>
#include<qlineedit.h>
#include<qpushbutton.h>
class LineEditorEnterWidget :public QWidget
{
	Q_OBJECT
public:
	LineEditorEnterWidget(QWidget*parent=nullptr);
	void setValue(const QString&);
public slots:
	void slot_sendString();
signals:
	void valueChanged(const QString&);
private:
	QLineEdit* m_textedit;
	QPushButton* m_enter_btn;
};
#endif // !LINEEDITORENTERWIDGET_H_
