#ifndef CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_CREDIT_H_
#define CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_CREDIT_H_

#include "./model/s21_model_credit.h"

namespace s21 {

class ControllerCredit {
 public:
  ControllerCredit();
  void CalculateAnnuityPayment(CreditParams &params);
  void CalculateDifferentiatedPayment(CreditParams &params);

 private:
  ModelCredit model_credit_;
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_CREDIT_H_
