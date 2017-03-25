#ifdef EXPORTS
#define DLL_EXPORT _declspec(dllexport)
#else
#define DLL_EXPORT _declspec(dllimport)
#endif

namespace HK
{
	DLL_EXPORT void videoshow();
}