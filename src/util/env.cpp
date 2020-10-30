#include "taco/util/env.h"
#include "taco/tpr.h"
#include <stdio.h>

namespace taco {
namespace util {

std::string cachedtmpdir = "";

void cachedtmpdirCleanup(void) {
  if (cachedtmpdir != ""){
    int rv = tpr_delete_directory_entries( cachedtmpdir.c_str() );
    taco_uassert(rv == 0) <<
      "Unable to create cleanup taco temporary directory. Sorry.";
  }
}
}}
