#ifndef SRC_UTIL_ENV_H_
#define SRC_UTIL_ENV_H_

#include <string>
#include <cstring>
#include "taco/tpr.h"

#include "taco/error.h"
#include <taco_export.h>

namespace taco {
namespace util {
TACO_EXPORT std::string getFromEnv(std::string flag, std::string dflt);
TACO_EXPORT std::string getTmpdir();
TACO_EXPORT extern std::string cachedtmpdir;
TACO_EXPORT extern void cachedtmpdirCleanup(void);

inline std::string getFromEnv(std::string flag, std::string dflt) {
  char const *ret = getenv(flag.c_str());
  if (!ret) {
    return dflt;
  } else {
    return std::string(ret);
  }
}

inline std::string getTmpdir() {
  if (cachedtmpdir == ""){
    // use posix logic for finding a temp dir
    auto tmpdir = getFromEnv("TMPDIR", "/tmp/");

    // if the directory does not have a trailing slash, add one
    if (tmpdir.back() != '/') {
      tmpdir += '/';
    }

    // ensure it is an absolute path
     taco_uassert(tmpdir.front() == '/') <<
      "The TMPDIR environment variable must be an absolute path";

    taco_uassert(tpr_file_exists(tmpdir.c_str()) == 0) <<
      "Unable to write to temporary directory for code generation. "
      "Please set the environment variable TMPDIR to somewhere writable";

    // ensure that we use a taco tmpdir unique to this process.
    auto tacotmpdirtemplate = tmpdir + "taco_tmp_XXXXXX";
    char *ctacotmpdirtemplate = new char[tacotmpdirtemplate.length() + 1];
    std::strcpy(ctacotmpdirtemplate, tacotmpdirtemplate.c_str());
    char *ctacotmpdir = tpr_mkdtemp(ctacotmpdirtemplate);
    taco_uassert(ctacotmpdir != NULL) <<
      "Unable to create taco temporary directory for code generation. Please set"
      "the environment variable TMPDIR to somewhere searchable and writable";
    std::string tacotmpdir(ctacotmpdir);
    delete [] ctacotmpdirtemplate;

    // if the directory does not have a trailing slash, add one
    if (tacotmpdir.back() != '/') {
      tacotmpdir += '/';
    }

    cachedtmpdir = tacotmpdir;

    //cleanup unless we are in debug mode
    #ifndef TACO_DEBUG
      atexit(cachedtmpdirCleanup);
    #endif
  }
  return cachedtmpdir;
}

}}

#endif /* SRC_UTIL_ENV_H_ */
