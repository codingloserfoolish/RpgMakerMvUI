#include"StartWidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<qlabel.h>
#include<qpushbutton.h>
#include<qfiledialog.h>
#include<qmessagebox.h>
#include"../Manager/PathManager.h"
StartWidget::StartWidget(QWidget* parent):QDialog(parent)
{
	QVBoxLayout* main_layer = new QVBoxLayout(this);
	//------
	QHBoxLayout* s_lineEdit_layer = new QHBoxLayout;
	main_layer->addLayout(s_lineEdit_layer);

	m_sceneName = new QLineEdit(this);
	s_lineEdit_layer->addWidget(new QLabel("scene name", this));
	s_lineEdit_layer->addWidget(m_sceneName);
	//------
	QHBoxLayout* p_lineEdit_layer = new QHBoxLayout;
	main_layer->addLayout(p_lineEdit_layer);

	m_projectName = new QLineEdit(this);
	p_lineEdit_layer->addWidget(new QLabel("rpgproject", this));
	p_lineEdit_layer->addWidget(m_projectName);
	QPushButton* searchButton = new QPushButton("...", this);
	p_lineEdit_layer->addWidget(searchButton);
	connect(searchButton, &QPushButton::clicked, this, &StartWidget::slot_popSearchDlg);
	//------
	QHBoxLayout* btn_layer = new QHBoxLayout;
	main_layer->addLayout(btn_layer);

	QPushButton* ensure_btn = new QPushButton("ensure", this);
	QPushButton* cancel_btn = new QPushButton("cancel", this);
	btn_layer->addWidget(ensure_btn);
	btn_layer->addWidget(cancel_btn);
	connect(ensure_btn, &QPushButton::clicked, this, &StartWidget::slot_ensure);
	connect(cancel_btn, &QPushButton::clicked, this, &StartWidget::slot_cancel);

}

void StartWidget::slot_ensure()
{
	QString scene_name = m_sceneName->text();
	if (scene_name.isEmpty()) { QMessageBox::information(this, "warning", "scene needs a name!"); return; }
	QString path = m_projectName->text();
	if(path.isEmpty()){ QMessageBox::information(this, "warning", "path can't be empty!"); return; }
	scene_name.replace(" ", "_");

	path = path.left(path.lastIndexOf('/') + 1);

	HistoryList history("./history/history.txt");
	if (history.exist(scene_name, path)) { QMessageBox::information(this, "warning", "this scene has been existed"); return; }
	PathManager::instance()->open(path);
	PathManager::instance()->setSceneName(scene_name);
	this->accept();
	this->close();
}

void StartWidget::slot_cancel()
{
	this->reject();
	this->close();
}

void StartWidget::slot_popSearchDlg()
{
	QString path = QFileDialog::getOpenFileName(this, "select your rpgproject", QDir::currentPath(), QString("RpgProject(*.rpgproject)"));
	if (path.isEmpty())return;
	m_projectName->setText(path);
}