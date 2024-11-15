#pragma once
#include<QtCore/qglobal.h>
#ifdef DLLITEMEXPORT
#define ITEMEXPORT Q_DECL_EXPORT
#else
#define ITEMEXPORT Q_DECL_IMPORT
#endif // DLLITEMEXPORT