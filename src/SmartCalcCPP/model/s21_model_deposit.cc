#include "s21_model_deposit.h"

#include <cmath>

namespace s21 {

ModelDeposit::ModelDeposit() {}

void ModelDeposit::CalculateDeposit(DepositParams& params) {
  params.initial_amount -= params.withdrawal_amount;
  params.initial_amount += params.deposit_amount;

  if (params.capitalization == 0) {
    params.profit =
        (params.initial_amount * (params.procent / 100) * params.month_time) /
        12;
    params.final_amount = params.initial_amount + params.profit;
  } else {
    double r = params.procent / 100;
    double rate = 1 + r / params.compound_frequency;
    int compound_periods = params.compound_frequency * (params.month_time / 12);
    params.profit = params.initial_amount * (pow(rate, compound_periods) - 1);
    params.final_amount = params.profit + params.initial_amount;
  }

  if (params.profit >= 42500) {
    params.sum_nalog = params.nalog * 0.01 * params.profit;
    params.profit -= params.sum_nalog;
  }
}

}  // namespace s21