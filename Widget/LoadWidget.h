#ifndef LOADWIDGET_H_
#define LOADWIDGET_H_
#include<qdialog.h>
#include<qtableview.h>
#include<qstandarditemmodel.h>
class HistoryListWidget :public QTableView
{
	Q_OBJECT
public:
	HistoryListWidget(QWidget* parent = nullptr);
	void init(const QString& filename);
	QString currentSceneName()const;
	QString currentPath()const;
	void removeCurrentRow();
private:
	QStandardItemModel* m_model;
};


class LoadWidget :public QDialog
{
	Q_OBJECT
public:
	LoadWidget(QWidget* parent = nullptr);
public slots:
	void slot_ensure_clicked();
	void slot_cancel_clicked();
protected:
	HistoryListWidget* m_listview;
};
#endif // !LOADWIDGET_H_
