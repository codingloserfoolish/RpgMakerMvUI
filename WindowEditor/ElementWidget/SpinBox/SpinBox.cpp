#include"SpinBox.h"

SpinBox::SpinBox(QWidget* parent):QSpinBox(parent)
{
	this->setRange(0, 800);
}
