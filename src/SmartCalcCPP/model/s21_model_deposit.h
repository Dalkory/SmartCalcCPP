#ifndef CPP3_S21_SMARTCALC_SRC_S21_MODEL_DEPOSIT_H_
#define CPP3_S21_SMARTCALC_SRC_S21_MODEL_DEPOSIT_H_

namespace s21 {

struct DepositParams {
  double initial_amount;
  double procent;
  int month_time;
  double nalog;
  double sum_nalog;
  int capitalization;
  double final_amount;
  int compound_frequency;
  double withdrawal_amount;
  double deposit_amount;
  double profit;
};

class ModelDeposit {
 public:
  ModelDeposit();
  void CalculateDeposit(DepositParams &params);
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALC_SRC_S21_MODEL_DEPOSIT_H_
