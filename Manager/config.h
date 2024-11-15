#pragma once
#include<QtCore/qglobal.h>
#ifdef DLL_MANAGER_EXPORT
#define MANAGER_EXPORT Q_DECL_EXPORT
#else
#define MANAGER_EXPORT Q_DECL_IMPORT
#endif // DLLEXPORT
