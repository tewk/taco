#ifndef TACO_MODE_FORMAT_DENSE_H
#define TACO_MODE_FORMAT_DENSE_H

#include "taco/lower/mode_format_impl.h"

namespace taco {

class TACO_EXPORT DenseModeFormat : public ModeFormatImpl {
public:
  DenseModeFormat();
  DenseModeFormat(const bool isOrdered, const bool isUnique);

  ~DenseModeFormat() override {}

  ModeFormat copy(std::vector<ModeFormat::Property> properties) const override;
  
  ModeFunction locate(ir::Expr parentPos, std::vector<ir::Expr> coords,
                      Mode mode) const override;

  ir::Stmt getInsertCoord(ir::Expr p, const std::vector<ir::Expr>& i, 
                          Mode mode) const override;
  ir::Expr getWidth(Mode mode) const override;
  ir::Stmt getInsertInitCoords(ir::Expr pBegin, ir::Expr pEnd, 
                               Mode mode) const override;
  ir::Stmt getInsertInitLevel(ir::Expr szPrev, ir::Expr sz, 
                              Mode mode) const override;
  ir::Stmt getInsertFinalizeLevel(ir::Expr szPrev, ir::Expr sz, 
                                  Mode mode) const override;
  
  std::vector<ir::Expr> getArrays(ir::Expr tensor, int mode, 
                                  int level) const override;

protected:
  ir::Expr getSizeArray(ModePack pack) const;
};

}

#endif
