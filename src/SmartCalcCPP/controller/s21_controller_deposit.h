#ifndef CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_DEPOSIT_H_
#define CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_DEPOSIT_H_

#include "./model/s21_model_deposit.h"

namespace s21 {

class ControllerDeposit {
 public:
  ControllerDeposit();
  void CalculateDeposit(DepositParams &params);

 private:
  ModelDeposit model_deposit_;
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_DEPOSIT_H_
