#ifndef RPGOBJECTEDITORLAUNCH_H_
#define RPGOBJECTEDITORLAUNCH_H_
#include<qobject.h>
class GameObject;
class RpgObjectEditorBase;
class RpgObjectEditorLaunch:public QObject
{
	Q_OBJECT
public:
	RpgObjectEditorLaunch(QObject* parent = nullptr);
	~RpgObjectEditorLaunch();
	void showEditor(GameObject*item);
	void closeEditor();
signals:
	void itemChanged();
private:
	RpgObjectEditorBase* m_current;
};
#endif // !RPGOBJECTEDITORLAUNCH_H_
