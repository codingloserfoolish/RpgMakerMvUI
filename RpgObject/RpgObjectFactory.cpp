#include"RpgObjectFactory.h"
#include"RpgWindowBase.h"
#include"RpgSprite.h"
#include"RpgText.h"
GameObject* RpgObjectFactory::create(QString&type)
{
	GameObject* result = 0;
	if (type == "RpgWindow")
	{
		result = new RpgWindow(100, 100);
	}
	else if (type == "RpgSprite")
	{
		result = new RpgSprite;
	}
	else if (type == "RpgText")
	{
		result = new RpgText;
	}
	return result;
}
