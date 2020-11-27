#ifndef TACO_FORMAT_H
#define TACO_FORMAT_H

#include <string>
#include <memory>
#include <vector>
#include <ostream>
#include "taco/type.h"
#include <taco_export.h>

namespace taco {

class ModeFormat;
class ModeFormatPack;
class ModeFormatImpl;


/// A Format describes the data layout of a tensor, and the sparse index data
/// structures that describe locations of non-zero tensor components.
class TACO_EXPORT Format {
public:
  /// Create a format for a 0-order tensor (a scalar).
  Format();

  /// Create a format for a 1-order tensor (a vector).
  Format(const ModeFormat modeFormat);

  Format(const std::initializer_list<ModeFormatPack>& modeFormatPacks);
  
  /// Create a tensor format whose modes have the given mode storage formats.
  /// The format of mode i is specified by modeFormats[i]. Mode i is stored in
  /// position i.
  Format(const std::vector<ModeFormatPack>& modeFormatPacks);

  /// Create a tensor format where the modes have the given mode storage formats
  /// and modes are stored in the given sequence. The format of the mode stored
  /// in position i is specified by the i-th element of modeFormatPacks
  /// linearized. The mode stored in position i is specified by modeOrdering[i].
  Format(const std::vector<ModeFormatPack>& modeFormatPacks,
         const std::vector<int>& modeOrdering);

  /// Returns the number of modes in the format.
  int getOrder() const;

  /// Get the storage types of the modes. The type of the mode stored in
  /// position i is specifed by element i of the returned vector.
  const std::vector<ModeFormat> getModeFormats() const;

  /// Get the storage formats of the modes, with modes that share the same
  /// physical storage grouped together.
  const std::vector<ModeFormatPack>& getModeFormatPacks() const;

  /// Get the ordering in which the modes are stored. The mode stored in
  /// position i is specifed by element i of the returned vector.
  const std::vector<int>& getModeOrdering() const;

  /// Gets the types of the coordinate arrays for each level
  const std::vector<std::vector<Datatype>>& getLevelArrayTypes() const;

  /// Gets the type of the position array for level i
  Datatype getCoordinateTypePos(size_t level) const;

  /// Gets the type of the idx array for level i
  Datatype getCoordinateTypeIdx(size_t level) const;

  /// Sets the types of the coordinate arrays for each level
  void setLevelArrayTypes(std::vector<std::vector<Datatype>> levelArrayTypes);

private:
  std::vector<ModeFormatPack> modeFormatPacks;
  std::vector<int> modeOrdering;
  std::vector<std::vector<Datatype>> levelArrayTypes;
};

TACO_EXPORT bool operator==(const Format&, const Format&);
TACO_EXPORT bool operator!=(const Format&, const Format&);
TACO_EXPORT std::ostream& operator<<(std::ostream&, const Format&);


/// The type of a mode defines how it is stored.  For example, a mode may be
/// stored as a dense array, a compressed sparse representation, or a hash map.
/// New mode formats can be defined by extending ModeTypeImpl.
class TACO_EXPORT ModeFormat {
public:
  /// Aliases for predefined mode formats
  static ModeFormat dense;       /// e.g., first mode in CSR
  static ModeFormat compressed;  /// e.g., second mode in CSR
  static ModeFormat singleton;   /// e.g., second mode in COO

  static ModeFormat sparse;      /// alias for compressed
  static ModeFormat Dense;       /// alias for dense
  static ModeFormat Compressed;  /// alias for compressed
  static ModeFormat Sparse;      /// alias for compressed
  static ModeFormat Singleton;   /// alias for singleton

  /// Properties of a mode format
  enum Property {
    FULL, NOT_FULL, ORDERED, NOT_ORDERED, UNIQUE, NOT_UNIQUE, BRANCHLESS,
    NOT_BRANCHLESS, COMPACT, NOT_COMPACT
  };

  /// Instantiates an undefined mode format
  ModeFormat();

  /// Instantiates a new mode format
  ModeFormat(const std::shared_ptr<ModeFormatImpl> impl);

  /// Instantiates a variant of the mode format with a differently configured
  /// property
  ModeFormat operator()(Property property) const;

  /// Instantiates a variant of the mode format with differently configured
  /// properties
  ModeFormat operator()(const std::vector<Property>& properties = {}) const;

  /// Returns string identifying mode format. The format name should not reflect
  /// property configurations; mode formats with differently configured properties
  /// should return the same name.
  std::string getName() const;

  /// Returns true if the mode format has the given properties.
  bool hasProperties(const std::vector<Property>& properties) const;

  /// Returns true if a mode format has a specific property, false otherwise
  bool isFull() const;
  bool isOrdered() const;
  bool isUnique() const;
  bool isBranchless() const;
  bool isCompact() const;

  /// Returns true if a mode format has a specific capability, false otherwise
  bool hasCoordValIter() const;
  bool hasCoordPosIter() const;
  bool hasLocate() const;
  bool hasInsert() const;
  bool hasAppend() const;

  /// Returns true if mode format is defined, false otherwise. An undefined mode
  /// type can be used to indicate a mode whose format is not (yet) known.
  bool defined() const;

  TACO_EXPORT friend bool operator==(const ModeFormat&, const ModeFormat&);
  TACO_EXPORT friend bool operator!=(const ModeFormat&, const ModeFormat&);
  TACO_EXPORT friend std::ostream& operator<<(std::ostream&, const ModeFormat&);

private:
  std::shared_ptr<const ModeFormatImpl> impl;

  friend class ModePack;
  friend class Iterator;
};


class TACO_EXPORT ModeFormatPack {
public:
  ModeFormatPack(const std::vector<ModeFormat> modeFormats);
  ModeFormatPack(const std::initializer_list<ModeFormat> modeFormats);
  ModeFormatPack(const ModeFormat modeFormat);

  /// Get the storage types of the modes. The type of the mode stored in
  /// position i is specifed by element i of the returned vector.
  const std::vector<ModeFormat>& getModeFormats() const;

private:
  std::vector<ModeFormat> modeFormats;
};

TACO_EXPORT bool operator==(const ModeFormatPack&, const ModeFormatPack&);
TACO_EXPORT bool operator!=(const ModeFormatPack&, const ModeFormatPack&);
TACO_EXPORT std::ostream& operator<<(std::ostream&, const ModeFormatPack&);


/// Predefined formats
/// @{
TACO_EXPORT extern const ModeFormat Dense;
TACO_EXPORT extern const ModeFormat Compressed;
TACO_EXPORT extern const ModeFormat Sparse;
TACO_EXPORT extern const ModeFormat Singleton;

TACO_EXPORT extern const ModeFormat dense;
TACO_EXPORT extern const ModeFormat compressed;
TACO_EXPORT extern const ModeFormat sparse;
TACO_EXPORT extern const ModeFormat singleton;

TACO_EXPORT extern const Format CSR;
TACO_EXPORT extern const Format CSC;
TACO_EXPORT extern const Format DCSR;
TACO_EXPORT extern const Format DCSC;

const Format COO(int order, bool isUnique = true, bool isOrdered = true, 
                 bool isAoS = false, const std::vector<int>& modeOrdering = {});
/// @}

/// True if all modes are dense.
TACO_EXPORT bool isDense(const Format&);

}
#endif
