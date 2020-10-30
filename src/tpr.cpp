#include"taco/tpr.h"
#ifdef TACO_WINDOWS
#include<windows.h>
#else
#endif

void tpr_dlclose( void * handle )
{
#ifdef TACO_WINDOWS
	FreeLibrary( (HMODULE) handle) ;
#else
	dlclose(handle);
#endif
}

void* tpr_dlopen( const char * fullpath, int flags )
{
#ifdef TACO_WINDOWS
	return LoadLibrary( fullpath );
#else
	return dlopen(fullpath, flags );
#endif
}

void* tpr_dlsym( void* handle, const char * sym_name )
{
#ifdef TACO_WINDOWS
	return GetProcAddress( (HMODULE) handle, sym_name );
#else
	return dlsym(handle, sym_name);
#endif
}

