#include "s21_model_credit.h"

#include <cmath>
#include <ctime>
#include <limits>

namespace s21 {

const int DAYS_IN_YEAR = 365;
const int MONTHS_IN_YEAR = 12;

ModelCredit::ModelCredit() {}

void ModelCredit::CalculateAnnuity(CreditParams &params) {
  double monthly_interest_rate = params.percent / MONTHS_IN_YEAR / 100.0;
  params.annuity_payment =
      (params.credit_sum * monthly_interest_rate *
       pow(1 + monthly_interest_rate, params.month_time)) /
      (pow(1 + monthly_interest_rate, params.month_time) - 1);
  params.total_payment = params.annuity_payment * params.month_time;
  params.overpay = params.total_payment - params.credit_sum;
}

void ModelCredit::CalculateDifferentiated(CreditParams &params) {
  double monthly_loan_payment = params.credit_sum / params.month_time;
  time_t current_time;
  struct tm *time_info;
  time(&current_time);
  time_info = std::localtime(&current_time);
  int current_month = time_info->tm_mon + 1;
  int current_year = time_info->tm_year + 1900;
  params.min_payment = std::numeric_limits<double>::max();
  params.max_payment = std::numeric_limits<double>::lowest();
  for (int i = 0; i < params.month_time; i++) {
    int current_month_number = current_month + i;
    int current_days;
    if (current_month_number % MONTHS_IN_YEAR == Month::JANUARY)
      current_days = 31;
    else if (current_month_number % MONTHS_IN_YEAR == Month::FEBRUARY) {
      if (current_year % 400 == 0 ||
          (current_year % 100 != 0 && current_year % 4 == 0))
        current_days = 29;
      else
        current_days = 28;
    } else if (current_month_number % MONTHS_IN_YEAR == Month::MARCH ||
               current_month_number % MONTHS_IN_YEAR == Month::MAY ||
               current_month_number % MONTHS_IN_YEAR == Month::JULY ||
               current_month_number % MONTHS_IN_YEAR == Month::AUGUST ||
               current_month_number % MONTHS_IN_YEAR == Month::OCTOBER ||
               current_month_number % MONTHS_IN_YEAR == Month::DECEMBER) {
      current_days = 31;
    } else {
      current_days = 30;
    }
    double interest_amount =
        (params.credit_sum * (params.percent / 100.0) * current_days) /
        DAYS_IN_YEAR;
    params.credit_sum -= monthly_loan_payment;
    params.total_payment += interest_amount + monthly_loan_payment;
    params.overpay += interest_amount;
    double current_payment = interest_amount + monthly_loan_payment;
    params.min_payment = std::min(params.min_payment, current_payment);
    params.max_payment = std::max(params.max_payment, current_payment);
    if (current_month_number % MONTHS_IN_YEAR == Month::DECEMBER)
      current_year++;
  }
}

}  // namespace s21
