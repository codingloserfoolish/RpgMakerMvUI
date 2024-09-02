#ifndef LINESTEXTWIDGET_H_
#define LINESTEXTWIDGET_H_
#include<qdialog.h>
#include<qtextedit.h>
#include"../ElementWidgetBase/ElementWidgetBase.h"

class LinesTextEditWidget:public QTextEdit
{
	Q_OBJECT
public:
	LinesTextEditWidget(QWidget* parent = nullptr);
	void insertText_Extra(QString& text);
public slots:
	void slot_selectIcon();
	void slot_selectVariable();
protected:
	virtual void contextMenuEvent(QContextMenuEvent* event) override;

};

class LinesTextEditMainWidget :public ElementWidgetBase
{
	Q_OBJECT
public:
	LinesTextEditMainWidget(QWidget* parent = nullptr);
public slots:
	void slot_TextChanged();
protected:
	virtual void setOtherProperty(WinElementBase*)override;
private:
	LinesTextEditWidget* m_text_edit;
};

#endif // !LINESTEXTWIDGET_H_
