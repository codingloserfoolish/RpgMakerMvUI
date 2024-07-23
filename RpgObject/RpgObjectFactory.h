#ifndef RPGOBJECTFACTORY_H_
#define RPGOBJECTFACTORY_H_
#include"GameObject.h"

class RpgObjectFactory
{
public:
	RpgObjectFactory() = delete;
	RpgObjectFactory(const RpgObjectFactory&) = delete;
	RpgObjectFactory& operator=(const RpgObjectFactory&) = delete;

	static GameObject* create(QString&);

};

#endif // !RPGOBJECTFACTORY_H_
