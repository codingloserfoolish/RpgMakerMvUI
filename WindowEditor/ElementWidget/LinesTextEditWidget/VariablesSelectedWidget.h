#ifndef VARIABLESSELECTEDWIDGET_H_
#define VARIABLESSELECTEDWIDGET_H_
#include<qdialog.h>
#include<qtableview.h>
#include<qstandarditemmodel.h>
#include"../../../Manager/PathManager.h"
class VariablesSelection:public QTableView
{
	Q_OBJECT
public:
	VariablesSelection(QWidget* parent = nullptr);
	void addItem(int id,QString& name);
	QString& current_id();
private:
	QStandardItemModel* m_model;
};

class VariablesSelectedWidget:public QDialog
{
	Q_OBJECT
public:
	VariablesSelectedWidget(QWidget* parent = nullptr);
	void searchVariables();

	static QString GetVariables();
public slots:
	void slot_ensureVariables();
private:
	void breakArray(QTextStream& stream);
	VariablesSelection* m_selections;
	static QString _id;
};

#endif // !VARIABLESSELECTEDWIDGET_H_
