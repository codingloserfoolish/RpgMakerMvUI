#ifndef SPINBOX_H_
#define SPINBOX_H_
#include<qspinbox.h>
class SpinBox :public QSpinBox
{
	Q_OBJECT
public:
	SpinBox(QWidget* parent = nullptr);
};
#endif // !SPINBOX_H_
