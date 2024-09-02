#ifndef WINDOWEDITORMAINWIDGET_H_
#define WINDOWEDITORMAINWIDGET_H_
#include<qmenu.h>
#include"ElementWidget/ElementEditLaunchWidget.h"
#include"../RpgObjectEditor/RpgObjectEditorBase.h"
class RpgWindow;
class WinElementMainMenu;
class WinElementBase;
class WindowEditorMainWidget :public RpgObjectEditorBase
{
	Q_OBJECT
public:
	WindowEditorMainWidget(QWidget* parent = nullptr);
	void initMenu();

	WinElementBase* pickupElement(const QPoint&point);
	void append_WinElement(WinElementBase*item);
public slots:
	void slot_createElement(const QString&);
	void slot_showEditWidget();
	void slot_updateWidget() { this->update(); }
protected:
	virtual void paintEvent(QPaintEvent* pe)override;
	virtual void mousePressEvent(QMouseEvent* event)override;
	virtual void mouseMoveEvent(QMouseEvent* event)override;
	virtual void closeEvent(QCloseEvent* event)override;

	virtual void InitEditor(GameObject* item)override;
private:
	int m_weType;
	QPoint m_diff;
	QPoint m_create_pos;

	RpgWindow* m_window;
	WinElementBase* m_active;
	QMenu m_popContextMenu;
	WinElementMainMenu* m_wepopCtxMenu;
	ElementEditLaunchWidget m_editLauncher;
	
};


#endif // !WINDOWEDITORMAINWIDGET_H_
