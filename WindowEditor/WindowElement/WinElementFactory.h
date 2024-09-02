#ifndef WINELEMENTFACTORY_H_
#define WINELEMENTFACTORY_H_
#include<qstring.h>
class WinElementBase;
class WinElementFactory
{
public:
	static WinElementBase* create(const QString& name);
};
#endif // !WINELEMENTFACTORY_H_
