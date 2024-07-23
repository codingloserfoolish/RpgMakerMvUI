#include<qapplication.h>
#include"Widget/StartWidget.h"
#include"Widget/MainWindow.h"
//#include"Widget/ObjectsTreeWidget.h"
//LINE_EXTRA:qtvariantproperty.cpp

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

	StartWidget tw;
	tw.setModal(true);
	if (QDialog::Accepted == tw.exec())
	{
		MainWindow w;
		w.show();
		/*ObjectsTreeWidget w({ "name" });
		w.show();*/
		return a.exec();
	}
	return 0;
}