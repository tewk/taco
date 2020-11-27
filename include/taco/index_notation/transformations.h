#ifndef TACO_TRANSFORMATIONS_H
#define TACO_TRANSFORMATIONS_H

#include <memory>
#include <string>
#include <ostream>
#include <vector>
#include "index_notation.h"
#include <taco_export.h>>

namespace taco {

class TensorVar;
class IndexVar;
class IndexExpr;
class IndexStmt;

class TransformationInterface;
class Reorder;
class Precompute;
class ForAllReplace;
class AddSuchThatPredicates;
class Parallelize;
class TopoReorder;

/// A transformation is an optimization that transforms a statement in the
/// concrete index notation into a new statement that computes the same result
/// in a different way.  Transformations affect the order things are computed
/// in as well as where temporary results are stored.
class TACO_EXPORT Transformation {
public:
  Transformation(Reorder);
  Transformation(Precompute);
  Transformation(ForAllReplace);
  Transformation(Parallelize);
  Transformation(TopoReorder);
  Transformation(AddSuchThatPredicates);

  IndexStmt apply(IndexStmt stmt, std::string *reason = nullptr) const;

  TACO_EXPORT friend std::ostream &operator<<(std::ostream &, const Transformation &);

private:
  std::shared_ptr<const TransformationInterface> transformation;
};


/// Transformation abstract class
class TACO_EXPORT TransformationInterface {
public:
  virtual ~TransformationInterface() = default;
  virtual IndexStmt apply(IndexStmt stmt, std::string *reason = nullptr) const = 0;
  virtual void print(std::ostream &os) const = 0;
};


/// The reorder optimization rewrites an index statement to swap the order of
/// the `i` and `j` loops.
/// Can also supply replacePattern and will find nested foralls with this set of indexvar
/// and reorder them to new ordering
class TACO_EXPORT Reorder : public TransformationInterface {
public:
  Reorder(IndexVar i, IndexVar j);
  Reorder(std::vector<IndexVar> replacePattern);

  IndexVar geti() const;
  IndexVar getj() const;
  const std::vector<IndexVar>& getreplacepattern() const;

  /// Apply the reorder optimization to a concrete index statement.  Returns
  /// an undefined statement and a reason if the statement cannot be lowered.
  IndexStmt apply(IndexStmt stmt, std::string *reason = nullptr) const;

  void print(std::ostream &os) const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

/// Print a reorder command.
TACO_EXPORT std::ostream &operator<<(std::ostream &, const Reorder &);


/// The precompute optimizaton rewrites an index expression to precompute `expr`
/// and store it to the given workspace.
class TACO_EXPORT Precompute : public TransformationInterface {
public:
  Precompute();
  Precompute(IndexExpr expr, IndexVar i, IndexVar iw, TensorVar workspace);

  IndexExpr getExpr() const;
  IndexVar geti() const;
  IndexVar getiw() const;
  TensorVar getWorkspace() const;

  /// Apply the precompute optimization to a concrete index statement.
  IndexStmt apply(IndexStmt stmt, std::string *reason = nullptr) const;

  void print(std::ostream &os) const;

  bool defined() const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

/// Print a precompute command.
TACO_EXPORT std::ostream &operator<<(std::ostream &, const Precompute &);

/// Replaces all occurrences of directly nested forall nodes of pattern with
/// directly nested loops of replacement
class TACO_EXPORT ForAllReplace : public TransformationInterface {
public:
  ForAllReplace();

  ForAllReplace(std::vector<IndexVar> pattern, std::vector<IndexVar> replacement);

  std::vector<IndexVar> getPattern() const;

  std::vector<IndexVar> getReplacement() const;

  IndexStmt apply(IndexStmt stmt, std::string *reason = nullptr) const;

  void print(std::ostream &os) const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

/// Adds a SuchThat node if it does not exist and adds the given IndexVarRels
class TACO_EXPORT AddSuchThatPredicates : public TransformationInterface {
public:
  AddSuchThatPredicates();

  AddSuchThatPredicates(std::vector<IndexVarRel> predicates);

  std::vector<IndexVarRel> getPredicates() const;

  IndexStmt apply(IndexStmt stmt, std::string *reason = nullptr) const;

  void print(std::ostream &os) const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

/// The parallelize optimization tags a Forall as parallelized
/// after checking for preconditions
class TACO_EXPORT Parallelize : public TransformationInterface {
public:
  Parallelize();
  Parallelize(IndexVar i);
  Parallelize(IndexVar i, ParallelUnit parallel_unit, OutputRaceStrategy output_race_strategy);

  IndexVar geti() const;
  ParallelUnit getParallelUnit() const;
  OutputRaceStrategy getOutputRaceStrategy() const;

  /// Apply the parallelize optimization to a concrete index statement.
  IndexStmt apply(IndexStmt stmt, std::string* reason=nullptr) const;

  void print(std::ostream& os) const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

/// Print a ForAllReplace command.
TACO_EXPORT std::ostream &operator<<(std::ostream &, const ForAllReplace &);

/// Print a parallelize command.
TACO_EXPORT std::ostream& operator<<(std::ostream&, const Parallelize&);

TACO_EXPORT std::ostream& operator<<(std::ostream&, const AddSuchThatPredicates&);

// Autoscheduling functions

/**
 * Parallelize the outer forallall loop if it passes preconditions.
 * The preconditions are:
 * 1. The loop iterates over only one data structure,
 * 2. Every result iterator has the insert capability, and
 * 3. No cross-thread reductions.
 */
TACO_EXPORT IndexStmt parallelizeOuterLoop(IndexStmt stmt);

/**
 * Topologically reorder ForAlls so that all tensors are iterated in order.
 * Only reorders first contiguous section of ForAlls iterators form constraints
 * on other dimensions. For example, a {dense, dense, sparse, dense, dense}
 * tensor has constraints i -> k, j -> k, k -> l, k -> m.
 */
TACO_EXPORT IndexStmt reorderLoopsTopologically(IndexStmt stmt);

/**
 * Performs scalar promotion so that reductions are done by accumulating into 
 * scalar temporaries whenever possible.
 */
TACO_EXPORT IndexStmt scalarPromote(IndexStmt stmt);

/**
 * Insert where statements with temporaries into the following statements kinds:
 * 1. The result is a is scattered into but does not support random insert.
 */
TACO_EXPORT IndexStmt insertTemporaries(IndexStmt stmt);
}
#endif
