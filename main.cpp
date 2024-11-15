#include<QApplication>
#include<QRegExp>
//#include"widget/MainWindow.h"
#include"manager/EditorLaunch.h"
#include"manager/ConfigInfo.h"
#include"widget/LauncherWidget.h"
#include"WindowBaseEdit/ElementEditManager/ElementLoader.h"

/*
* 由于WindowBase小世界的原因，释放内存和卸载动态库的顺序必须是
* 先释放RpgObject,再释放EditorLaunch,最后卸载动态库
*/
int main(int argc,char**argv)
{
	QApplication app(argc,argv);

	hjdd::manager::ElementLibraryLoader::instance()->loadElementsName();

	hjdd::widget::MainWindow startWindow;
	
	hjdd::widget::LauncherWidget* w=new hjdd::widget::LauncherWidget;
	w->setupUi();
	w->initLoad();
	w->setStartWindow(&startWindow);
	w->resize(800, 600);
	w->show();

	int exec = app.exec();
	//到这里已经调用了MainWindow的closeEvent

	//释放所有的编辑窗口，关于断开连接的操作在MainWindow的closeEvent里
	hjdd::manager::EditorLaunch::instance()->destroyAll();

	hjdd::manager::ElementLibraryLoader::instance()->unloadAllLib();

	//最后卸载动态库，防止其他物体通过自动释放内存比卸载动态库晚而爆了
	hjdd::manager::ObjectLoader::instance()->unloadAllLib();


	return exec;
}