#ifndef CUDA_H
#define CUDA_H

#include <string>
#include <sstream>
#include <ostream>
#include <taco_export.h>

#ifndef CUDA_BUILT
  #define CUDA_BUILT false
#endif

namespace taco {
/// Functions used by taco to interface with CUDA (especially unified memory)
/// Check if should use CUDA codegen
TACO_EXPORT bool should_use_CUDA_codegen();
/// Check if should use CUDA unified memory
TACO_EXPORT bool should_use_CUDA_unified_memory();
/// Enable/Disable CUDA codegen
TACO_EXPORT void set_CUDA_codegen_enabled(bool enabled);
/// Enable/Disable CUDA unified memory
TACO_EXPORT void set_CUDA_unified_memory_enabled(bool enabled);
/// Gets default compiler flags by checking current gpu model
TACO_EXPORT std::string get_default_CUDA_compiler_flags();
/// Allocates memory using unified memory (and checks for errors)
TACO_EXPORT void* cuda_unified_alloc(size_t size);
/// Frees memory from unified memory (and checks for errors)
TACO_EXPORT void cuda_unified_free(void *ptr);

}

#endif
