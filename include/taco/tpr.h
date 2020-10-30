#pragma once
//Taco Portable Runtime

#ifdef TACO_WINDOWS
#include<io.h>
#include<errno.h>
#include<direct.h>
#include<time.h>
#include<process.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#else
#include<unistd.h>
#include<tfw.h>
#endif

inline int tpr_file_exists( const char *path )
{
#ifdef TACO_WINDOWS
	return _access( path, 0 );
#else
	return access( path, F_OK );
#endif
}

inline int tpr_access( const char *path, int mode )
{
#ifdef TACO_WINDOWS
	return _access( path, mode );
#else
	return access( path, mode );
#endif
}

// patterned after https://github.com/microsoft/microsoft-r-open/blob/master/source/src/main/mkdtemp.c
inline char* tpr_mkdtemp( char *tmpl )
{
#ifdef TACO_WINDOWS
# define __set_errno(Val) errno = (Val)
  constexpr static const int max_iterations = 238328;
  constexpr static const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  int save_errno = errno;

  const int len = (int) strlen(tmpl);
  if (len < 6 || strcmp (&tmpl[len - 6], "XXXXXX"))
  {
	  __set_errno (EINVAL);
	  return NULL;
  }

  /* This is where the Xs start.  */
  char* XXXXXX = &tmpl[len - 6];

  /* Get some more or less random data.  We need 36 bits. */
  srand( static_cast< unsigned int >( time( NULL ) ) );
  const uint64_t random_time_bits = rand();
  static uint64_t value;
  value += (random_time_bits << 8) ^ _getpid ();

  for( int count = 0; count < max_iterations; value += 7777, ++count )
  {
	  uint64_t v = value;
	  for (int j = 0; j < 6; ++j)
	  {

		  /* Fill in the random bits.  */
		  XXXXXX[j] = letters[v % 62]; v /= 62;
	  }

	  const int fd = _mkdir (tmpl);
	  //fd = mkdir (tmpl, S_IRUSR | S_IWUSR | S_IXUSR);

	  if( fd == 0)
	  {
		return tmpl;
	  }
	  else if (fd >= 0)
	  {
		  __set_errno (save_errno);
		  return NULL;
	  }
	  else if (errno != EEXIST)
	  {
		  return NULL;
	  }
  }

  /* We got out of the loop bVecause we ran out of combinations to try.  */
  __set_errno (EEXIST);
  return NULL;
#else
	return mkdtemp( tmpl );
#endif
}

inline int tpr_mkdir( const char *path, int mode = 0755 )
{
#ifdef TACO_WINDOWS
	return _mkdir( path );
#else
	return mkdir( path, mode );
#endif
}

#ifdef TACO_WINDOWS
#else
static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);
    taco_uassert(rv == 0) <<
      "Unable to create cleanup taco temporary directory. Sorry.";
    return rv;
}
#endif

inline int tpr_delete_directory_entries( const char *path )
{
#ifdef TACO_WINDOWS
	printf( "tpr_nftw not implemented %s", path );
	exit(-1);
#else
	return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
#endif
}

#ifdef TACO_WINDOWS
#define tpr_RTLD_NOW 0x1 
#define tpr_RTLD_LOCAL 0x2
#define tpr_RTLD_GLOBAL 0x4
#else
#define tpr_RTLD_NOW RTLD_NOW
#define tpr_RTLD_LOCAL RTLD_LOCAL
#define tpr_RTLD_GLOBAL RTLD_GLOBAL
#endif

void tpr_dlclose( void * handle );
void* tpr_dlopen( const char * fullpath, int flags = 0 );
void* tpr_dlsym( void* handle, const char * sym_name );

