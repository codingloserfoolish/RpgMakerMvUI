#include<qapplication.h>
#include"Widget/MainWindow.h"
//#include"Widget/ObjectsTreeWidget.h"
//LINE_EXTRA:qtvariantproperty.cpp


int main(int arg, char** argv)
{
	QApplication a(arg, argv);
	MainWindow w;
	w.show();
	/*ObjectsTreeWidget w({ "name" });
	w.show();*/
	return a.exec();
}