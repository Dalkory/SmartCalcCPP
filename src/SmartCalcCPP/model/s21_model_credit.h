#ifndef CPP3_S21_SMARTCALC_SRC_S21_MODEL_CREDIT_H_
#define CPP3_S21_SMARTCALC_SRC_S21_MODEL_CREDIT_H_

namespace s21 {

struct CreditParams {
  double percent;
  double month_time;
  double credit_sum;
  double overpay;
  double total_payment;
  double annuity_payment;
  double min_payment;
  double max_payment;
};

class ModelCredit {
 public:
  ModelCredit();
  void CalculateAnnuity(CreditParams &params);
  void CalculateDifferentiated(CreditParams &params);
};

enum Month {
  DECEMBER,
  JANUARY,
  FEBRUARY,
  MARCH,
  APRIL,
  MAY,
  JUNE,
  JULY,
  AUGUST,
  SEPTEMBER,
  OCTOBER,
  NOVEMBER
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_MODEL_CREDIT_H_
