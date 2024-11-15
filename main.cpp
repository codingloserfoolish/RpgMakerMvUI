#include<QApplication>
#include<QRegExp>
//#include"widget/MainWindow.h"
#include"manager/EditorLaunch.h"
#include"manager/ConfigInfo.h"
#include"widget/LauncherWidget.h"
#include"WindowBaseEdit/ElementEditManager/ElementLoader.h"

/*
* ����WindowBaseС�����ԭ���ͷ��ڴ��ж�ض�̬���˳�������
* ���ͷ�RpgObject,���ͷ�EditorLaunch,���ж�ض�̬��
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
	//�������Ѿ�������MainWindow��closeEvent

	//�ͷ����еı༭���ڣ����ڶϿ����ӵĲ�����MainWindow��closeEvent��
	hjdd::manager::EditorLaunch::instance()->destroyAll();

	hjdd::manager::ElementLibraryLoader::instance()->unloadAllLib();

	//���ж�ض�̬�⣬��ֹ��������ͨ���Զ��ͷ��ڴ��ж�ض�̬���������
	hjdd::manager::ObjectLoader::instance()->unloadAllLib();


	return exec;
}