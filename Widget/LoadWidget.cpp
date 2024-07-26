#include"LoadWidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<qlistwidget.h>
#include<qpushbutton.h>
#include<qfile.h>
#include<qtextstream.h>
#include<qfileinfo.h>
#include<qmessagebox.h>
#include"../Manager/PathManager.h"
HistoryListWidget::HistoryListWidget(QWidget* parent):QTableView(parent)
{
	this->setFixedWidth(540);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_model = new QStandardItemModel(this);
	this->setModel(m_model);
	m_model->setHorizontalHeaderLabels({ "scene","path","time"});
	this->init("./history/history.txt");
}

void HistoryListWidget::init(const QString& filename)
{
	int row = 0;
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);
	QStandardItem* item = 0;
	while (!stream.atEnd())
	{
		for (int i = 0; i < 3; ++i)
		{
			m_model->setItem(row, i, new QStandardItem(stream.readLine()));
		}
		row++;
	}
	file.close();
}

QString HistoryListWidget::currentSceneName()const
{
	return m_model->item(this->currentIndex().row(), 0)->text();
}

QString HistoryListWidget::currentPath()const
{
	if (!this->currentIndex().isValid())return "";
	return m_model->item(this->currentIndex().row(), 1)->text();
}

void HistoryListWidget::removeCurrentRow()
{
	m_model->removeRow(this->currentIndex().row(), this->currentIndex().parent());
}


LoadWidget::LoadWidget(QWidget* parent):QDialog(parent)
{
	QVBoxLayout* main_layer = new QVBoxLayout(this);

	m_listview = new HistoryListWidget(this);
	main_layer->addWidget(m_listview);

	QHBoxLayout* b_layer = new QHBoxLayout;
	main_layer->addLayout(b_layer);
	QPushButton* ensure_btn = new QPushButton("ensure", this);
	QPushButton* cancel_btn = new QPushButton("cancel", this);
	b_layer->addWidget(ensure_btn);
	b_layer->addWidget(cancel_btn);
	connect(ensure_btn, &QPushButton::clicked, this, &LoadWidget::slot_ensure_clicked);
	connect(cancel_btn, &QPushButton::clicked, this, &LoadWidget::slot_cancel_clicked);
}

void LoadWidget::slot_ensure_clicked()
{
	QString path = this->m_listview->currentPath();
	if (path.isEmpty())return;
	path = path.left(path.lastIndexOf('/') + 1);
	PathManager::instance()->open(path);
	PathManager::instance()->setSceneName(this->m_listview->currentSceneName());

	QFileInfo file_check(PathManager::instance()->save_path() + PathManager::instance()->sceneName() +".xml");
	if (!file_check.exists())
	{
		QMessageBox::information(this, "warning", "file is not existed!");
		HistoryList history("./history/history.txt");
		history.remove(PathManager::instance()->sceneName(), path);
		this->m_listview->removeCurrentRow();
		return;
	}
	this->accept();
}
void LoadWidget::slot_cancel_clicked()
{
	this->reject();
}

