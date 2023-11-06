#include "s21_controller_credit.h"

namespace s21 {

s21::ControllerCredit::ControllerCredit() {}

void s21::ControllerCredit::CalculateAnnuityPayment(CreditParams &params) {
  model_credit_.CalculateAnnuity(params);
}

void s21::ControllerCredit::CalculateDifferentiatedPayment(
    CreditParams &params) {
  model_credit_.CalculateDifferentiated(params);
}

}  // namespace s21
