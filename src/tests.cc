#include <stdio.h>
#include <string.h>

#include <cctype>
#include <cmath>
#include <cstring>

#include "./SmartCalcCPP/model/s21_model_calc.h"
#include "./SmartCalcCPP/model/s21_model_credit.h"
#include "./SmartCalcCPP/model/s21_model_deposit.h"
#include "gtest/gtest.h"

void CheckCorrectExpression(char *expression, double answer) {
  s21::Model calculus;
  double calculus_result;
  EXPECT_FALSE(calculus.Calculate(expression, &calculus_result, 0));
  EXPECT_DOUBLE_EQ(answer, calculus_result);
}

void CheckInCorrectExpression(char *expression) {
  s21::Model calculus(expression);
  double calculus_result;
  EXPECT_TRUE(calculus.Calculate(expression, &calculus_result, 0));
}

TEST(test_calc_model_num, test_1) {
  char expression[] = {"2^3^2"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, 512);
}

TEST(test_calc_model_num, test_2) {
  char expression[] = {"1/2 + (2+3)/(sin(9-2)^2-6/7)"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, -11.250564665395567);
}

TEST(test_calc_model_num, test_3) {
  char expression[] = {"sin(30)"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, -0.98803162409286183);
}

TEST(test_calc_model_num, test_4) {
  char expression[] = {
      "sin(2) * cos(5) + tan(67^2) / sin(128) - atan(sqrt(9)) "
      "- (4mod2) * log(5)"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, -1.475321162572871);
}

TEST(test_calc_model_num, test_5) {
  char expression[] = {"77"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, 77);
}

TEST(test_calc_model_num, test_6) {
  char expression[] = {"-15"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, -15);
}

TEST(test_calc_model_num, test_7) {
  char expression[] = {"3.3"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, 3.3);
}

TEST(test_calc_model_num, test_8) {
  char expression[] = {"ln(5)"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, 1.6094379124341);
}

TEST(test_calc_model_num, test_9) {
  char expression[] = {"+5"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, 5);
}

TEST(test_calc_model_num, test_10) {
  char expression[] = {"asin(1)"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, 1.5707963267948966);
}

TEST(test_calc_model_num, test_11) {
  char expression[] = {"acos(1)"};
  char *pexp = &expression[0];
  CheckCorrectExpression(pexp, 0);
}

TEST(test_calc_model_error, test_1) {
  char expression[] = {"(30+1"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}
TEST(test_calc_model_error, test_2) {
  char expression[] = {"1/2+(2.3.5+3)"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}
TEST(test_calc_model_error, test_3) {
  char expression[] = {"**"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}

TEST(test_calc_model_error, test_4) {
  char expression[] = {"-2.2.2"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}

TEST(test_calc_model_error, test_5) {
  char expression[] = {"1--1"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}

TEST(test_calc_model_error, test_6) {
  char expression[] = {"1-"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}

TEST(test_calc_model_error, test_8) {
  char expression[] = {"("};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}

TEST(test_calc_model_error, test_9) {
  char expression[] = {"2//2"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}

TEST(test_calc_model_error, test_10) {
  char expression[] = {"2^"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}

TEST(test_calc_model_error, test_11) {
  char expression[] = {")"};
  char *pexp = &expression[0];
  CheckInCorrectExpression(pexp);
}

TEST(test_calc_credit, test_anuitet) {
  s21::CreditParams params;
  params.credit_sum = 10000;
  params.percent = 0.5;
  params.month_time = 12;
  params.annuity_payment = 0.0;
  params.total_payment = 0.0;
  params.overpay = 0.0;

  s21::ModelCredit credit;
  credit.CalculateAnnuity(params);

  EXPECT_DOUBLE_EQ(835.59, round(params.annuity_payment * 100) / 100);
  EXPECT_DOUBLE_EQ(27.10, round(params.overpay * 100) / 100);
  EXPECT_DOUBLE_EQ(10027.10, round(params.total_payment * 100) / 100);
}

TEST(test_calc_credit, test_diff) {
  s21::CreditParams params;
  params.credit_sum = 10000;
  params.percent = 0.5;
  params.month_time = 12;
  params.annuity_payment = 0.0;
  params.total_payment = 0.0;
  params.overpay = 0.0;

  s21::ModelCredit credit;
  credit.CalculateDifferentiated(params);

  EXPECT_DOUBLE_EQ(833.69, round(params.min_payment * 100) / 100);
  EXPECT_DOUBLE_EQ(837.44, round(params.max_payment * 100) / 100);
  EXPECT_DOUBLE_EQ(27.10, round(params.overpay * 100) / 100);
  EXPECT_DOUBLE_EQ(10027.1, round(params.total_payment * 100) / 100);
}

TEST(test_calc_deposit, test_1) {
  s21::DepositParams params;
  params.initial_amount = 100000;
  params.procent = 0.7;
  params.month_time = 12;
  params.nalog = 13.0;
  params.deposit_amount = 0.0;     // Initialize deposit amount
  params.withdrawal_amount = 0.0;  // Initialize withdrawal amount
  params.profit = 0.0;
  params.compound_frequency = 0.0;
  params.final_amount = 0.0;
  params.capitalization = 0.0;
  params.sum_nalog = 0.0;

  s21::ModelDeposit deposit;
  deposit.CalculateDeposit(params);

  EXPECT_DOUBLE_EQ(699.99999999999989, params.profit * 100 / 100);
  EXPECT_DOUBLE_EQ(0, params.sum_nalog);
  EXPECT_DOUBLE_EQ(100700, params.final_amount * 100 / 100);
}

TEST(test_calc_deposit, test_2) {
  s21::DepositParams params;
  params.initial_amount = 100000;
  params.procent = 75.72121;
  params.month_time = 1221;
  params.nalog = 13.0;
  params.deposit_amount = 0.0;     // Initialize deposit amount
  params.withdrawal_amount = 0.0;  // Initialize withdrawal amount
  params.profit = 0.0;
  params.compound_frequency = 0.0;
  params.final_amount = 0.0;
  params.capitalization = 0.0;
  params.sum_nalog = 0.0;

  s21::ModelDeposit deposit;
  deposit.CalculateDeposit(params);

  EXPECT_DOUBLE_EQ(6703030.812225, params.profit * 100 / 100);
  EXPECT_DOUBLE_EQ(1001602.305275, params.sum_nalog);
  EXPECT_DOUBLE_EQ(7804633.1174999997, params.final_amount * 100 / 100);
}

TEST(test_calc_deposit, test_3) {
  s21::DepositParams params;
  params.initial_amount = 100000054300;
  params.procent = 0.7;
  params.month_time = 123;
  params.nalog = 13.0;
  params.deposit_amount = 0.0;     // Initialize deposit amount
  params.withdrawal_amount = 0.0;  // Initialize withdrawal amount
  params.profit = 0.0;
  params.compound_frequency = 0.0;
  params.final_amount = 0.0;
  params.capitalization = 0.0;
  params.sum_nalog = 0.0;

  s21::ModelDeposit deposit;
  deposit.CalculateDeposit(params);

  EXPECT_DOUBLE_EQ(6242253389.541749, params.profit * 100 / 100);
  EXPECT_DOUBLE_EQ(932750506.4832499, params.sum_nalog * 10 / 10);
  EXPECT_DOUBLE_EQ(107175058196.02499, params.final_amount * 10 / 10);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
