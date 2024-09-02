#ifndef RPGTEXTEDITOR_H_
#define RPGTEXTEDITOR_H_
#include"RpgObjectEditorBase.h"
#include<qtextedit.h>


class TextWidget :public QTextEdit
{
	Q_OBJECT
public:
	TextWidget(QWidget* parent = nullptr);
public slots:
	void slot_insertIcon();
	void slot_insertVariable();
	void slot_insertColor();
protected:
	virtual void contextMenuEvent(QContextMenuEvent* event) override;
};

class RpgTextEditor :public RpgObjectEditorBase
{
	Q_OBJECT
public:
	RpgTextEditor(QWidget*parent=nullptr);
public slots:
	void slot_TextChanged();
protected:
	virtual void InitEditor(GameObject* item)override;
private:
	TextWidget* m_text_edit;
};
#endif // !RPGTEXTEDITOR_H_
