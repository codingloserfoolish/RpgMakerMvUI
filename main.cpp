#include<qapplication.h>
#include"Widget/PreStartWidget.h"
#include"Widget/StartWidget.h"
#include"Widget/LoadWidget.h"
#include"Widget/MainWindow.h"
#include"WindowEditor/WindowEditorMainWidget.h"
#include"Manager/ConfigureInfoContainer.h"
//LINE_EXTRA:qtvariantproperty.cpp

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	if (type != QtWarningMsg || !msg.startsWith("QWindowsWindow::setGeometry")) {
		QByteArray localMsg = msg.toLocal8Bit();
		fprintf(stdout, localMsg.constData());
	}
}

int main(int arg, char** argv)
{
	qInstallMessageHandler(myMessageOutput);
	QApplication a(arg, argv);
	int exec_data = 0;
	PreStartWidget* psw=new PreStartWidget;
	StartWidget* tw = new StartWidget;
	LoadWidget* lw = new LoadWidget;

	psw->setModal(true);
	tw->setModal(true);
	lw->setModal(true);
	int ret = 0;
	int result = 0;
	while (psw && (ret=psw->exec()))
	{
		if (ret == 1&& QDialog::Accepted == tw->exec())
		{
			result = 1;
		}
		else if (ret == 2 && QDialog::Accepted == lw->exec())
		{
			result = 2;
		}

		if (result)
		{
			delete psw;
			psw = 0;
		}
	}
	delete tw; delete lw;
	tw = 0; lw = 0;
	if (ret)
	{
		ConfigureInfoContainer::instance()->setGameFont(PathManager::instance()->font_path());
		MainWindow w;
		if (result == 2)w.emit_load();
		w.show();
		exec_data = a.exec();
	}

	PathManager::destroy();
	ConfigureInfoContainer::destroy();
	return exec_data;
}