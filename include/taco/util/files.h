#ifndef TACO_UTIL_FILES_H
#define TACO_UTIL_FILES_H

#include <string>
#include <fstream>
#include <taco_export.h>

namespace taco {
namespace util {

TACO_EXPORT std::string sanitizePath(std::string path);

TACO_EXPORT void openStream(std::fstream& stream, std::string path, std::fstream::openmode mode);

}}
#endif
