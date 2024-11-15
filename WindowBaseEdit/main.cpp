#include<QApplication>
#include"Widget/ElementDisplayWidget.h"
#include"ElementEditManager/ElementLoader.h"
#include"manager/ConfigInfo.h"
int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	hjdd::manager::ConfigInfo::instance()->loadWorkSpace("D:/res/RpgRes/Project2/");
	hjdd::manager::ElementLibraryLoader::instance()->loadElementsName();

	hjdd::widget::ElementDisplayWidget w;
	w.resize(800, 600);

	w.show();
	int ret = app.exec();
	w.clearElements();
	hjdd::manager::ElementEditorManager::instance()->destroyAll();
	hjdd::manager::ElementLibraryLoader::instance()->unloadAllLib();
	return ret;
}