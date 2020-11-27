#ifndef TACO_ERROR_MESSAGES_H
#define TACO_ERROR_MESSAGES_H

#include <string>
#include <taco_export.h>

namespace taco {
namespace error {

// unsupported type bit width error
TACO_EXPORT extern const std::string type_mismatch;
TACO_EXPORT extern const std::string type_bitwidt;

// TensorVar::setIndexExpression error messages
TACO_EXPORT extern const std::string expr_dimension_mismatch;
TACO_EXPORT extern const std::string expr_transposition;
TACO_EXPORT extern const std::string expr_distribution;
TACO_EXPORT extern const std::string expr_einsum_missformed;

// compile error messages
TACO_EXPORT extern const std::string compile_without_expr;
TACO_EXPORT extern const std::string compile_tensor_name_collision;

// assemble error messages
TACO_EXPORT extern const std::string assemble_without_compile;

// compute error messages
TACO_EXPORT extern const std::string compute_without_compile;

// factory function error messages
TACO_EXPORT extern const std::string requires_matrix;

#define INIT_REASON(reason) \
string reason_;             \
do {                        \
  if (reason == nullptr) {  \
    reason = &reason_;      \
  }                         \
  *reason = "";             \
} while (0)

}}
#endif
