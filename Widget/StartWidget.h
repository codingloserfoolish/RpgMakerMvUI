#ifndef STARTWIDGET_H_
#define STARTWIDGET_H_
#include<QDialog.h>
#include<qlineedit.h>
class StartWidget :public QDialog
{
	Q_OBJECT
public:
	StartWidget(QWidget* parent = nullptr);
public slots:
	void slot_popSearchDlg();
	void slot_ensure();
	void slot_cancel();
private:
	QLineEdit* m_sceneName;
	QLineEdit* m_projectName;
};
#endif // !STARTWIDGET_H_
