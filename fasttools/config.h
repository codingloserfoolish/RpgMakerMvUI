#pragma once
#include<QtCore/qglobal.h>
#ifdef DLLEXPORT
#define EXPORT Q_DECL_EXPORT
#else
#define EXPORT Q_DECL_IMPORT
#endif // DLLEXPORT
