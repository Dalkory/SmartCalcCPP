#ifndef CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_CALC_H_
#define CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_CALC_H_

#include "./model/s21_model_calc.h"

namespace s21 {

class ControllerCalc {
 public:
  ControllerCalc();
  bool CalculateExpression(char *expression, double *result, double x);

 private:
  Model model_;
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_CONTROLLER_H_
