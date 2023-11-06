#include "s21_controller_calc.h"

namespace s21 {

s21::ControllerCalc::ControllerCalc() {}

bool s21::ControllerCalc::CalculateExpression(char *expression, double *result,
                                              double x) {
  bool success = true;
  double value;
  if (!model_.Calculate(expression, &value, x)) {
    success = false;
  }
  *result = value;
  return success;
}

}  // namespace s21
