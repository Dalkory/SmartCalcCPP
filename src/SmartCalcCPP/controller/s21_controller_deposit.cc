#include "s21_controller_deposit.h"

namespace s21 {

s21::ControllerDeposit::ControllerDeposit() {}

void s21::ControllerDeposit::CalculateDeposit(DepositParams &params) {
  model_deposit_.CalculateDeposit(params);
}

}  // namespace s21
