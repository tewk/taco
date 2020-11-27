#ifndef TACO_UTIL_NAME_GENERATOR_H
#define TACO_UTIL_NAME_GENERATOR_H

#include <string>
#include <map>
#include <vector>
#include <taco_export.h>

namespace taco {
namespace util {

TACO_EXPORT std::string uniqueName(char prefix);
TACO_EXPORT std::string uniqueName(const std::string& prefix);

int getUniqueId();

class NameGenerator {
public:
  NameGenerator();
  NameGenerator(std::vector<std::string> reserved);

  std::string getUniqueName(std::string name);

private:
  std::map<std::string, int> nameCounters;
};

}}
#endif
