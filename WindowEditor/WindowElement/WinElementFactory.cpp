#include"WinElementFactory.h"
#include"wbLinesText.h"
#include"wbBitmap.h"
WinElementBase* WinElementFactory::create(const QString& name)
{
	if (name == "wbLinesText")return new wbLinesText;
	else if (name == "wbBitmap")return new wbBitmap;
	else return 0;
}
