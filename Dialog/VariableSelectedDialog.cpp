#include"VariableSelectedDialog.h"
#include<QVBoxLayout>
#include<qpushbutton.h>
#include<qheaderview.h>
#include<qfile.h>
#include<qtextstream.h>
#include<qtextcodec.h>


QString VariableSelectedDialog::_id = "";

VariablesSelection::VariablesSelection(QWidget* parent) :QTableView(parent)
{
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->verticalHeader()->setVisible(false);
	m_model = new QStandardItemModel(this);
	this->setModel(m_model);
	m_model->setHorizontalHeaderLabels({ "id","variables name" });
}

void VariablesSelection::addItem(int id, QString& name)
{
	int count = m_model->rowCount();
	m_model->setItem(count, 0, new QStandardItem(QString("%1").arg(id)));
	m_model->setItem(count, 1, new QStandardItem(name));
}

QString& VariablesSelection::current_id()
{
	QModelIndex index = this->currentIndex();
	if (index.isValid())
	{
		return m_model->item(index.row(), 0)->text();
	}
	return QString();
}

VariableSelectedDialog::VariableSelectedDialog(QWidget* parent) :QDialog(parent)
{
	this->setWindowTitle("select variable");
	QBoxLayout* main_layer = new QVBoxLayout(this);
	m_selections = new VariablesSelection(this);
	main_layer->addWidget(m_selections);
	QPushButton* ensure_btn = new QPushButton("ensure", this);
	main_layer->addWidget(ensure_btn);
	connect(ensure_btn, &QPushButton::clicked, this, &VariableSelectedDialog::slot_ensureVariables);
	this->searchVariables();
}

void VariableSelectedDialog::searchVariables()
{
	QFile file(PathManager::instance()->data_path() + "System.json");
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);
		stream.setCodec(QTextCodec::codecForName("utf-8"));
		QString variables = "variables";
		qint64 current = file.size();
		QChar current_char;
		stream.seek(--current);
		while (current)
		{
			stream >> current_char;
			stream.seek(--current);
			if (current_char == ':')
			{
				stream.seek(--current);
				int len = 8;
				do {
					stream >> current_char;
					stream.seek(--current);
					if (variables[len] != current_char)break;
				} while (len--);
				if (len == -1)
				{
					stream.seek(current + 12);
					breakArray(stream);
					break;
				}
			}
		}


		file.close();
	}
}

QString VariableSelectedDialog::GetVariables()
{
	VariableSelectedDialog w;
	if (QDialog::Accepted == w.exec())
	{
		return VariableSelectedDialog::_id;
	}
	return QString();
}

void VariableSelectedDialog::slot_ensureVariables()
{
	VariableSelectedDialog::_id = m_selections->current_id();
	if (!VariableSelectedDialog::_id.isEmpty())
	{
		this->accept();
	}
	else
	{
		this->reject();
	}
}

void VariableSelectedDialog::breakArray(QTextStream& stream)
{
	int id = 0;
	QChar symbol = 0;
	QString c_name;
	while (symbol != ']')
	{
		stream >> symbol;
		if (symbol == '[')continue;
		if (symbol == ',')++id;
		else if (symbol == '\"')
		{
			if (!c_name.isEmpty())
			{
				m_selections->addItem(id, c_name);
				c_name.clear();
			}
		}
		else
		{
			c_name.push_back(symbol);
		}
	}
}